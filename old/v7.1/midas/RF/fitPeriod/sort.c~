#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{
  double T=8.48409;
  if(ptr->channel==chn)
  if(ptr->waveform_length!=0)
    {
      get_sin_par(T,ptr->waveform_length,waveform,&spar);
      
      /* printf("A  %f\n",spar.A); */
      /* printf("t0 %f\n",spar.t0); */
      /* printf("C  %f\n",spar.C); */
      h->Reset();
      f->SetParameter(0,spar.A);
      f->SetParameter(1,spar.t0);
      f->SetParameter(2,T);
      f->SetParameter(3,spar.C);


      for(Int_t i=0;i<ptr->waveform_length;i++)
	{
	  h->Fill(i,waveform[i]);
	}
    
       h->Fit("RFfit","QN","",0.,(Double_t)ptr->waveform_length-1);
       Double_t chi2=f->GetChisquare();
       Int_t ndf=f->GetNDF(); 
       chi2/=ndf;
       if(chi2<max_chisq)
	 {
	   n++;
	   t+=f->GetParameter(2);
	   t2+=f->GetParameter(2)*f->GetParameter(2);
	   g->Fill(f->GetParameter(2));
	 }
    }

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  if(argc!=4)
    {
      printf("\n ./RF_fitFrequency midas_input_data_file_name channel max_chisq\n");
      exit(-1);
    }

  chn=atoi(argv[2]);
  max_chisq=atof(argv[3]);

 
  h=new TH1D("Waveform","Waveform",MAX_SAMPLES,0,MAX_SAMPLES);
  g=new TH1D("RF Period","RF_Period",1000,6,10);
  f=new TF1("RFfit","[3]+[0]*sin(6.283185307*(x-[1])/[2])",0,MAX_SAMPLES);
  f->SetParName(0,"A");
  f->SetParName(1,"t0");
  f->SetParName(2,"T");  
  f->SetParName(3,"C");
  t=0.;
  t2=0.;
  n=0.;

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("\n"); 
/* display results */
  printf("Program shows chisq/ndf for RF period fit\n");
  t/=n;
  t2/=n;
  t2-=t*t;
  t2=sqrt(t2/n);
  printf("Mean period [clock samples]  %16.10f\n",t);
  printf("      error [clock samples]  %16.10f\n",t2);
 
  theApp=new TApplication("App", &argc, argv);
  c = new TCanvas("Chisq", "Chisq",10,10, 700, 500);
  gPad->SetLogy();
  g->Draw();
  theApp->Run(kTRUE);
 

  return 0;



}
