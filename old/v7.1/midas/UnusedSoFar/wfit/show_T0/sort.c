#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {

      get_t00(ptr->waveform_length,waveform,&wpar); 
      
      if(wpar.t00_flag==1)
	if((wpar.t00<high)&&(wpar.t00>low))
	  {
	    print_fragment_info(ptr,S16K);
	    printf("   Waveform    b: %10.1f\n",wpar.b);
	    printf("   Waveform  max: %10.1f\n",wpar.max);
	    printf("   Waveform tmax: %10.4f\n",wpar.tmax);
	    printf("   Waveform  t00: %10.4f\n",wpar.t00);
	    printf("   Waveform  t05: %10.4f\n",wpar.t05);
	    printf("   Waveform  t10: %10.4f\n",wpar.t10);

	    if(h!=NULL) delete h;
	    h=new TH1D("Waveform","Waveform",ptr->waveform_length,0,ptr->waveform_length);
	    if(c!=NULL) delete c;
	    c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);
	    
	    for(Int_t i=0;i<ptr->waveform_length;i++)
	      h->Fill(i,waveform[i]);
		  
	    h->Draw();
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
      printf("\n ./wfit_show_T0 midas_input_data_file_name channel low high\n");
      exit(-1);
    }

  chn=atoi(argv[2]);
  low=atof(argv[3]);
  high=atof(argv[4]);
  theApp=new TApplication("App", &ac, av);
  printf("Program shows waveforms with the T0 limits for selected channel\n");

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
 
}
