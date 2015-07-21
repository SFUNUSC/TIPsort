#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos1,pos2,take=0;
  double t1,t2,tdiff;
  
  countTot++;
  if(data->csiarray.h.Tfold>=3)
    {
      count++;
    }
  if(data->csiarray.h.Tfold==2)
    for(pos1=1;pos1<NCSI;pos1++)
      if(((data->csiarray.h.THP&(one<<pos1))!=0) && (take!=1))
	for(pos2=pos1+1;pos2<NCSI;pos2++)
	  if((data->csiarray.h.THP&(one<<pos2))!=0)
	    {
	      take=1;
	      t1=data->csiarray.csi[pos1].cfd&0x00ffffff;
	      t1-=(data->csiarray.csi[pos1].timestamp*16)&0x00ffffff;
	      t2=data->csiarray.csi[pos2].cfd&0x00ffffff;
	      t2-=(data->csiarray.csi[pos2].timestamp*16)&0x00ffffff;
	      t1+=S8K;t1/=16;
	      t2+=S8K;t2/=16;

	      /* if(((data->h.setupHP&RF_BIT)!=0)) */
	      /* 	{ */
	      /* 	  trf=data->rf.sin.t0; */
	      /* 	  t1-=trf; */
	      /* 	  t2-=trf; */
	      
	      //PJV: h_t1 is plot of time of detector that
	      //fired first. This will have a smaller cfd-ts+SXK time.

	      if(t1>t2)
		{
		  tdiff=t1-t2;
		  h_t1->Fill(t2);
		  h_t2->Fill(t1);
		  h_tdiff->Fill(tdiff);
		  h->Fill(t2,t1);
		}
	      else if(t2>t1)
		{
		  tdiff=t2-t1;
		  h_t1->Fill(t1);
		  h_t2->Fill(t2);
		  h_tdiff->Fill(tdiff);
		  h->Fill(t1,t2);
		}
	      else 
		{
		  printf("The times are equal?! t1 and t2 are %f %f.\n",t1,t2);
		  //getc(stdin);
		}
	      /*  } */
	      /* else printf("No RF bit was set!\n"); */
	    } 
  return SEPARATOR_DISCARD;
}
/*=====================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  char title[132];
  int cont=0,size;

  if(argc!=3)
    {
      printf("CsIArray_TTRaw sfu_input_file contraction_factor\n");
      exit(-1);
    }
  
  cont=atoi(argv[2]);
  size=cont*(S1K/16);

  h = new TH2D("TTRaw","TTRaw",size,0,size-1,size,0,size-1);
  h->Reset();
  h_t1 = new TH1D("TRaw_one","TRaw_one",size,0,size-1);
  h_t1->Reset();
  h_t2 = new TH1D("TRaw_two","TRaw_two",size,0,size-1);
  h_t2->Reset();
  h_tdiff = new TH1D("TRaw Diff","TRaw Diff",size,0,size-1);
  h_tdiff->Reset();
  
  printf("Program sorts TTRaw Spectra for CsIArray.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
 
  sort(name);
  
  sprintf(title,"CsIArray_TTRaw.root");
  TFile f(title, "recreate");
  h->GetXaxis()->SetTitle("CsIArray Det 1 TRaw [10ns]");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("CsIArray Det 2 TRaw [10ns]");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  gStyle->SetPalette(1);
  
  h->Write();
  h_t1->Write();
  h_t2->Write();
  h_tdiff->Write();

  printf("Fold 3 and higher %d and total number of events %d.\n",count,countTot);

}
