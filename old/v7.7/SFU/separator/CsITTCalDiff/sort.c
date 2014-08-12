#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int csi,pos1,pos2;
  double t1,t2,tdiff;
  
  long long int one=1,none=-1,kill;
  long long int flag_csi,drop;
  
  if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;
  
  if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  
  flag_csi=0;
  
  if(cev->csiarray.h.FT>0)
    for(pos1=1;pos1<NCSI;pos1++)
      if((cev->csiarray.h.THP&(one<<pos1))!=0)
	{
	  t1=cev->csiarray.csi[pos1].T/cal_par->csiarray.contr_t;
	  for(pos2=pos1+1;pos2<NCSI;pos2++)
	    if((cev->csiarray.h.THP&(one<<pos2))!=0)
	      {
		t2=cev->csiarray.csi[pos2].T/cal_par->csiarray.contr_t;
		tdiff=t1-t2;
		
		h->Fill(tdiff);
		if((tdiff>=low) && (tdiff<=high))
		  {
		    g->Fill(tdiff);
		    flag_csi|=(one<<pos1);
		    flag_csi|=(one<<pos2);
		  }
	      }
	}
  
  free(cev);
  
  //drop csi out of the time limits
  for(csi=1;csi<NCSI;csi++)
    if((data->csiarray.h.TSHP&(one<<csi))!=0)
      if((flag_csi&(one<<csi))==0)
	{
	  memset(&data->csiarray.csi[csi],0,sizeof(channel));
	  memset(&data->csiarray.wfit[csi],0,sizeof(ShapePar));
	  memset(&data->csiarray.t0[csi],0,sizeof(double));
	  data->csiarray.h.Efold--;
	  data->csiarray.h.Tfold--;	  
	  data->csiarray.h.TSfold--;
	  kill=none-(one<<csi);
	  data->csiarray.h.TSHP&=kill;
	  data->csiarray.h.EHP&=kill;
	  data->csiarray.h.THP&=kill;
	}
  
  if(data->csiarray.h.TSfold<=0)
    {
      kill=none-CsIArray_BIT; 
      data->h.setupHP&=kill;
      memset(&data->csiarray,0,sizeof(CsIArray));
    } 
  
  if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;
  
  if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;
  
  encode(data,output,enb);
  
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  
  if(argc!=4)
    {
      printf("\n ./separate_CsIArray_TTCalDiff master_file_name low high\n");
      exit(-1);
    }
  
  h = new TH1D("Csiarray Time","Csiarray Time",S16K,-S8K,S8K);
  h->Reset();
  
  g = new TH1D("Csiarray Gate","Csiarray Gate",S16K,-S8K,S8K);
  g->Reset();
  
  low=atof(argv[2]);
  high=atof(argv[3]);
  
  printf("Program sorts calibrated 2D histogram for CsIArray timing.\n");
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
  
  sort(name);
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("TTCalDiff", "TTCalDiff",10,10, 500, 300);
  gPad->SetLogz(1);
  g->SetLineColor(2);
  h->Draw();
  g->Draw("same");
  theApp->Run(kTRUE);
}
