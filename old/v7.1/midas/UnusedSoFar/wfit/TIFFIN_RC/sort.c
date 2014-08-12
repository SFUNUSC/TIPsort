#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
  ShapePar *par;
  double a,b;
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
	    a=par->am[0]-wpar.s0;
	    b=par->am[1]-wpar.s1;
	    
	    h->Fill(-a/b);
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
      printf("\n ./wfit_TIFFIN_RC midas_input_data_file_name channel chimin chimax\n");
      exit(-1);
    }

  
  chn=atoi(argv[2]);
  chimin=atof(argv[3]);
  chimax=atof(argv[4]);
  h=new TH1D("E/T","R/T",S1K,-S1K/2,S1K/2);
  h->Reset();
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("Program sorts spectrum of the fitted RC slope for the selected channel\n");
  theApp=new TApplication("App", &ac, av);
  c = new TCanvas("RC", "RC",10,10, 700, 500);
  h->Draw();
  theApp->Run(kTRUE);
}
