#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
 
 Int_t d;
 WaveFormPar wpar;

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      wpar.baseline_range=RANGE;
      get_t10t90(d,waveform,&wpar);
      if(wpar.t10t90_flag==1)
	{
	  get_two_risetime_par(d,waveform,par,&wpar);
	  h->Fill(par->chisq/par->ndf);
	}
    }

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

  TCanvas *c;  
  TApplication *theApp;
  int ac;
  char *av[10];

 if(argc!=6)
    {
      printf("\n ./wfit_lin_chisq midas_input_data_file_name channel trc tf ts\n");
      exit(-1);
    }
  par=(TwoRiseTimePar*)malloc(sizeof(TwoRiseTimePar));
  chn=atoi(argv[2]);
  par->t[0]=0;
  par->t[1]=atof(argv[3]);
  par->t[2]=atof(argv[4]);
  par->t[3]=atof(argv[5]);
  theApp=new TApplication("App", &ac, av);
  h=new TH1D("Chisq","Chisq",10000,0,1000);

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */

  printf("Program shows chisq for two-risetime waveform fit\n");
  c = new TCanvas("Chisq", "Chisq",10,10, 700, 500);
  gPad->SetLogy();
  h->GetXaxis()->SetTitle("Chisq");
  h->GetYaxis()->SetTitle("Counts");
  h->Draw();
  theApp->Run(kTRUE);


}
