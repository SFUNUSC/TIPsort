#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{  
  if(ptr->collector==collector)
    if(ptr->tig10==tig10)
      if(ptr->charge_flag!=0)
  	h[ptr->port]->Fill(ptr->charge);
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  int port,ch;
  char name[132];
  TCanvas *c;
  TApplication *theApp;
  
  if(argc!=4)
    {
      printf("project_tig10 midas_input_data collector tig10\n");
      exit(-1);
    }

  collector=atoi(argv[2]);
  tig10=atoi(argv[3]);
  
  for(port=0;port<10;port++)
    {
      h[port] = new TH1D("","",S65K,0,S65K-1);
      h[port]->Reset();
      sprintf(name,"Port %1d",port);
      h[port]->SetName(name);
      ch=120*(collector-1)+10*(tig10-1)+port;
      sprintf(name,"Coll.# %2d Tig10# %3d Ch.# %4d",collector,tig10,ch);
      h[port]->SetTitle(name);
    }
  
  sort_but_not_assemble(argv[1]);
  
  printf("Program provides a 1D projection for a selected Tig10 module.\n");
  sprintf(name,"Tig10 #%3d",collector);
  
  theApp=new TApplication("App", &argc, argv);
  c = new TCanvas(name, name,10,10, 2000, 1000);
  c->Divide(5,2);
  
  for(port=0;port<10;port++)
    {
      c->cd(port+1);
      gPad->SetLogy();
      h[port]->Draw();
    }
  theApp->Run(kTRUE);
}
