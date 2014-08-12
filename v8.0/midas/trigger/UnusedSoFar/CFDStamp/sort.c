#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{

  int tcfd,ts,t;

  if(ptr->channel!=channel) return -1;

  tcfd=ptr->cfd&0x00ffffff;
  ts=(ptr->timestamp*16)&0x00ffffff;
  t=tcfd-ts;
  t/=16;
  t+=S1K;
  //  printf("%8.8x %8.8x %8.8x %d\n",tcfd,ts,t,t);
  
  if(t>=0)
    if(t<S2K)
      hist[t]++;
      
  h->Fill(t);
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  FILE* output;
  TCanvas *c;
  TApplication *theApp;
  int ac;
  char *av[10];

  if(argc!=3)
    {
      printf("\n ./trig_CFDStamp midas_input_data_file_name channel\n");
      exit(-1);
    }
  channel=atoi(argv[2]);
  h=new TH1D("CFDStamp","CFDStamp",S2K,0,S2K);
  h->Reset();
  memset(&hist,0,sizeof(hist));
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("\n"); 
  printf("Program sorts TIGRESS DAQ CFD stmap\n");

  if((output=fopen("channelHP.spn","w"))==NULL)
    {
      printf("\nI can't open file channelHP.spn\n");
      exit(EXIT_FAILURE);
    }
  fwrite(hist,sizeof(hist),1,output);
  fclose(output);

  theApp=new TApplication("App", &ac, av);
  c = new TCanvas("Fold", "Fold",10,10, 700, 500);
  gPad->SetLogy();
  h->GetXaxis()->SetTitle("Channel");
  h->GetYaxis()->SetTitle("Counts");
  h->Draw();
  theApp->Run(kTRUE);

}
