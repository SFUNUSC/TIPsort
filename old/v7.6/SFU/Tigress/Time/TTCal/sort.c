#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos1,pos2,col1,col2;
  double t1,t2,tdiff;
  //int take=0;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  if(cev->tg.h.FT>0)
    for(pos1=1;pos1<NPOSTIGR;pos1++)
      if((cev->tg.h.THP&(1<<(pos1-1)))!=0)
	if(cev->tg.det[pos1].hge.FT>0)
	  for(col1=0;col1<NCOL;col1++)
	    if((cev->tg.det[pos1].hge.THP&(1<<col1))!=0)
	      if(cev->tg.det[pos1].ge[col1].h.FT>0)
		if((cev->tg.det[pos1].ge[col1].h.THP&1)!=0)
		  {
		    t1=cev->tg.det[pos1].ge[col1].seg[0].T/cal_par->tg.contr_t;
		    for(pos2=pos1+1;pos2<NPOSTIGR;pos2++)
		      if((cev->tg.h.THP&(1<<(pos2-1)))!=0)
			if(cev->tg.det[pos2].hge.FT>0)
			  for(col2=0;col2<NCOL;col2++)
			    if((cev->tg.det[pos2].hge.THP&(1<<col2))!=0)
			      if(cev->tg.det[pos2].ge[col2].h.FT>0)
				if((cev->tg.det[pos2].ge[col2].h.THP&1)!=0)
				  {
				    t2=cev->tg.det[pos2].ge[col2].seg[0].T/cal_par->tg.contr_t;
				    
				    tdiff=t1-t2;
				    h_t1->Fill(t2);
				    h_t2->Fill(t1);
				    h_tdiff->Fill(tdiff);
				    //Symmetrized
				    h->Fill(t2,t1);
				    h->Fill(t1,t2);
				  }}
  free(cev);
  return SEPARATOR_DISCARD;
}
/*==========================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  char title[132];
  
  if(argc!=2)
    {
      printf("TIGRESS_TTCal master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("Tigress TTCal","Tigress TTCal",S2K,0,S4K-1,S2K,0,S4K-1);
  h->Reset();
  h_t1 = new TH1D("TCal1","TCal1",S16K,-S8K,S8K);
  h_t1->Reset();
  h_t2 = new TH1D("TCal2","TCal2",S16K,-S8K,S8K);
  h_t2->Reset();
  h_tdiff = new TH1D("TCal Diff","TCal Diff",S16K,-S8K,S8K);
  h_tdiff->Reset();
  
  printf("Program sorts TTCal histogram for addback TIGRESS cores.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);
  
  if(name->flag.inp_data!=1)
    {
      printf("ERROR! Input data file not defined.\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calibration read from: %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR! TIGRESS calibration parameters not defined.\n");
      exit(EXIT_FAILURE);
    }
  
  sort(name);
  
  sprintf(title,"Tigress_TTCal.root");
  TFile f(title, "recreate");
  h->GetXaxis()->SetTitle("Tigress TCal [ns]");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("CsI TCal [ns]");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  gStyle->SetPalette(1);

  h_tdiff->GetXaxis()->SetTitle("TIGRESS TCal1 - TIGRESS TCal2 [ns]");
  h_tdiff->GetXaxis()->CenterTitle(true);
  h_tdiff->GetXaxis()->SetTitleOffset(1.5);

  h->Write();
  h_t1->Write();
  h_t2->Write();
  h_tdiff->Write();
}
