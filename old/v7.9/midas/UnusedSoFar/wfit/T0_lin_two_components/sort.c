#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      wpar.baseline_range=CSI_BASELINE_RANGE;
      get_exclusion_zone_for_CsI(d,waveform,&wpar);
      get_shape(4,d,waveform,par,&wpar);
      h->Fill(par->t[0]);
   
    }
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

  int ac;
  char *av[10];
  TCanvas *c;  
  TApplication *theApp;

 if(argc!=6)
    {
      printf("\n ./wfit_T0_lin_two_components midas_input_data_file_name channel trc tf ts \n");
      exit(-1);
    }

  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));
  chn=atoi(argv[2]);
  par->t[1]=atof(argv[3]);
  par->t[2]=atof(argv[4]);
  par->t[3]=atof(argv[5]);


  h=new TH1D("T0","T0",S2K,-4,S16K-4);
  h->Reset();
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("Program fit chisq spectrum for the selected channel\n");
  theApp=new TApplication("App", &ac, av);
  c = new TCanvas("T0", "T0",10,10, 700, 500);
  h->Draw();
  theApp->Run(kTRUE);
}
