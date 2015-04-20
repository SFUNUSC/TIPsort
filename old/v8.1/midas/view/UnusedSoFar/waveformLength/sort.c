#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{
  int len;
  
  if(ptr->channel==channel)
    {
      len=ptr->waveform_length;
      h->Fill(len);
      if((len>=0) && (len<S16K)) hist[len]++;
      else hist[S16K-1000]++;
    }
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  int ac;
  char *av[10];

  FILE* output;
  TCanvas *c;
  TApplication *theApp;
  
  if(argc!=3)
    {
      printf("view_waveformLength midas_input_data channel\n");
      exit(-1);
    }
  channel=atoi(argv[2]);

  h=new TH1D("Waveform length","Waveform length",S16K,0,S16K);
  h->Reset();
  memset(&hist,0,sizeof(hist));

  printf("Program sorts TIGRESS DAQ channel waveform lengths\n");
  sort_but_not_assemble(argv[1]);
 
  if((output=fopen("waveformLength.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(hist,sizeof(hist),1,output);
  fclose(output);
  
  theApp=new TApplication("App", &ac, av);
  c = new TCanvas("Waveform length", "Waveform length",10,10, 700, 500);
  gPad->SetLogy();
  h->GetXaxis()->SetTitle("Waveform Length");
  h->GetYaxis()->SetTitle("Counts");
  h->Draw();
  theApp->Run(kTRUE);
}
