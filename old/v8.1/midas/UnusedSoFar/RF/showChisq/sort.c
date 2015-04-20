#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{
   double T=8.72;
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
       g->Fill(chi2/ndf);
    }

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  if(argc!=3)
    {
      printf("\n ./RF_showChisq midas_input_data_file_name channel\n");
      exit(-1);
    }

  chn=atoi(argv[2]);
 
 
  h=new TH1D("Waveform","Waveform",MAX_SAMPLES,0,MAX_SAMPLES);
  g=new TH1D("Chisq/NDF","Chisq/NDF",1000,0,10);
  f=new TF1("RFfit","[3]+[0]*sin(6.283185307*(x-[1])/[2])",0,MAX_SAMPLES);
  f->SetParName(0,"A");
  f->SetParName(1,"t0");
  f->SetParName(2,"T");  
  f->SetParName(3,"C");

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("\n"); 
/* display results */

 printf("Program shows chisq/ndf for RF period fit\n");
 theApp=new TApplication("App", &argc, argv);
 c = new TCanvas("Chisq", "Chisq",10,10, 700, 500);
 gPad->SetLogy();
 g->Draw();
 theApp->Run(kTRUE);

  return 0;



}
