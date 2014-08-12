#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  
  WaveFormPar wpar;

  
  if(ptr->channel==chn)
    if(ptr->waveform_length!=0)
      {
	get_tmax(ptr->waveform_length,waveform,&wpar);
	get_baseline(RANGE,waveform,&wpar);
	h->Fill(wpar.max-wpar.b);
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
      printf("\n ./wfit_amplitude  midas_input_data_file_name channel\n");
      exit(-1);
    }
  
  
  chn=atoi(argv[2]);
 
  h=new TH1D("Amp.","Amp.",S32K,0,S32K);
  
  h->Reset();
 
  theApp=new TApplication("App", &ac, av);
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("Program shows linearized baseline fit for selected channel\n");

  h->Draw();
  theApp->Run(kTRUE);


}
