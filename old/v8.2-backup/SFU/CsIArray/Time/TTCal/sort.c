#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos1,pos2;//take=0;
  double t1,t2,tdiff;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  
  if(cev->csiarray.h.FT>0)
    for(pos1=1;pos1<NCSI;pos1++)
      if((cev->csiarray.h.THP&(one<<pos1))!=0)
	//if(((cev->csiarray.h.THP&(one<<pos1))!=0) && (take!=1))
	for(pos2=pos1+1;pos2<NCSI;pos2++)
	  if((cev->csiarray.h.THP&(one<<pos2))!=0)
	    //if(((cev->csiarray.h.THP&(one<<pos2))!=0) && (take!=1))
	    {
	      //take=1;
	      
	      //In CFD units, then scaled by contraction
	      t1=cev->csiarray.csi[pos1].T/cal_par->csiarray.contr_t;
	      t2=cev->csiarray.csi[pos2].T/cal_par->csiarray.contr_t;
	      //printf("ti %f t2 %f\n",t1,t2);
	      //getc(stdin);
	      //In ns
	      //t1*=0.625;
	      //t2*=0.625;

	      tdiff=t1-t2;
	      h_t1->Fill(t2);
	      h_t2->Fill(t1);	      
	      h_tdiff->Fill(tdiff);
	      //Symmetrized
	      h->Fill(t2,t1);
	      h->Fill(t1,t2);
	    }
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*=====================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  char title[132];

  if(argc!=2)
    {
      printf("CsIArray_TTCal master_file\n");
      exit(-1);
    }
  
  h = new TH2D("CsI TTCal","CsI TTCal",S8K,0,S16K-1,S8K,0,S16K-1);
  h->Reset();
  h_t1 = new TH1D("TCal1","TCal1",S16K,0,S16K-1);
  h_t1->Reset();
  h_t2 = new TH1D("TCal2","TCal2",S16K,0,S16K-1);
  h_t2->Reset();
  h_tdiff = new TH1D("TCal Diff","TCal Diff",S16K,-S8K,S8K);
  h_tdiff->Reset();
  
  printf("Program sorts TTCal histograms for the CSIARRAY.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);
  
  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.CSIARRAY_cal_par==1)
    {
      printf("CsIArray calpar read from: %s\n",name->fname.CSIARRAY_cal_par);
      initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
    }
  else
    {
      printf("ERROR! CsIArray calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  sort(name);
    
  sprintf(title,"CsIArray_TTCal.root");
  TFile f(title, "recreate");
  h->GetXaxis()->SetTitle("CsI TCal1 [ns]");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("CsI TCal2 [ns]");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  gStyle->SetPalette(1);

 h_tdiff->GetXaxis()->SetTitle("CsI TCal1 - CsI TCal2 [ns]");
  h_tdiff->GetXaxis()->CenterTitle(true);
  h_tdiff->GetXaxis()->SetTitleOffset(1.5);
  
  h->Write();
  h_t1->Write();
  h_t2->Write();
  h_tdiff->Write();
}
