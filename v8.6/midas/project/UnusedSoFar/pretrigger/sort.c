#include "sort.h"

int analyze_fragment(Tig10_event* ptr, short* waveform)
{
  int ptp;
  ptp=(ptr->pattern&0x0007F000)>>12;
  h->Fill(ptp);

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  TCanvas *c;
  TApplication *theApp;
  int ac;
  char *av[10];
  char name[132];

  if(argc!=2)
    {
      printf("project_pretrigger midas_input_data\n");
      exit(-1);
    }

  h=new TH1D("PreTrig Pattern","PreTrig Pattern",S1K/2,0,S1K/2);
  h->Reset();
 
  sort_but_not_assemble(argv[1]);

  printf("Program sort a pattern of Pre Triggers in the data stream.\n");

  theApp=new TApplication("App", &ac, av);
  c = new TCanvas(name, name,10,10, 700, 500);
  h->GetXaxis()->SetTitle("PreTrig Pattern");
  h->GetYaxis()->SetTitle("Counts");
  h->Draw();
  theApp->Run(kTRUE);
}
