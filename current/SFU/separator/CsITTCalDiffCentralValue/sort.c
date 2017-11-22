#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int csi,pos1;
  double thit,tgate;
  bool first_hit = true;
  double t1=0;

  int64_t one=1,none=-1,kill;
  int64_t flag_csi[4];
  
  //if((data->h.setupHP&RF_BIT)==0)
  //  return SEPARATOR_DISCARD;
  
  if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  
  memset(flag_csi,0,sizeof(flag_csi));

	//check the number of hits, make sure there are at least 2
	int numHits=0;
	if(cev->csiarray.h.FT>0)
    for(pos1=1;pos1<NCSI;pos1++) //look at each CsI position
      if((cev->csiarray.h.THP[pos1/64]&(one<<pos1%64))!=0) //is there a hit in the detector?
      	numHits++;
  if(numHits<2)
  	return SEPARATOR_DISCARD; //don't keep events without enough hits

  //Given any number of CsI detector hits during the event, find the time at which 
  //the first and second hits occur.
  if(cev->csiarray.h.FT>0)
    for(pos1=1;pos1<NCSI;pos1++) //look at each CsI position
      if((cev->csiarray.h.THP[pos1/64]&(one<<pos1%64))!=0) //is there a hit in the detector?
        {
          thit=cev->csiarray.csi[pos1].T/cal_par->csiarray.contr_t;
          if(abs(thit-csiTVal)<=half_gate_length)//check whether hit is near central value
          	{
				      if (first_hit == true)
				        {
				          first_hit = false;
				          t1=thit;
				        }
				      else if (t1>thit)
				        {
				          t1=thit; //assign the time of the first hit
				        }
            }
        }

  //Open a gate after the first hit, if the subsequent hits fall within the time window 
  //flag them to be preserved.
  tgate=t1+gate_length; //time at the end of the gatee
	  if(cev->csiarray.h.FT>0)
	    for(pos1=1;pos1<NCSI;pos1++) //look at each CsI position
	      if((cev->csiarray.h.THP[pos1/64]&(one<<pos1%64))!=0) //is there a hit in the detector?
	        {
	          thit=cev->csiarray.csi[pos1].T/cal_par->csiarray.contr_t;
	          if(abs(thit-csiTVal)<=half_gate_length)//check whether hit is near central value
	          	{
					      if ((corr==1)&&(thit<=tgate)) //check whether the hit is in the gate
					        flag_csi[pos1/64]|=(one<<pos1%64); //flag the correlated hit for preservation
					      else if ((corr!=1)&&(thit>tgate))
					      	flag_csi[pos1/64]|=(one<<pos1%64); //flag the uncorrelated hit for preservation
	          	}
	        }

  free(cev);
  
  //drop csi out of the time limits
  for(csi=1;csi<NCSI;csi++)
    if((data->csiarray.h.TSHP[csi/64]&(one<<csi%64))!=0)
      if((flag_csi[csi/64]&(one<<csi%64))==0)
				{
					memset(&data->csiarray.csi[csi],0,sizeof(channel));
					memset(&data->csiarray.wfit[csi],0,sizeof(ShapePar));
					memset(&data->csiarray.t0[csi],0,sizeof(double));
					data->csiarray.h.Efold--;
					data->csiarray.h.Tfold--;	  
					data->csiarray.h.TSfold--;
					kill=none-(one<<csi%64);
					data->csiarray.h.TSHP[csi/64]&=kill;
					data->csiarray.h.EHP[csi/64]&=kill;
					data->csiarray.h.THP[csi/64]&=kill;
				}
  
  if(data->csiarray.h.TSfold<=0)
    {
      kill=none-CsIArray_BIT; 
      data->h.setupHP&=kill;
      memset(&data->csiarray,0,sizeof(CsIArray));
    }
  
  /* if((data->h.setupHP&RF_BIT)==0) */
  /*   return SEPARATOR_DISCARD; */
  
  if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;
  
  //check that there are 2 CsI events 
  if(data->csiarray.h.TSfold<2)
  	return SEPARATOR_DISCARD;
  
  encode(data,output,enb);
  
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  //TCanvas *canvas;
  //TApplication *theApp;
  
  if((argc!=3)&&(argc!=4))
    {
      printf("\n ./separate_CsIArray_TTCalDiffCentralValue master_file_name CsI_val correlated\n");
      printf("\n Separates out particle-particle coincidences where the subsequent particles arrive within a (calibrated) time gate.  Discards any particle hits which arrive outside of the time gate (with respect to the first hit).\nThe time gate length is specified in the CsI array calibration parameters file (under 'CSIARRAY_TTCal_gate_length').\nAll hits must be around the central time value (+/-the gate length) specified by the parameter 'CsI_val'.\n\nThe third argument should be set to 'no' to sort uncorrelated data (events outside the time gate), and 'yes' (or left empty) to sort correlated data.\n");
      exit(-1);
    }
  
  printf("Program sorts data separated on CsIArray timing.\n");
  corr=1;
  if(argc==4)
  	if(strcmp(argv[3],"no")==0)
  		{
  			corr=0;
  			printf("Uncorrelated data will be separated.\n");
  		}
  
  h = new TH1D("Csiarray Time","Csiarray Time",S16K,-S8K,S8K);
  h->Reset();
  
  g = new TH1D("Csiarray Gate","Csiarray Gate",S16K,-S8K,S8K);
  g->Reset();

	csiTVal=atof(argv[2]);
  gate_length=0;
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

  read_master(argv[1],name);

  if(name->flag.inp_data!=1)
    {
      printf("\nInput data file not defined\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.CSIARRAY_cal_par==1)
    {
      printf("\nCSIARRAY calibration read from the file:\n %s\n",name->fname.CSIARRAY_cal_par);
      initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
    }
  else
    {
      printf("\nCSIARRAY calibration parameters not defined\n");
      exit(-1);
    }
  
  if(name->flag.out_data!=1)
    {
      printf("\nOutput data file not defined\n");
      exit(EXIT_FAILURE);
    }
 
  if((output=fopen(name->fname.out_data,"w"))==NULL)
    {
      printf("\nI can't open output file %s for writing\n",name->fname.out_data);
      exit(-2);
    }
  memset(enb,0,sizeof(enb));
  enb[0]=BUFFER_TAG;
  enb[1]++;
  enb[1]++;
  gate_length=cal_par->csiarray.TTCal_gate_length;
  half_gate_length=gate_length/2.;
  printf("Gate length is %f\n",gate_length);
  
  sort(name);
  
	//save the last buffer which will be dropped otherwise
	//if enb[1]==2 then the buffer contains no data, only the header
	if(enb[1]>2)
		fwrite(enb,sizeof(int),BUFFSIZE,output);
  
  /*theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("TTCalDiff", "TTCalDiff",10,10, 500, 300);
  gPad->SetLogz(1);
  g->SetLineColor(2);
  h->Draw();
  g->Draw("same");
  theApp->Run(kTRUE);*/
}
