#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{
  int t;

  t=ptr->trigger_num&0x0fffffff;
  if(t==trigNum)
    {
      printf("=========================================================\n");
      printf("Trigger  number : %8d\n",t);
      printf("           Port : %8d\n",ptr->port);
      printf("          Tig10 : %8d\n",ptr->tig10);
      printf("      Collector : %8d\n",ptr->collector);
      printf("        Channel : %8d\n",ptr->channel);
      printf("         Charge : %8d\n",ptr->charge);
      printf("            CFD : %8d\n",ptr->cfd);
      printf("      timestamp : %8.8x\n",ptr->timestamp);
      printf("   timestamp up : %8.8x\n",ptr->timestamp_up);
      printf("Waveform length : %8d\n",ptr->waveform_length);
      printf("=========================================================\n");  
      if(ptr->waveform_length!=0)
	{  
	  if(h!=NULL) delete h;
	  h=new TH1D("Waveform","Waveform",ptr->waveform_length,0,ptr->waveform_length);
	  if(c!=NULL) delete c;
	  c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);
	  for(Int_t i=0;i<ptr->waveform_length;i++)
	    {
	      h->Fill(i,waveform[i]);
	    }
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
  char* av[10];

  if(argc!=3)
    {
      printf("view_EventFragments midas_input_data TriggerNumber\n");
      exit(-1);
    }
  trigNum=atoi(argv[2]);
  
  theApp=new TApplication("App", &ac, av);
  /* do sorting */
  sort_but_not_assemble(argv[1]);
  printf("Program provides information on consecutive fragments.\n");
}
