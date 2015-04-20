#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  double e1=0,e2=0;
  long long int one=1;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_GRIFFIN(data,&cal_par->gr,&cev->gr);

  if(cev->gr.h.FE>0)
    if(pos<NPOSGRIF)
      if((cev->gr.h.EHP&(one<<(pos-1)))!=0)
  	if(cev->gr.det[pos].hge.FE>0)
	  {
	    if((cev->gr.det[pos].hge.EHP&(one<<col1))!=0)
	      if(cev->gr.det[pos].ge[col1].h.FE>0)
		if((cev->gr.det[pos].ge[col1].h.EHP&1)!=0)
		  if((cev->gr.det[pos].hge.EHP&(one<<col2))!=0)
		    if(cev->gr.det[pos].ge[col2].h.FE>0)
		      if((cev->gr.det[pos].ge[col2].h.EHP&1)!=0)
			{
			  e1=cev->gr.det[pos].ge[col1].seg[0].E/cal_par->gr.contr_e;
			  e2=cev->gr.det[pos].ge[col2].seg[0].E/cal_par->gr.contr_e;
			}
	    
	    if(e1==0) 
	      {
		e1=S8K-2000;
		count++;
	      }
	    if(e2==0) 
	      {
		e2=S8K-2000;
		count++;
	      }
	    h_e1->Fill(e1);
	    h_e2->Fill(e2);
	    h->Fill(e1,e2);
	  }

  free(cev);
  return SEPARATOR_DISCARD;
}
/*============================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *c1,*c2,*c3;
  TApplication *theApp;
  
  if(argc!=5)
    {
      printf("Griffin_2DECal master_file_name pos col1 col2\n");
      exit(-1);
    }
  
  h = new TH2D("2DECal","2DECal",S4K,0,S16K,S4K,0,S16K);
  h->Reset();
  h_e1 = new TH1D("2DECal_e1","2DECal_e1",2*S8K,0,2*S8K-1);
  h_e1->Reset();
  h_e2 = new TH1D("2DECal_e2","2DECal_e2",2*S8K,0,2*S8K-1);
  h_e2->Reset();
  
  printf("Program sorts 2DECal hists for specified pos and 2 col of GRIFFIN.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);
  
  pos=atoi(argv[2]);
  col1=atoi(argv[3]);
  col2=atoi(argv[4]);

  if(col2<col1)
    {
      printf("ERROR!!! Please specify the two desired cores (B=0, G=1, R=2, W=3)");
      printf("in increasing order!!!\n");
      exit(EXIT_FAILURE);
    }
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

  //theApp=new TApplication("App", &argc, argv);
  c1 = new TCanvas("2DECal","2DECal",10,10, 500, 700);
  c2 = new TCanvas("2DECal_e1","2DECal_e1",10,10, 500, 300);
  c3 = new TCanvas("2DECal_e2","2DECal_e2",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(0);
  
  TFile *f = new TFile(name->fname.root_output_file,"RECREATE");
  f->cd();
  
  h->Draw("COLZ");
  h->Write("2DECal");
  h_e1->Draw("");
  h_e1->Write("2DECal_e1");
  h_e2->Draw("");
  h_e2->Write("2DECal_e2");
  
  f->Close();
  //theApp->Run(kTRUE);
  printf("Bad reported energies %d times\n.",count);
}
