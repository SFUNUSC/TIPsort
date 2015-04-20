#include "sort.h"
/*================================================================*/
Double_t myfunction(Double_t *y, Double_t *par)
{
 
  Double_t x;
 
  x=y[0]-par[1];

  if(x<=0) 
    return par[0];
  else
  return par[0]+(par[2]+par[3])*exp(-x/par[4])-par[2]*exp(-x/par[5])-par[3]*exp(-x/par[6]);

}

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
  double a;
  
  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
      {
	memset(&wpar,0,sizeof(WaveFormPar));
	wpar.baseline_range=RANGE;
	get_t10t90(d,waveform,&wpar);
	a=wpar.max-wpar.b;
	if(a>amp)
	  if(wpar.t10t90_flag==1)
	    if(wpar.t10t90>low)
	      if(wpar.t10t90<high)
		{
		
		  if(h!=NULL) delete h;
		  h=new TH1D("Waveform","Waveform",ptr->waveform_length,0,ptr->waveform_length);
		  for(Int_t i=0;i<ptr->waveform_length;i++)
		    h->Fill(i,waveform[i]);
		
		  f->SetParameter(0,wpar.b);
		  f->SetParameter(1,wpar.t10);
		  f->SetParameter(2,0.5*a);
		  f->SetParameter(3,0.5*a);
		  f->FixParameter(4,TRC);
		  f->FixParameter(5,70);		
		  f->SetParameter(6,200);  
		  h->Fit("wfit","QN","QN",0,d-1);
		  a=f->GetParameter(6);
		 
		  g->Fill(a,f->GetChisquare());		
	      }
    }
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

  int ac;
  char *av[10];

 if(argc!=8)
    {
      printf("\n ./wfit_slow_component midas_input_data_file_name channel low high TRC TF amplitude\n");
      exit(-1);
    }

  chn=atoi(argv[2]);
  low=atof(argv[3]);
  high=atof(argv[4]);
  TRC=atof(argv[5]);
  TF=atof(argv[6]);
  amp=atof(argv[7]);

  f=new TF1("wfit",myfunction,0,MAX_SAMPLES,7);
  f->SetParName(0,"C");
  f->SetParName(1,"t0");
  f->SetParName(2,"A1");
  f->SetParName(3,"A2");
  f->SetParName(4,"TRC");
  f->SetParName(5,"T1");
  f->SetParName(6,"T2");
  f->SetLineColor(kRed);
  f->SetLineWidth(0.3);
  g=new TH2D("TF/chisq","TF/chisq",S1K,0,S1K,1000,0,10);
  theApp=new TApplication("App", &ac, av);
  printf("Program fits fast risetime waveforms for selected channel\n");

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  if(c!=NULL) delete c;
  c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);
  //  gStyle->SetPalette(2);
  g->Draw();
  theApp->Run(kTRUE);

 
}
