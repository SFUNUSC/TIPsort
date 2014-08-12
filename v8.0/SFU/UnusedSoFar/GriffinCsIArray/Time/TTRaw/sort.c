#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,col,csi;
  double tgr,tcsi,tdiff;
  
  TotalCount++;
  if((data->csiarray.h.Tfold)+(data->gr.h.Gefold)>=3) HigherFoldCount++;

  if(data->gr.h.Gefold>0)
    if(data->csiarray.h.Tfold>0)
      for(pos=1;pos<NPOSGRIF;pos++)
	if((data->gr.h.GeHP&(1<<(pos-1)))!=0)
	  if(data->gr.det[pos].h.Gefold>0)
	    for(col=0;col<NCOL;col++)
	      if((data->gr.det[pos].h.GeHP&(1<<col))!=0)
		if(data->gr.det[pos].ge[col].h.Tfold>0)
		  if((data->gr.det[pos].ge[col].h.THP&1)!=0)
		    {
		      tgr=data->gr.det[pos].ge[col].seg[0].cfd&0x00ffffff;
		      tgr-=(data->gr.det[pos].ge[col].seg[0].timestamp*16)&0x00ffffff;
		      tgr+=S8K;tgr/=16;
		      
		      for(csi=1;csi<NCSI;csi++)
			if((data->csiarray.h.THP&(one<<csi))!=0)
			  {
			    tcsi=data->csiarray.csi[csi].cfd&0x00ffffff;
			    tcsi-=(data->csiarray.csi[csi].timestamp*16)&0x00ffffff;
			    tcsi+=S8K;tcsi/=16;
		
			    //PJV: h_t1 is plot of time of detector that
			    //fired first. This will have a smaller cfd-ts+SXK time.

			    if(tgr>tcsi) //csi event opened the trigger!
			      {
				tdiff=tgr-tcsi;
				h_t1->Fill(tcsi);
				h_t2->Fill(tgr);
				h_tdiff->Fill(tdiff);
				h->Fill(tcsi,tgr);
			      }
			    
			    else if(tcsi>tgr) //tigress event opened the trigger!
			      {
				tdiff=tcsi-tgr;
				h_t1->Fill(tgr);
				h_t2->Fill(tcsi);
				h_tdiff->Fill(tdiff);
				h->Fill(tgr,tcsi);
			      }
			    
			    else 
			      {
				printf("The times are equal?! t1 and t2 are %f %f.\n",tgr,tcsi);
				//getc(stdin);
			      }}}

  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  char title[132];

  if(argc!=2)
    {
      printf("Griffin_CsIarray_TTRaw sfu_input_file\n");
      exit(-1);
    }
  
  h = new TH2D("Griffin_CsIArray_TTRaw","Griffin_CsIArray_TTRaw",S1K,0,S1K,S1K,0,S1K);
  h->Reset();
  h_t1 = new TH1D("TRaw_one","TRaw_one",S1K,0,S1K-1);
  h_t1->Reset();
  h_t2 = new TH1D("TRaw_two","TRaw_two",S1K,0,S1K-1);
  h_t2->Reset();
  h_tdiff = new TH1D("TRaw Diff","TRaw Diff",S1K,0,S1K-1);
  h_tdiff->Reset();

  printf("Program sorts Griffin/CsIArray TTRaw spectra.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);

  sort(name);

  sprintf(title,"Griffin_CsIArray_TTRaw.root");
  TFile f(title, "recreate");
  h->GetXaxis()->SetTitle("Det 1 TRaw [10ns]");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("Det 2 TRaw [10ns]");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  gStyle->SetPalette(1);
  
  h->Write();
  h_t1->Write();
  h_t2->Write();
  h_tdiff->Write();

  printf("Fold 3 and higher %d and total number of events %d.\n",HigherFoldCount,TotalCount);
}

