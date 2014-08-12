#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
  
  h->Fill(data->h.frag_fold);
 
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
      printf("\n ./trig_frag_fold midas_input_data_file_name map_file_name\n");
      exit(-1);
    }
  h=new TH1D("Fold","Fold",256,0,256);
  h->Reset();
/* do sorting */
  sort_and_assemble(argv[1],argv[2]);

 /* display results */
  printf("\n"); 
  printf("Program displays a histogram of fragment fold associated with a given trigger\n");
  theApp=new TApplication("App", &ac, av);
  c = new TCanvas("Fold", "Fold",10,10, 700, 500);
  gPad->SetLogy();
  h->GetXaxis()->SetTitle("Fragment fold");
  h->GetYaxis()->SetTitle("Counts");
  h->Draw();
  theApp->Run(kTRUE);


}
