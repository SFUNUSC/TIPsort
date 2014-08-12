#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
  if((data->h.setupHP&RF_BIT)!=0)
    {
      A->Fill(data->rf.sin.A);
      C->Fill(data->rf.sin.C);
      t0->Fill(data->rf.sin.t0);
    }
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  int ac=0;
  char *av[1];
  if(argc!=3)
    {
      printf("\n ./RF_verify midas_input_data_file_name map_name\n");
      exit(-1);
    }
 
  A=new TH1D("Amplitude","Amplitude",4096,0,4096);
  C=new TH1D("Offset","Offset",32768,0,32768);
  t0=new TH1D("t0","t0",150,-10,140);
 

  /* do sorting */
  sort_and_assemble(argv[1],argv[2]);
 /* display results */
  printf("\n"); 
  printf("Program displays results of the RF fit\n");

  theApp=new TApplication("App", &ac, av);
  c = new TCanvas("RF fit", "RF fit",0,0, 700, 600);
  c->Divide(1,3);
  c->cd(1);
  gPad->SetLogy();
  t0->Draw();
  c->cd(2);
  gPad->SetLogy();
  A->Draw();
  c->cd(3);
  gPad->SetLogy();
  C->Draw();
  theApp->Run(kTRUE);
  return 0;
}
