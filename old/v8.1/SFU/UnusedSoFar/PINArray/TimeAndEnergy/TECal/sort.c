#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos;
  double e,t;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_PINARRAY(data,&cal_par->pinarray,&cev->pinarray);
  
  if(cev->pinarray.h.FH>0)
    for(pos=1;pos<NPIN;pos++)
      if((cev->pinarray.h.HHP&(one<<pos))!=0)
	{
	  t=cev->pinarray.pin[pos].T/cal_par->pinarray.contr_t;
	  e=cev->pinarray.pin[pos].E/cal_par->pinarray.contr_e;
	  h->Fill(t,e);
	  h_t->Fill(t);
	  h_e->Fill(e);
	}
  return SEPARATOR_DISCARD;
}
/*====================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *c1,*c2,*c3;
  TApplication *theApp;
  
  if(argc!=2)
    {
      printf("PINArray_TECal master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("TECal","TECal",S1K,0,S4K-2,S1K,0,S32K);
  h->Reset();
  h_t = new TH1D("TECal_t","TECal_t",S1K,0,S4K-2);
  h_t->Reset();
  h_e = new TH1D("TECal_e","TECal_e",S1K,0,S32K);
  h_e->Reset();


  printf("Program sorts TECal histograms for the PINArray.\n");
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
  
  if(name->flag.PINARRAY_cal_par==1)
    {
      printf("PINArray calibration read from %s\n",name->fname.PINARRAY_cal_par);
      initialize_PINARRAY_calibration(&cal_par->pinarray,name->fname.PINARRAY_cal_par);
    }
  else
    {
      printf("ERROR!!! PINArray calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }


  if(name->flag.root_output_file==1) printf("Root output file is: %s\n",name->fname.root_output_file);
  else
    {
      printf("ERROR!!! Root output file not defined!\n");
      exit(EXIT_FAILURE);
    }

  sort(name);
  
  //theApp=new TApplication("App", &argc, argv);
  c1 = new TCanvas("TECal", "TECal",10,10, 500, 300);
  c2 = new TCanvas("TECal_t", "TECal_t",10,10, 500, 300);
  c3 = new TCanvas("TECal_e", "TECal_e",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);

  TFile *f = new TFile(name->fname.root_output_file,"RECREATE");
  f->cd();
  h->Draw("COLZ");
  h->Write("TECal");
  h_t->Draw("");
  h_t->Write("TECal_t");
  h_e->Draw("");
  h_e->Write("TECal_e");

  f->Close();
  //theApp->Run(kTRUE);
}

