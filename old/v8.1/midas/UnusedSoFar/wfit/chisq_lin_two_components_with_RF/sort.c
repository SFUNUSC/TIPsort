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
      wpar.baseline_range=CSI_BASELINE_RANGE;
      get_exclusion_zone_for_CsI(d,waveform,&wpar);
      get_shape_with_RF(4,d,waveform,par,&wpar);
      chisq=par->chisq/par->ndf;
      h->Fill(chisq);
      if(chisq>S32K) chisq=S32K-4;
      if(chisq<0) chisq=S32K-2;

      hist[(int)rint(chisq)]++;
    }
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  FILE *output;
  int ac;
  char *av[10];
  TCanvas *c;  
  TApplication *theApp;

 if(argc!=7)
    {
      printf("\n ./wfit_chisq_lin_two_components midas_input_data_file_name channel trc tf ts trf\n");
      exit(-1);
    }

  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));
  chn=atoi(argv[2]);
  par->t[1]=atof(argv[3]);
  par->t[2]=atof(argv[4]);
  par->t[3]=atof(argv[5]);
  par->rf[0]=atof(argv[6]);

  h=new TH1D("chisq","chisq",S2K,-4,S16K-4);
  h->Reset();
  memset(hist,0,sizeof(hist));
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("Program fit chisq spectrum for the selected channel\n");

  if((output=fopen("CSI_fit_chisq.mca","w"))==NULL)
    {
      printf("\nI can't open file %s\n","CSI_fit_chisq.mca");
      exit(EXIT_FAILURE);
    }
 
  fwrite(hist,S32K*sizeof(int),1,output);

  theApp=new TApplication("App", &ac, av);
  c = new TCanvas("chisq", "chisq",10,10, 700, 500);
  h->Draw();
  theApp->Run(kTRUE);
}
