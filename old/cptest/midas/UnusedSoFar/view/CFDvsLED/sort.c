#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{
  int time1,time2;
  if(ptr->channel==chn)
    {
      time1=ptr->cfd&0x00ffffff;
      time1-=(ptr->timestamp*16)&0x00ffffff;
      time1*=0.625;
      time1/=10;
      time1+=S1K;
      if(time1<0) time1=S2K-2;
      if(time1>S2K-3) time1=S2K-1;

      time2=ptr->led&0x00ffffff;
      time2-=ptr->timestamp&0x00ffffff;
      time2+=S1K;
      if(time2<0) time2=S2K-2;
      if(time2>S2K-3) time2=S2K-1;
      h->Fill(time1,time2);
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
      printf("\n ./view_CFDvsTS midas_input_data_file_name channel\n");
      exit(-1);
    }
  printf("Program provides statistics of bad CFD events\n");
  
  chn=atoi(argv[2]);
  h=new TH2D("CFDvsTS","CFDvsTS",S2K,0,S2K,S2K,0,S2K);
  h->Reset();
 
  /* do sorting */
  sort_but_not_assemble(argv[1]);
  
 theApp=new TApplication("App", &ac, av);
 gStyle->SetPalette(1);
 h->Draw("COLZ");
 theApp->Run(kTRUE); 
}
