#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos;
  double t;
  
  if(data->pinarray.h.Tfold>0)
    for(pos=1;pos<NPIN;pos++)
      if((data->pinarray.h.THP&(one<<pos))!=0)
	{
	  t=data->pinarray.t0[pos]*16;
	  if((data->h.setupHP&RF_BIT)!=0)
	    {
	      t-=data->rf.sin.t0;
	      t*=0.625;
	      t+=S16K;
	      if(t<0)    t=S32K-2000;
	      if(t>S32K) t=S32K-3000;
	    }
	  else
	    t=S32K-4000;
	  
	  hist[pos][(int)rint(t)]++;
	  h->Fill(t,pos);
	}
  return SEPARATOR_DISCARD;
}
/*===========================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  
  if(argc!=2)
    {
      printf("PINArray_TFitRaw sfu_input_data\n");
      exit(-1);
    }
  
  h = new TH2D("TFitRaw","TFitRaw",S32K,0,S32K-1,NPIN+1,0,NPIN);
  h->Reset();
  
  printf("Program sorts TFitRaw histograms for the PINArray.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  memset(hist,0,sizeof(hist));
  strcpy(name->fname.inp_data,argv[1]);
  
  sort(name);
  
  if((output=fopen("PINArray_TFitRaw.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  for(int pos=0;pos<NPIN;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);
  
  fclose(output);
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("TFitRaw","TFitRaw",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  //theApp->Run(kTRUE);
}
