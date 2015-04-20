#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos;
  double e;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_PINARRAY(data,&cal_par->pinarray,&cev->pinarray);
  if(cev->pinarray.h.FE>0)
    for(pos=1;pos<NPIN;pos++)
      if((cev->pinarray.h.EHP&(one<<pos))!=0)
	{
	  e=cev->pinarray.pin[pos].E/cal_par->pinarray.contr_e;
	  
	  if((e<0) || (e>S32K)) e=S32K-1000;
	  hist[pos][(int)rint(e)]++;
	  h->Fill(e,pos);
	}
  free(cev);
  return SEPARATOR_DISCARD;
}
/*========================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  
  if(argc!=2)
    {
      printf("PINArray_ECal master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("ECal","ECal",S8K,0,S32K-8,NPIN+1,0,NPIN);
  h->Reset();

  printf("Program sorts ECal histograms for the PINArray.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  memset(hist,0,sizeof(hist));

  read_master(argv[1],name);
  
  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined.\n");
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

  sort(name);
  
  if((output=fopen("PINArray_ECal.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }

  for(int pos=0;pos<NPIN;pos++) fwrite(hist[pos],S32K*sizeof(int),1,output);
  
  fclose(output);
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("ECal", "ECal",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  //theApp->Run(kTRUE);
}
