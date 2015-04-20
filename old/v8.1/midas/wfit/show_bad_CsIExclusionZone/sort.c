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

      if(wpar.temin>temin || wpar.temax<temax)
	{
	  print_fragment_info(ptr,S16K);
	  printf("Waveform temin parameter: %d\n",wpar.temin);
	  printf("Waveform temax parameter: %d\n",wpar.temax);
	  
	  show_CsI_exclusion_zone(d,waveform,&wpar,theApp);
	    
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
      printf("wfit_show_bad_CsIExclusionZone midas_input_data_file_name channel temin temax\n");
      exit(-1);
    }

  printf("Program shows bad exclusion zone events for a single channel\n");

  chn=atoi(argv[2]); //channel number
  temin=atof(argv[3]); //threshold temin
  temax=atof(argv[4]); // threshhold temax

  theApp=new TApplication("App", &ac, av);

  /* do sorting */
  sort_but_not_assemble(argv[1]);
  /* display results */
  
}
