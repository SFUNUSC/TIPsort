#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
  int j,hp,det;

  for(det=1;det<NPOSTIGR;det++)
    if(det==pos)
      for(col=0;col<NCOL;col++)
	if((hp=data->tg.det[pos].ge[col].h.EHP)!=0)
	  for(j=0;j<NHP;j++)
	    if((hp&(1<<j))!=0)
	      h[col]->Fill(j);

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  TCanvas *c;
  TApplication *theApp;  
  int ac;
  char* av[10];
  char name[132];
  if(argc!=4)
    {
      printf("\n ./assemble_CloverEHP midas_input_data_file_name map Clover_position\n");
      exit(-1);
    }

 pos=atoi(argv[3]);

 /* initialize histograms */

  for(col=0;col<NCOL;col++)
    {
      h[col] = new TH1D("","",NHP,0,NHP);
      h[col]->Reset();
      sprintf(name,"Pos %2d %s",pos,cs[col]);
      h[col]->SetName(name);
      h[col]->SetTitle(name);
    }

/* do sorting */
  sort_and_assemble(argv[1],argv[2]);
 /* display results */
  printf("\n"); 


       printf("Program sorts energy hit patterns for Tigress detectors \n");

      theApp=new TApplication("App", &ac, av);
      c = new TCanvas("Time Hit Patterns", "Time Hit Patterns",10,10, 2000, 1000);
      c->Divide(2,2);

      for(col=0;col<NCOL;col++)
      	{
      	  c->cd(col+1);
	  gPad->SetLogy();
	  h[col]->SetFillColor(rc[col]);
      	  h[col]->Draw();
	}
      theApp->Run(kTRUE);


  
}
