#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  Int_t i,d;
  WaveFormPar wpar;

  
  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
      {
	get_baseline(RANGE,waveform,&wpar);
	if(wpar.bsq>bsqmin)
	  if(wpar.bsq<bsqmax)
	    {
	      printf("*");
	      h->Reset();
	      for(i=0;i<d;i++)
		h->Fill(i,waveform[i]);
	      h->Draw();
	      
	      TF1 *base=new TF1("base","[0]",0,RANGE);
	      base->SetParameter(0,wpar.b);
	      base->SetLineColor(kRed);
	      base->SetLineWidth(2);
	      base->Draw("same");
	      
	      
	      theApp->Run(kTRUE);
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
      printf("\n ./wfit_baseline midas_input_data_file_name channel bsqmin bsqmax\n");
      exit(-1);
    }
  
  
  chn=atoi(argv[2]);
  bsqmin=atof(argv[3]);
  bsqmax=atof(argv[4]);
  h=new TH1D("Waveform","Waveform",MAX_SAMPLES,0,MAX_SAMPLES);
  theApp=new TApplication("App", &ac, av);
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("Program shows linearized baseline fit for selected channel\n");



}
