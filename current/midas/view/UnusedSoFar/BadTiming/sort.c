#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{
 
  if((ptr->cfd&0x00ffffff)>((ptr->timestamp*16)&0x00ffffff))
    h->Fill(ptr->channel,1);
  else
    g->Fill(ptr->channel,1);
  
  s->Fill(ptr->channel,1);

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  
  int ac;
  char* av[10];
  if(argc!=2)
    {
      printf("\n ./view_BadTiming midas_input_data_file_name\n");
      exit(-1);
    }
  printf("Program provides statistics of bad CFD events\n");
  
  h=new TH1D("Bad Events","Bad Events",S1K,0,S1K);
  g=new TH1D("Good Events","Good Events",S1K,0,S1K);
  s=new TH1D("Sum Events","Sum  Events",S1K,0,S1K);
  h->Reset();
  g->Reset();
  s->Reset();
  /* do sorting */
  sort_but_not_assemble(argv[1]);
  
 theApp=new TApplication("App", &ac, av);
 h->Divide(s);
 h->Draw();
 theApp->Run(kTRUE); 
}
