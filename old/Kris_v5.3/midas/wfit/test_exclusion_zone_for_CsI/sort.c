#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
 
  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      CsI_get_exclusion_zone(d,waveform,&wpar); 
      h->Fill(wpar.temin);
   
    }
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

  int ac;
  char *av[10];

 if(argc!=3)
    {
      printf("\n ./wfit_test_exclusion_zone_for_CsI midas_input_data_file_name channel\n");
      exit(-1);
    }


  chn=atoi(argv[2]);

  h=new TH1D("bsq","bsq",S2K,0,S2K);
  h->Reset();
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("Program shows baseline fluctuations for selected channel\n");
  theApp=new TApplication("App", &ac, av);
  c = new TCanvas("bsq", "bsq",10,10, 700, 500);
  h->Draw();
  theApp->Run(kTRUE);
}
