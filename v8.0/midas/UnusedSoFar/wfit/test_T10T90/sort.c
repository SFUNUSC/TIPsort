#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
  int show=0;
 
  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      memset(&wpar,0,sizeof(WaveFormPar));
      wpar.baseline_range=RANGE;
      f[0]++;
      get_t10t90(ptr->waveform_length,waveform,&wpar);
      if(wpar.t10t90_flag==1)
	{
	  f[1]++;
	  h->Fill(wpar.t90-wpar.t10);
	}
      else
	f[-wpar.t10t90_flag+1]++;
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
      printf("\n ./wfit_test_tmax midas_input_data_file_name channel\n");
      exit(-1);
    }


  chn=atoi(argv[2]);
  h=new TH1D("t90t10","t90t10",S32K,0,S32K);
  h->Reset();
  theApp=new TApplication("App", &ac, av);
  memset(f,0,sizeof(f));

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("Program sorts T10-T90 risetime for good waveforms\n");

  for(int d=0;d<12;d++)
    printf(" %4d %10d\n",d,f[d]);
  
  c = new TCanvas("T10T90", "T10T90",10,10, 700, 500);
  h->Draw();
  theApp->Run(kTRUE);
}
