#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos,col;
  double traw,tcal;
  //int take=0;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  //printf("FT: %d\n",cev->tg.h.FT);
  if(cev->tg.h.FT>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.THP&(1<<(pos-1)))!=0)
	if(cev->tg.det[pos].hge.FT>0)
	  for(col=0;col<NCOL;col++)
	    if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
	      if(cev->tg.det[pos].ge[col].h.FT>0)
		if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
		  {
		    traw=data->tg.det[pos].ge[col].seg[0].cfd&0x00ffffff;
		    traw-=(data->tg.det[pos].ge[col].seg[0].timestamp*16)&0x00ffffff;
		    traw-=data->rf.sin.t0;
		    traw+=S16K;

		    tcal=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;

		    h->Fill(traw,tcal);
		  }
		  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*==========================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  char title[132];
  
  if(argc!=2)
    {
      printf("TIGRESS_TTCal master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("Tigress TRaw vs TCal","Tigress TRaw vs TCal",S8K,0,S16K-1,S8K,0,S16K-1);
  h->Reset();
  
  printf("Program sorts TTCal histogram for addback TIGRESS cores.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);
  
  if(name->flag.inp_data!=1)
    {
      printf("ERROR! Input data file not defined.\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calibration read from: %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR! TIGRESS calibration parameters not defined.\n");
      exit(EXIT_FAILURE);
    }
  
  sort(name);
  
  sprintf(title,"Tigress_TRaw_v_TCal.root");
  TFile f(title, "recreate");
  h->GetXaxis()->SetTitle("Tigress TRaw [arb]");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("Tigress TCal [ns]");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  gStyle->SetPalette(1);
  h->Write();
}
