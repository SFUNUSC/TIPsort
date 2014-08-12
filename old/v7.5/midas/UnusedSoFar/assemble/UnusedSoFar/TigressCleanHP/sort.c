#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
  int j,ehp,thp;

  for(pos=1;pos<NPOSTIGR;pos++)
    for(col=0;col<NCOL;col++)
      if((ehp=data->tg.det[pos].ge[col].h.EHP)!=0)
	if((thp=data->tg.det[pos].ge[col].h.THP)!=0)	      
	  for(j=0;j<NHP;j++)
	    if((ehp&(1<<j))!=0)
	      if((thp&(1<<j))!=0)
		if(data->tg.det[pos].ge[col].seg[j].charge>0)
		  if(data->tg.det[pos].ge[col].seg[j].cfd>0)
		    h[pos][col]->Fill(j);


  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  TCanvas *c;
  TApplication *theApp;  
  int ac=0;
  char* av[1];
  char name[132];
  if(argc!=3)
    {
      printf("\n ./assemble_TigressCleanHP midas_input_data_file_name map\n");
      exit(-1);
    }

   for(pos=1;pos<NPOSTIGR;pos++)
    for(col=0;col<NCOL;col++)
      {
	h[pos][col] = new TH1D("","",NHP,0,NHP);
	h[pos][col]->Reset();
	sprintf(name,"Pos %2d %s",pos,cs[col]);
	h[pos][col]->SetName(name);
	h[pos][col]->SetTitle(name);
      }


/* do sorting */
  sort_and_assemble(argv[1],argv[2]);
 /* display results */
  printf("\n"); 


       printf("Program sorts clean hit patterns for Tigress detectors \n");

      theApp=new TApplication("App", &ac, av);
      c = new TCanvas("Clean Hit Patterns", "Clean Hit Patterns",10,10, 2000, 1000);
      c->Divide(16,4);
      
      for(pos=1;pos<NPOSTIGR;pos++)
	for(col=0;col<NCOL;col++)
	  {
	    c->cd(col*(NPOSTIGR-1)+pos);
	    gPad->SetLogy();
	    h[pos][col]->SetFillColor(rc[col]);
	    h[pos][col]->Draw();
	  }
      theApp->Run(kTRUE);



  
}
