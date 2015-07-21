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
	g->Fill(wpar.t0);
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
      printf("wfit_show_T0dist midas_input_data_file_name channel\n");
      exit(-1);
    }

  chn=atoi(argv[2]);

  if(g!=NULL) delete g;
  g=new TH1D("t0","t0",S1K,0,S1K-1);
  chn=atoi(argv[2]);

  theApp=new TApplication("App", &ac, av);
  printf("Program sorts distribution of t0 values from waveform fit\n");

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */

  c = new TCanvas("t0", "t0",10,10, 700, 500);
  g->Draw();
  theApp->Run(kTRUE);
 
}
