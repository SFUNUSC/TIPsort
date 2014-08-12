#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{

  int N;
  WaveFormPar wpar;
  double chi;

  if(ptr->channel==chn)
    if((N=ptr->waveform_length)!=0)
      {       
	memset(&wpar,0,sizeof(WaveFormPar));
	if((chi=fit_newT0(N,waveform,&wpar))>min)
	  if(chi<max)
	    {
	      print_WavePar(&wpar);
	      printf("chisq/NDF %10.3f\n",chi);
	      display_newT0_fit(N,waveform,&wpar,theApp);
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
      printf("\n ./wfit_show_chi_newT0 midas_input_data_file_name channel chisq_min chisq_max\n");
      exit(-1);
    }

  chn=atoi(argv[2]);
  min=atof(argv[3]);
  max=atof(argv[4]);
  theApp=new TApplication("App", &ac, av);
  printf("Program fits fast risetime waveforms for selected channel\n");

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */

 
 
}
