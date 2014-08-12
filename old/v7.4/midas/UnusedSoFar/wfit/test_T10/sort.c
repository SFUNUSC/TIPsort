#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
 
  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      memset(&wpar,0,sizeof(WaveFormPar));
      wpar.baseline_range=RANGE;
      get_sig2noise(d,waveform,&wpar);
   
      if(wpar.sig2noise>SIGN2NOISE)
      	{
	  get_t10(d,waveform,&wpar);
      	  if(wpar.t10_flag==1)
	    {
	      h->Fill(wpar.t10);
	      good++;
	    }
	  else
	    bad++;
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
      printf("\n ./wfit_T10 midas_input_data_file_name channel\n");
      exit(-1);
    }


  chn=atoi(argv[2]);
  bad=0;
  good=0;

  h=new TH1D("t10","T10",S2K,0,S16K);
  h->Reset();
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("Program shows T10 for selected channel\n");
  printf("Number of successful finds is %6.1f\n",(double)good/(double)(bad+good));
  theApp=new TApplication("App", &ac, av);
  c = new TCanvas("t10", "t10",10,10, 700, 500);
  h->Draw();
  theApp->Run(kTRUE);
}
