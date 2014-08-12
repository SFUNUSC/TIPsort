#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{

  if(ptr->channel>=0)
    if(ptr->channel<S2K)
      hist[ptr->channel]++;

  h->Fill(ptr->channel);
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

  if(argc!=2)
    {
      printf("\n ./trig_channelHP midas_input_data_file_name\n");
      exit(-1);
    }
  h=new TH1D("Fold","Fold",S2K,0,S2K);
  h->Reset();
  memset(&hist,0,sizeof(hist));
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("\n"); 
  printf("Program sorts TIGRESS DAQ channel hit patter\n");

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
