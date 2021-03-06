#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int csi,pos1,pos2;
  double thit1,thit2;
  bool inGate=false;

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

  //For each CsI hit, see if there are other CsI hits
  //within a single gate-length of the hit, and preserve the
  //hit if true
  if(cev->csiarray.h.FT>0)
    for(pos1=1;pos1<NCSI;pos1++) //look at each CsI position
      if((cev->csiarray.h.THP[pos1/64]&(one<<pos1%64))!=0) //is there a hit in the detector?
        {
          thit1=cev->csiarray.csi[pos1].T/cal_par->csiarray.contr_t;
          
          //compare this hit to other hits
          inGate=false;
          for(pos2=1;pos2<NCSI;pos2++) //look at each CsI position
          	if(pos2!=pos1)//check that hits are not the same
      				if((cev->csiarray.h.THP[pos2/64]&(one<<pos2%64))!=0) //is there a hit in the detector?
      					{
      						thit2=cev->csiarray.csi[pos2].T/cal_par->csiarray.contr_t;
      						if(abs(thit2-thit1)<=gate_length)
										inGate=true;
      					}
          
          if ((corr==1)&&(inGate)) //check whether the hit is in the gate
            flag_csi[pos1/64]|=(one<<pos1%64); //flag the correlated hit for preservation
          else if ((corr!=1)&&(inGate==false))
          	flag_csi[pos1/64]|=(one<<pos1%64); //flag the uncorrelated hit for preservation
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
  
  if((argc!=2)&&(argc!=3))
    {
      printf("\n ./separate_CsIArray_TTCalDiff master_file_name correlated\n");
      printf("\n Separates out particle-particle coincidences where the subsequent particles arrive within a (calibrated) time gate.  Discards any particle hits which arrive outside of the time gate (with respect to the first hit).\nThe time gate length is specified in the CsI array calibration parameters file (under 'CSIARRAY_TTCal_gate_length').\n\nThe third argument should be set to 'no' to sort uncorrelated data (events outside the time gate), and 'yes' (or left empty) to sort correlated data.\n");
      exit(-1);
    }
  
  printf("Program sorts data separated on CsIArray timing.\n");
  corr=1;
  if(argc==3)
  	if(strcmp(argv[2],"no")==0)
  		{
  			corr=0;
  			printf("Uncorrelated data will be separated.\n");
  		}
  
  h = new TH1D("Csiarray Time","Csiarray Time",S16K,-S8K,S8K);
  h->Reset();
  
  g = new TH1D("Csiarray Gate","Csiarray Gate",S16K,-S8K,S8K);
  g->Reset();

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
