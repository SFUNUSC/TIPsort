#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{

  int N;
  WaveFormPar wpar;
 

  if(ptr->channel==chn)
    if((N=ptr->waveform_length)!=0)
      {       
	memset(&wpar,0,sizeof(WaveFormPar));
	fit_newT0(N,waveform,&wpar);
	//if(wpar.max>=MAX_VALUE)
	//{
	printf("________________________________________________________\n");
	printf("Trigger  number : %8d\n",ptr->trigger_num&0x0fffffff);

	print_WavePar(&wpar);
	display_newT0_fit(N,waveform,&wpar,theApp);
	//}
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
      printf("wfit_show_newT0Fit midas_input_data_file_name channel\n");
      exit(-1);
    }

  chn=atoi(argv[2]);

 

  theApp=new TApplication("App", &ac, av);
  printf("Program fits fast risetime waveforms for selected channel\n");

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
 
}
