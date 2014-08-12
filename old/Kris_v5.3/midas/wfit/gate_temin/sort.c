#include "sort.h"
/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
 
  Int_t d;
  WaveFormPar wpar;
 
  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
     CsI_get_exclusion_zone(d,waveform,&wpar);
     if(wpar.temin>=low)
       if(wpar.temin<=high)
	 {
	   print_WavePar(&wpar);
	   CsI_show_exclusion_zone(d,waveform,&wpar,theApp);
	 }
    }
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

   int ac;
  char *av[10];

 if(argc!=5)
    {
      printf("\n ./wfit_gate_temin midas_input_data_file_name channel low high\n");
      exit(-1);
    }
 
  chn=atoi(argv[2]);
  low=atoi(argv[3]);
  high=atoi(argv[4]);
 
  theApp=new TApplication("App", &ac, av);

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */

 


}
