#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  WaveFormPar wpar;
  
  wpar.baseline_range=CSI_BASELINE_RANGE;

  if(ptr->channel==chn)
    if(ptr->waveform_length!=0)
      {
	get_tmax(ptr->waveform_length,waveform,&wpar);
	get_baseline(CSI_BASELINE_RANGE,waveform,&wpar);
	h->Fill(wpar.max-wpar.b);
      }
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  if(argc!=3)
    {
      printf("wfit_amplitude midas_input_data channel\n");
      exit(-1);
    }
  
  printf("Program plots amplitude tmax-baseline for waveforms.\n");
  chn=atoi(argv[2]);
  
  h=new TH1D("Amplitude","Amplitude",S32K,0,S32K);
  h->Reset();
  
  sort_but_not_assemble(argv[1]);

  theApp=new TApplication("App", &argc, argv);
  h->Draw();
  theApp->Run(kTRUE);
}
