#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
 
  Int_t i,j,d,su,sl,delta=8,Delta=2*delta;

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
    
      if(h!=NULL) delete h;		
      h=new TH1D("Waveform","Waveform",ptr->waveform_length,0,ptr->waveform_length);
      if(g!=NULL) delete g;
      g=new TH1D("FF","FF",ptr->waveform_length,0,ptr->waveform_length);
      if(c!=NULL) delete c;
      c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);
      for(i=0;i<ptr->waveform_length;i++)
	{
	  h->Fill(i,waveform[i]);
	}
      for(i=0;i<ptr->waveform_length-Delta-2*delta;i++)
	{
	  sl=0;
	  for(j=i;j<i+delta;j++)
	    sl+=waveform[j];
	  su=0;
	  for(j=i+Delta+delta;j<i+Delta+2*delta;j++)
	    su+=waveform[j];
	  g->Fill(i,(su-sl)/delta+4000);
	}
      h->Draw();
      g->SetLineColor(kRed);
      g->Draw("same");
      theApp->Run(kTRUE);	
    }

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

 int ac;
 char* av[10];

 if(argc!=3)
    {
      printf("\n ./wfit_show_ff midas_input_data_file_name channel\n");
      exit(-1);
    }
  chn=atoi(argv[2]);
 theApp=new TApplication("App", &ac, av);
 

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */

 


}
