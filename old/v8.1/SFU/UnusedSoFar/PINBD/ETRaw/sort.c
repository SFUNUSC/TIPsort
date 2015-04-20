#include "sort.h"

int analyze_data(raw_event *data)
{

  int t,e;
  
  if(data->pinbd.h.Tfold>0)
      if(data->pinbd.h.THP!=0)
	{
	  t=data->pinbd.pin.cfd&0x00ffffff;
	  t-=(data->pinbd.pin.timestamp*16)&0x00ffffff;
	  if((data->h.setupHP&RF_BIT)!=0)
	    {
	      t-=(int)data->rf.sin.t0;
	      t+=S8K;
	      if(t<0)t=S4K-2;
	      if(t>S4K) t=S4K-3;
	    }
	  else
	    t=S4K-4;
	  if(data->pinbd.h.Efold>0)
	    if(data->pinbd.h.EHP!=0)
	      {
		e=data->pinbd.pin.charge/8;
		h->Fill(e,t);
	      }
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
      printf("\n ./PINBD_ETRaw sfu_input_data_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("PINBD Raw Time","PINBD Raw Time",S4K,0,S4K,S4K,0,S4K);
  h->Reset();

  printf("Program sorts raw time histogram for the beam dump PIN \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);

   
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("PINBD Raw Energy Time", "PINBD Raw Energy Time",10,10, 500, 300);
  gStyle->SetPalette(1);
  gPad->SetLogz(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

