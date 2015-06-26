#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
 
  Int_t d;
  WaveFormPar wpar;

  
  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      memset(&wpar,0,sizeof(WaveFormPar));  

      if(c!=NULL) delete c;
      c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);

      if(h!=NULL) delete h;		
      h=new TH1D("Waveform","Waveform",ptr->waveform_length,0,ptr->waveform_length);

      for(Int_t i=0;i<ptr->waveform_length;i++)
	h->Fill(i,waveform[i]);

      h->Draw();
      get_Griffin_t0(d,waveform,&wpar);
     
      print_fragment_info(ptr,S16K);
      printf("   Waveform    d: %10d\n",d);
      printf("   Waveform    b: %10.1f\n",wpar.b);
      printf("   Waveform  max: %10.1f\n",wpar.max);
      printf("   Waveform tmax: %10.4f\n",wpar.tmax);
      printf("  Waveform temin: %10d\n",wpar.temin);
      printf("  Waveform temax: %10d\n",wpar.temax);
      printf("  Waveform thigh: %10.4f\n",wpar.thigh);
      printf("   Waveform   b0: %10.4f\n",wpar.b0);
      printf("   Waveform   b1: %10.4f\n",wpar.b1);
      printf("   Waveform   s0: %10.4f\n",wpar.s0);
      printf("   Waveform   s1: %10.4f\n",wpar.s1);
      printf("   Waveform   s2: %10.4f\n",wpar.s2);
      printf("   Waveform   t0: %10.4f\n",wpar.t00);
	  if(wpar.t00>0)
	    {
	  if(g!=NULL) delete g;
	  g=new TH1D("Fit","Fit",ptr->waveform_length,0,ptr->waveform_length);

	  if(f!=NULL) delete f;
	  f=new TH1D("Fits","Fits",ptr->waveform_length,0,ptr->waveform_length);

	  
	  for(Int_t i=0;i<=(int)(wpar.t00+2);i++)
	    g->Fill(i,wpar.b0+wpar.b1*i);

	  for(Int_t i=(int)(wpar.t00-2);i<=wpar.thigh;i++)
	    f->Fill(i,wpar.s0+wpar.s1*i+wpar.s2*i*i);
  
	  g->SetLineColor(kRed);
	  g->Draw("same");
	  f->SetLineColor(kBlue);
	  f->Draw("same");
	}
      theApp->Run(kTRUE);

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
      printf("\n ./wfit_Griffin_t0 midas_input_data_file_name channel");
      exit(-1);
    }
 
  chn=atoi(argv[2]);
  
  theApp=new TApplication("App", &ac, av);

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */

 


}