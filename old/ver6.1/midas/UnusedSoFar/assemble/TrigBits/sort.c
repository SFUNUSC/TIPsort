#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
    h->Fill(data->h.setupHP);
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  TCanvas *c;
  TApplication *theApp;  
  if(argc!=3)
    {
      printf("\n ./trig_TrigBits midas_input_data_file_name map_file_name\n");
      exit(-1);
    }
  h = new TH1D("TrigBits","TrigBits",257,-1,256);
  h->Reset();

/* do sorting */
  sort_and_assemble(argv[1],argv[2]);
 /* display results */
  printf("\n"); 


  printf("Program sorts trigger bits histogram \n");
  
  theApp=new TApplication("App", &argc, argv);
  c = new TCanvas("TrigBits", "TrigBits",10,10, 500, 300);
  gPad->SetLogy(1);
  h->Draw();
  theApp->Run(kTRUE);

  
}
