#include "sort.h"

int analyze_data(raw_event *data)
{
  int diff;
  ts_min=0;
  ts_min=data->h.timestamp_up_min&0x00ffffff;
  ts_min=ts_min<<24;
  ts_min|=(data->h.timestamp_min&0x00ffffff);
 
  ts_max=0;
  ts_max=data->h.timestamp_up_max&0x00ffffff;
  ts_max=ts_max<<24;
  ts_max|=(data->h.timestamp_max&0x00ffffff);
 
  if(stored)
    if(ts_min==ts_max)
      if((data->h.trig_num-store_trig_num)==1)
	{
	  diff=ts_min-store_ts;
	  h->Fill(diff/100);
	}
  
  if(ts_min==ts_max)
    {
      store_trig_num=data->h.trig_num;
      store_ts=ts_min;
      stored=1;
    }
  else
    stored=0;
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
      printf("check_EventTimeDifference sfu_data_file\n");
      exit(-1);
    }
  
  h = new TH1D("Time Difference [us]","Time Difference [us]",S4K,0,S4K-1);
  h->Reset();
  
  printf("Program sorts histogram of time differences between consecutive events.\n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name); 
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Time Difference", "Time Difference",10,10, 500, 300);
  gPad->SetLogy(1);
  h->Draw();
  
  theApp->Run(kTRUE);
}
