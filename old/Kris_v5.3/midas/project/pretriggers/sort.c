#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{
  int ptp,i;
 
  if (ptr->collector==collector)
    {
      ptp=(ptr->pattern&0x0007F000)>>12;
      
  //  printf(" %8.8x   %8.8x\n",ptr->pattern,ptp);
  //  getc(stdin);
      h->Fill(ptp);
      for(i=0;i<8;i++)
	if((ptp&(1<<i))!=0)
	  g->Fill(i);
    }

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  TCanvas *c;
  TApplication *theApp;
  int ac;
  char *av[10];
  

  if(argc!=3)
    {
      printf("\n ./project_pretriggers midas_input_data_file_name collector\n");
      exit(-1);
    }

  collector=atoi(argv[2]);
  h=new TH1D("PreTrig Pattern","PreTrig Pattern",S1K,0,S1K);

  g=new TH1D("PreTrig Pattern","PreTrig Pattern",8,0,8);
 
  h->Reset();
  g->Reset();

/* do sorting */
  sort_but_not_assemble(argv[1]);

 /* display results */
  printf("\n"); 
  printf("Program sort a pattern of Pre Triggers in the data stream\n");

  theApp=new TApplication("App", &ac, av);
  c = new TCanvas(" ", " ",10,10, 700, 500);
  /* h->GetXaxis()->SetTitle("PreTrig Pattern"); */
  /* h->GetYaxis()->SetTitle("Counts"); */
  /* h->Draw(); */
  /* theApp->Run(kTRUE); */

  g->GetXaxis()->SetTitle("PreTrig Pattern");
  g->GetYaxis()->SetTitle("Counts");
  g->Draw();
  theApp->Run(kTRUE);
 


}
