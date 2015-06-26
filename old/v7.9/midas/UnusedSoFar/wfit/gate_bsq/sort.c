#include "sort.h"
/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
 
  Int_t d;
  WaveFormPar wpar;
 
  wpar.baseline_range=CSI_BASELINE_RANGE;
  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
     get_baseline(wpar.baseline_range,waveform,&wpar);
     if(wpar.bsq>=low)
       if(wpar.bsq<=high)
	 {
	   if(h!=NULL) delete h;		
	   h=new TH1D("Waveform","Waveform",ptr->waveform_length,0,ptr->waveform_length);
	   if(c!=NULL) delete c;
	   c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);


	   for(Int_t i=0;i<ptr->waveform_length;i++)
	     h->Fill(i,waveform[i]);
	   h->Draw();
	   printf("\n===============================================\n");
	   printf(" b    : %10.2f\n",wpar.b);
	   printf(" bsq  : %10.2f\n",wpar.bsq);

	 
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
      printf("\n ./wfit_gate_bsq midas_input_data_file_name channel low high\n");
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