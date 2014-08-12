#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{
  int timeCFD,timeLED;
  if(ptr->channel==chn)
    {
      timeCFD=ptr->cfd&0x00ffffff;
      timeCFD-=(ptr->timestamp*16)&0x00ffffff;
      timeCFD/=16;
      timeCFD+=S1K;
      if(timeCFD<0) timeCFD=S2K-2;
      if(timeCFD>S2K-3) timeCFD=S2K-1;

      timeLED=ptr->led&0x00ffffff;
      timeLED-=(ptr->timestamp*16)&0x00ffffff;
      timeLED/=16;
      timeLED+=S1K;
      if(timeLED<0) timeLED=S2K-2;
      if(timeLED>S2K-3) timeLED=S2K-1;

      h->Fill(timeCFD,timeLED);
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
      printf("view_CFDvsTS midas_input_data channel\n");
      exit(-1);
    }
  printf("Program plots CFD and LED for events.\n");
  
  chn=atoi(argv[2]);
  h=new TH2D("CFD(X)vsLED(Y)","CFD(X)vsLED(Y)",S2K,0,S2K,S2K,0,S2K);
  h->Reset();
  
  /* do sorting */
  sort_but_not_assemble(argv[1]);
  
 theApp=new TApplication("App", &ac, av);
 gStyle->SetPalette(1);
 h->Draw("COLZ");
 theApp->Run(kTRUE); 
}
