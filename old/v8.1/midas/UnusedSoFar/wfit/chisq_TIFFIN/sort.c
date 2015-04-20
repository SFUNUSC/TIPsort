#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
  ShapePar *par;
  double chisq;


  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      fit_TIFFIN_waveform(d,waveform,par,&wpar);
      chisq=par->chisq/par->ndf;
      h->Fill(chisq);
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

 if(argc!=3)
    {
      printf("\n ./wfit_TIFFIN midas_input_data_file_name channel\n");
      exit(-1);
    }

  
  chn=atoi(argv[2]);
 

  h=new TH1D("chisq","chisq",S2K,-4,S16K-4);
  h->Reset();
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("Program fit chisq spectrum for the selected channel\n");
  theApp=new TApplication("App", &ac, av);
  c = new TCanvas("chisq", "chisq",10,10, 700, 500);
  h->Draw();
  theApp->Run(kTRUE);
}
