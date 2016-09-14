#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long ts;
  double t;
  
  ts=data->h.timestamp_up_max&0x00ffffff;
  ts=ts<<24;
  ts|=(data->h.timestamp_max&0x00ffffff);
  t=ts/100000000.;
  h->Fill(t);

  return SEPARATOR_DISCARD;
}
/*============================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  
  if(argc!=2)
    {
      printf("check_Rate sfu_input_data\n");
      exit(-1);
    }
  
  h = new TH1D("Events per second","Events per second",S32K,0,S32K-1);
  h->Reset();
  
  printf("Program sorts number of events per second histogram \n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type)); 
  strcpy(name->fname.inp_data,argv[1]);
  sort(name); 
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("TrigBits", "TrigBits",10,10, 500, 300);
  gPad->SetLogy(1);
  h->Draw();
  
  theApp->Run(kTRUE);
}
