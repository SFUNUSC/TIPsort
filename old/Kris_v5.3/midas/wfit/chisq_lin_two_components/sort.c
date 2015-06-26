#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
  double chisq;

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      fit_waveform(d,waveform,par,&wpar,theApp);
      chisq=par->chisq/par->ndf;
      h->Fill(chisq);
   
    }
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

  int ac;
  char *av[10];
  TCanvas *c;  
 

 if(argc!=7)
    {
      printf("\n ./wfit_chisq_lin_two_components midas_input_data_file_name channel trc tf ts tp\n");
      exit(-1);
    }

  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));
  chn=atoi(argv[2]);
  par->t[1]=atof(argv[3]);
  par->t[2]=atof(argv[4]);
  par->t[3]=atof(argv[5]);
  par->t[4]=atof(argv[6]);


  h=new TH1D("chisq","chisq",S2K,-4,S16K-4);
  h->Reset();
 theApp=new TApplication("App", &ac, av);
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("Program fit chisq spectrum for the selected channel\n");
 
  c = new TCanvas("chisq", "chisq",10,10, 700, 500);
  h->Draw();
  theApp->Run(kTRUE);
}