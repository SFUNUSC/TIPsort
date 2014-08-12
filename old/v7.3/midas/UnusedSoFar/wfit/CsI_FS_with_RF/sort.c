#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
  double s,f;

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      wpar.baseline_range=CSI_BASELINE_RANGE;
      get_exclusion_zone_for_CsI(d,waveform,&wpar);
      get_shape_with_RF(4,d,waveform,par,&wpar);

      s=par->am[3];
      f=par->am[2];

      h->Fill(f,s);
   
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

 if(argc!=7)
    {
      printf("\n ./wfit_CsI_FS_with_RF midas_input_data_file_name channel trc tf ts trf\n");
      exit(-1);
    }

  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));
  chn=atoi(argv[2]);
  par->t[1]=atof(argv[3]);
  par->t[2]=atof(argv[4]);
  par->t[3]=atof(argv[5]);
  par->rf[0]=atof(argv[6]);

  h = new TH2D("PID F/S","PID F/S",S1K,0,S4K,S1K,0,S4K);
  h->Reset();

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("Program fit chisq spectrum for the selected channel\n");
  theApp=new TApplication("App", &ac, av);
  c = new TCanvas("FS", "FS",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
   
  h->Draw("COLZ");
  h->GetXaxis()->SetTitle("Amp. Fast [arb]");
  h->GetYaxis()->SetTitle("Amp. Slow [arb]");
  theApp->Run(kTRUE);
}
