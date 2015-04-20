#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
 
  if(ptr->channel==chn)
    {
      tot++;
    if((d=ptr->waveform_length)!=0)
      {
	
	memset(&wpar,0,sizeof(WaveFormPar));
	wpar.baseline_range=RANGE;
	get_sig2noise(d,waveform,&wpar);
	
	if(wpar.sig2noise>SIGN2NOISE)
	  {
	    get_t50(d,waveform,&wpar);
	    if(wpar.t50_flag==1)
	      {
		h->Fill(wpar.t50);
		good++;
	      }
	    else
	      bad++;
	    
	  }
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
      printf("\n ./wfit_T50 midas_input_data_file_name channel\n");
      exit(-1);
    }

 good=0;
 bad=0;
 tot=0;
  chn=atoi(argv[2]);

  h=new TH1D("t50","T50",S2K,0,S16K);
  h->Reset();
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("Program shows T50 for selected channel\n");
  printf("fraction of successful finds is %8.2f \%\n",100.*good/(float)tot);
  printf("fraction of attempts         is %8.2f \%\n",100.*(good+bad)/(float)tot);
  theApp=new TApplication("App", &ac, av);
  c = new TCanvas("t50", "t50",10,10, 700, 500);
  h->Draw();
  theApp->Run(kTRUE);
}
