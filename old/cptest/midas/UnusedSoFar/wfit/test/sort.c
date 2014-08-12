#include "sort.h"
Double_t myfunction(Double_t *y, Double_t *par)
{
  Double_t x=y[0];
  Double_t f=(par[2]+par[3])*exp(-(x-par[1])/trc)-par[2]*exp(-(x-par[1])/tf)-par
[3]*exp(-(x-par[1])/ts);

  if(f<0)
    f=par[0];
  else
    f+=par[0];

  return f;
}

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
 if(ptr->channel==chn)
  if(ptr->waveform_length!=0)
    {
     
      h->Reset();
      for(Int_t i=0;i<ptr->waveform_length;i++)
	{
	  h->Fill(i,waveform[i]);
	}
     
      f->SetParameter(0,12445);
      f->SetParameter(1,220);
      f->SetParameter(2,50);
      f->SetParameter(3,70);
  
      h->Fit("wfit","","",0.,(Double_t)ptr->waveform_length-1);
      h->Draw();
      theApp->Run(kTRUE);
     
    }

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

  int ac;
  char *av[10];
  if(argc!=6)
    {
      printf("\n ./wfit_test midas_input_data_file_name channel trc tf ts\n");
      exit(-1);
    }
  chn=atoi(argv[2]);
  trc=atof(argv[3]);
  tf=atof(argv[4]);
  ts=atof(argv[5]);
  
  theApp=new TApplication("App", &ac, av);
  h=new TH1D("Waveform","Waveform",MAX_SAMPLES,0,MAX_SAMPLES);
  f=new TF1("wfit",myfunction,0,MAX_SAMPLES-1,4);
  f->SetParName(0,"C");
  f->SetParName(1,"t0");
  f->SetParName(2,"nf");
  f->SetParName(3,"ns");
  f->SetLineColor(kRed);
  f->SetLineWidth(0.3);
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("\n"); 
  printf("Program shows waveform fit for selected channel\n");


}
