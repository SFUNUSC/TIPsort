#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{
  int time;
  if(ptr->channel==chn)
    {
      time=ptr->led&0x00ffffff;
      time-=ptr->timestamp&0x00ffffff;
      time+=S1K;
      if(time<0) time=S2K-2;
      if(time>S2K-3) time=S2K-1;
      h->Fill(time);
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
      printf("\n ./view_LEDvsTS midas_input_data_file_name channel\n");
      exit(-1);
    }
  printf("Program provides LED vs Time Stamp timing\n");
  
  chn=atoi(argv[2]);
  h=new TH1D("LEDvsTS","LEDvsTS",S2K,0,S2K);
  h->Reset();
 
  /* do sorting */
  sort_but_not_assemble(argv[1]);
  
 theApp=new TApplication("App", &ac, av);
 h->Draw();
 theApp->Run(kTRUE); 
}
