#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
  ShapePar *par;
  double e,t;
  double chisq;

  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      fit_TIFFIN_waveform(d,waveform,par,&wpar);
      chisq=par->chisq/par->ndf;
      if(chisq>chimin)
	if(chisq<chimax)
	  {
	    e=par->t[1];
	    t=par->t[2];
	    h->Fill(e,t);
	  }
    }
  free(par);
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

  int ac;
  char *av[10];
  TCanvas *c;  
  TApplication *theApp;

 if(argc!=5)
    {
      printf("\n ./wfit_TIFFIN_E midas_input_data_file_name channel chimin chimax\n");
      exit(-1);
    }

  
  chn=atoi(argv[2]);
  chimin=atof(argv[3]);
  chimax=atof(argv[4]);
  h=new TH2D("E/T","R/T",S4K,0,S4K,S4K,0,S4K);
  h->Reset();
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("Program sorts energy spectrum for the selected channel\n");
  theApp=new TApplication("App", &ac, av);
  c = new TCanvas("chisq", "chisq",10,10, 700, 500);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  theApp->Run(kTRUE);
}
