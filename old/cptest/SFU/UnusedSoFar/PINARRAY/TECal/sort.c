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
	}
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;

  if(argc!=2)
    {
      printf("\n ./PINARRAY_ECal master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("Cal. Time-Energy","Cal. Time-Energy",S4K,0,S4K-1,S4K,0,S4K-1);
  h->Reset();

  printf("Program sorts calibrated energy histogram for the PINARRAY \n");
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

  if(name->flag.PINARRAY_cal_par==1)
        {
          printf("\nPINARRAY calibration read from the file:\n %s\n",name->fname.PINARRAY_cal_par);
          initialize_PINARRAY_calibration(&cal_par->pinarray,name->fname.PINARRAY_cal_par);
	  
        }
      else
        {
          printf("\nPINARRAY calibration parameters not defined\n");
          exit(EXIT_FAILURE);
        }



  sort(name);

 
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Cal. Time-Energy", "Cal. Time-Energy",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

