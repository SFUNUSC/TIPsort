#include "sort.h"

int analyze_data(raw_event *data)
{

  int pos1,col1,pos2,col2;
  double t1,t2;

  if(data->tg.h.Gefold>1)
    for(pos1=1;pos1<NPOSTIGR;pos1++)
      if((data->tg.h.GeHP&(1<<(pos1-1)))!=0)
	if(data->tg.det[pos1].h.Gefold>0)
	  for(col1=0;col1<NCOL;col1++)
	    if((data->tg.det[pos1].h.GeHP&(1<<col1))!=0)
	      if(data->tg.det[pos1].ge[col1].h.Tfold>0)
		if((data->tg.det[pos1].ge[col1].h.THP&1)!=0)
		  {
		    t1=data->tg.det[pos1].ge[col1].seg[0].cfd&0x00ffffff;
		    t1-=(data->tg.det[pos1].ge[col1].seg[0].timestamp*16)&0x00ffffff;
		    t1*=10.;
		    t1/=16.;
		    t1+=S4K;
		    for(pos2=pos1+1;pos2<NPOSTIGR;pos2++)
		      if((data->tg.h.GeHP&(1<<(pos2-1)))!=0)
			if(data->tg.det[pos2].h.Gefold>0)
			  for(col2=0;col2<NCOL;col2++)
			    if((data->tg.det[pos2].h.GeHP&(1<<col2))!=0)
			      if(data->tg.det[pos2].ge[col2].h.Tfold>0)
				if((data->tg.det[pos2].ge[col2].h.THP&1)!=0)
				  {
				    t2=data->tg.det[pos2].ge[col2].seg[0].cfd&0x00ffffff;
				    t2-=(data->tg.det[pos2].ge[col2].seg[0].timestamp*16)&0x00ffffff;
				    t2*=10.;
				    t2/=16.;
				    t2+=S4K;
			    
				    h->Fill(t1,t2);
				    h->Fill(t2,t1);
				  }
		  }
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
 

  if(argc!=2)
    {
      printf("\n ./Tigress_TGeGeRaw sfu_input_data_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts raw time histogram for TIGRESS core coincidences\n");
  h = new TH2D("TGeGe","TGeGe",S2K,0,S2K,S2K,0,S2K);
  h->Reset();


  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);

  sort(name);

  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("TGeGe", "TGeGe",10,10, 500, 500);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

 


}

