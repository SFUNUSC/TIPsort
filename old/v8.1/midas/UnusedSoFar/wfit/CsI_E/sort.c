#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
  double s,f,e;

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      wpar.baseline_range=CSI_BASELINE_RANGE;
      get_exclusion_zone_for_CsI(d,waveform,&wpar);
      get_shape(4,d,waveform,par,&wpar);

      s=par->am[3];
      f=par->am[2];
      e=f+alpha*s;
      h->Fill(e,s);
      g->Fill(e);
      if(e<0)e=S32K-2;
      if(e>S32K-4) e=S32K-4;
      hist[(int)rint(e)]++;

    }
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

  int ac;
  char *av[10];
  TCanvas *c,*c1;  
  TApplication *theApp;
  FILE * output;
 if(argc!=7)
    {
      printf("\n ./wfit_CsI_E midas_input_data_file_name channel trc tf ts  alpha\n");
      exit(-1);
    }

  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));
  chn=atoi(argv[2]);
  par->t[1]=atof(argv[3]);
  par->t[2]=atof(argv[4]);
  par->t[3]=atof(argv[5]);
  alpha=atof(argv[6]);
  h = new TH2D("E/S","E/S",S1K,0,S4K,S1K,0,S4K);
  h->Reset();
  memset(hist,0,sizeof(hist));
  g = new TH1D("E","E",S1K,0,S4K);
  g->Reset();

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("Program fit energy spectrum for the selected channel\n");


  if((output=fopen("wfit_CSI_E.mca","w"))==NULL)
    {
      printf("\nI can't open file %s\n","wfit_CSI_E.mca");
      exit(EXIT_FAILURE);
    }
  fwrite(hist,S32K*sizeof(int),1,output);
  fclose(output);

  theApp=new TApplication("App", &ac, av);
  c = new TCanvas("E/S", "E/S",10,10, 500, 300);

  gStyle->SetPalette(1);
  h->GetXaxis()->SetTitle("Energy [arb]");
  h->GetYaxis()->SetTitle("Amp. Slow [arb]");   
  h->Draw("COLZ");

  c1 = new TCanvas("E", "E",10,10, 500, 300);
  g->GetXaxis()->SetTitle("Energy [arb]");
  g->Draw();
  theApp->Run(kTRUE);
}
