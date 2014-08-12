#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
  double toff,ton;
  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      get_risetime(ptr->waveform_length,waveform,&wpar); 
      if(wpar.t90_flag==1)
	if(wpar.t10_flag==1)
	  {
	    toff=(wpar.t90-wpar.t10);
	    ton=ptr->cfd&0x00ffffff;
	    ton-=(ptr->timestamp*16)&0x00ffffff;
	    ton*=0.0625;
	 
	    h->Fill(ton,toff);
	  }
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
      printf("\n ./wfit_compare_times midas_input_data_file_name channel\n");
      exit(-1);
    }


  chn=atoi(argv[2]);

  h=new TH2D("CFD","T10T90",S2K,-S1K,S1K,S2K,0,S2K);
  h->Reset();
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("Program shows waveform maximum for selected channel\n");
  theApp=new TApplication("App", &ac, av);
  c = new TCanvas("times", "times",10,10, 700, 500);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  theApp->Run(kTRUE);
}
