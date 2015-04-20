#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
 
  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      get_exclusion_zone_for_CsI(d,waveform,&wpar); 
      h1->Fill(wpar.temax);
      h2->Fill(wpar.temin);
      h3->Fill(wpar.temax-wpar.temin);
      //printf("temax %d\n",wpar.temax);
      //getc(stdin);
    }
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  char runFile[132];
  
  if(argc!=3)
    {
      printf("wfit_test_CsIExclusionZone midas_input_data channel\n");
      exit(-1);
    }
  
  printf("Program tests exclusion zone for CsI waveforms.\n");
  strcpy(runFile,argv[1]);
  chn=atoi(argv[2]);

  h1=new TH1D("temax","temax",S1K/2,0,S1K/2-1);
  h1->Reset();
  h2=new TH1D("temin","temin",S1K/2,0,S1K/2-1);
  h2->Reset();
  h3=new TH1D("temax-temin","temax-temin",S1K/2,0,S1K/2-1);

  sort_but_not_assemble(runFile);

  theApp=new TApplication("App", &argc, argv);
  c = new TCanvas("temax and temin", "temax and temin",10,10, 700, 1000);
  c->Divide(1,2);
  c->cd(1);
  h1->Draw();
  c->cd(2);
  h2->Draw();
  theApp->Run(kTRUE);

  c1 = new TCanvas("temax and temin", "temax and temin",10,10, 700, 500);
  h3->Draw();
  theApp->Run(kTRUE);

  //sprintf(title,"ExclusionZone.root");
  //TFile f(title, "recreate");
  //h1->Write();
  //h2->Write();
}
