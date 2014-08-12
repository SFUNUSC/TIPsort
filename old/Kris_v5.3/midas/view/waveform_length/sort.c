#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{

  int len;

  if(ptr->channel==channel)
    {
      len=ptr->waveform_length;
      h->Fill(len);
    if(len>=0)
      if(len<S16K)
	hist[len]++;
    }
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
      printf("\n ./view_waveform_length midas_input_data_file_name channel\n");
      exit(-1);
    }
  channel=atoi(argv[2]);
  h=new TH1D("Waveform length","Waveform length",S16K,0,S16K);
  h->Reset();
  memset(&hist,0,sizeof(hist));
/* do sorting */
  sort_but_not_assemble(argv[1]);
 
 /* display results */
  printf("\n"); 
  printf("Program sorts TIGRESS DAQ channel waveform lengths\n");

  if((output=fopen("waveform_length.mca","w"))==NULL)
    {
      printf("\nI can't open file waveform_length.mca\n");
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
