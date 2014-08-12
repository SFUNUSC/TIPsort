#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{
  
  
  h->Fill(ptr->collector);
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
 
  char name[132];
  TCanvas *c;
  TApplication *theApp;
 
  
  
  if(argc!=2)
    {
      printf("\n ./project_collectorHP midas_input_data_file_name\n");
      exit(-1);
    }


  
  
  /* initialize histograms*/
 
  h = new TH1D("collectorHP","collectorHP",16,0,16);
  h->Reset();

/* do sorting */
  sort_but_not_assemble(argv[1]);

 /* display results */
  printf("\n"); 
  printf("Program sorts collector hit patterns\n");
  
  theApp=new TApplication("App", &argc, argv);
  c = new TCanvas(name, name,10,10, 700, 500);
  
  gPad->SetLogy();
  h->Draw();
    
  theApp->Run(kTRUE);



}
