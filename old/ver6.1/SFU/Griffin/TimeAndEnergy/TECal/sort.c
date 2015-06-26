#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos,col;
  double e,t;
  long long int one=1;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_GRIFFIN(data,&cal_par->gr,&cev->gr);
  if(cev->gr.h.FH>0)
    for(pos=1;pos<NPOSGRIF;pos++)
      if((cev->gr.h.HHP&(one<<(pos-1)))!=0)
  	if(cev->gr.det[pos].hge.FH>0)
  	  for(col=0;col<NCOL;col++)
  	    if((cev->gr.det[pos].hge.HHP&(one<<col))!=0)
  	      if(cev->gr.det[pos].ge[col].h.FH>0)
  		if((cev->gr.det[pos].ge[col].h.HHP&1)!=0)
  		  {
		    t=cev->gr.det[pos].ge[col].seg[0].T/cal_par->gr.contr_t;
  		    e=cev->gr.det[pos].ge[col].seg[0].E/cal_par->gr.contr_e;
		    
		    h_t->Fill(t);
		    h_e->Fill(e);
		    h->Fill(t,e);
		  }
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *c1,*c2,*c3;
  TApplication *theApp;

  if(argc!=2)
    {
      printf("Griffin_TECal master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("TECal","TECal",S1K,512,S1K,S8K,0,S8K);
  h->Reset();
  h_t = new TH1D("TECal_t","TECal_t",S1K,512,S1K);
  h_t->Reset();
  h_e = new TH1D("TECal_e","TECal_e",S8K,0,S8K);
  h_e->Reset();
  
  printf("Program sorts TECal histograms for the GRIFFIN.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);
  
  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.GRIFFIN_cal_par==1)
    {
      printf("Griffin calibration read from %s.\n",name->fname.GRIFFIN_cal_par);
      initialize_GRIFFIN_calibration(&cal_par->gr,name->fname.GRIFFIN_cal_par);
    }
  else
    {
      printf("ERROR!!! Griffin calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  sort(name);

  theApp=new TApplication("App", &argc, argv);
  c1 = new TCanvas("TECal", "TECal",10,10, 500, 700);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");

  c2 = new TCanvas("TECal_t", "TECal_t",10,10, 500, 300);
  gPad->SetLogz(1);
  h_t->Draw("");
  
  c3 = new TCanvas("TECal_e", "TECal_e",10,10, 500, 300);
  gPad->SetLogz(1);
  h_e->Draw("");
  
  theApp->Run(kTRUE);
}