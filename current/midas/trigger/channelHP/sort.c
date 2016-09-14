#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{
  /* if((ptr->channel<0) || (ptr->channel>S2K)) */
  /*   { */
  /*     printf("Bad channel values present! Investigate!!!\n"); */
  /*     printf("Offending channel is %d\n",ptr->channel); */
  /*     exit(EXIT_FAILURE); */
  /*   } */
  /* else */
  /*   {   */
  /*     hist[ptr->channel]++; */
  /*     h->Fill(ptr->channel); */
  /*   } */

  //ignoring bad channels!
  if((ptr->channel>=0) && (ptr->channel<=S2K))
    {
      hist[ptr->channel]++;
      h->Fill(ptr->channel);
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
  
  if(argc!=2)
    {
      printf("trig_channelHP midas_data_file\n");
      exit(-1);
    }
  printf("Program sorts DAQ channel hit pattern.\n");

  h=new TH1D("Fold","Fold",S2K,0,S2K);
  h->Reset();
  memset(&hist,0,sizeof(hist));
  
  sort_but_not_assemble(argv[1]);
  
  if((output=fopen("channelHP.spn","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the spn file.\n");
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
