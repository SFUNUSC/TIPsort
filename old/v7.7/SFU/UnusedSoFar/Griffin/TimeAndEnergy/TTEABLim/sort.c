#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos1,pos2,col1,col2,take=0;
  double e1,t1,e2,t2,tdiff;
  long long int one=1;

  /*temporary, PJV*/
#  define NPOSGRIF 3
  /*temporary, PJV*/

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_GRIFFIN(data,&cal_par->gr,&cev->gr);

  if(cev->gr.h.FH>0)
    for(pos1=1;pos1<NPOSGRIF;pos1++)
      if(((cev->gr.h.HHP&(one<<(pos1-1)))!=0) && (take==0))
  	if(cev->gr.det[pos1].hge.FH>0)
	  if((cev->gr.h.AHP&(one<<(pos1-1)))!=0)
	    {
	      e1 = cev->gr.det[pos1].addback.E;
	      col1 = cev->gr.det[pos1].addbackC;
	      t1=cev->gr.det[pos1].ge[col1].seg[0].T/cal_par->gr.contr_t;
	      
	      for(pos2=pos1+1;pos2<NPOSGRIF;pos2++)
		if((cev->gr.h.HHP&(one<<(pos2-1)))!=0)
		  if(cev->gr.det[pos2].hge.FH>0)
		    if((cev->gr.h.AHP&(one<<(pos2-1)))!=0)
		      {
			take=1;
			e2 = cev->gr.det[pos2].addback.E;
			col2 = cev->gr.det[pos2].addbackC;
			t2=cev->gr.det[pos2].ge[col2].seg[0].T/cal_par->gr.contr_t;
			
			if(e1>cal_par->gr.celow[pos1][col1] && e1<cal_par->gr.cehigh[pos1][col1])
			  if(e2>cal_par->gr.celow[pos2][col2] && e2<cal_par->gr.cehigh[pos2][col2])
			    {
			      if(t1>t2)
				{
				  tdiff=t1-t2;
				  h_t1->Fill(t2);
				  h_e1->Fill(e2);
				  h_t2->Fill(t1);
				  h_e2->Fill(e1);
				  h_tdiff->Fill(tdiff);
				  h->Fill(t2,t1);
				}
			      else if(t2>t1)
				{
				  tdiff=t2-t1;
				  h_t1->Fill(t1);
				  h_e1->Fill(e1);
				  h_t2->Fill(t2);
				  h_e2->Fill(e2);
				  h_tdiff->Fill(tdiff);
				  h->Fill(t1,t2);
				}
			      else 
				{
				  printf("The times are equal?! t1 and t2 are %f %f.\n",t1,t2);
				  //getc(stdin);
				}}}}
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  char title[132];

  if(argc!=2)
    {
      printf("Griffin_TTEABLim master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("TT","TT",S1K/2,S1K/2,S1K-1,S1K/2,S1K/2,S1K-1);
  h->Reset();
  
  h_t1 = new TH1D("t1","t1",S1K/2,S1K/2,S1K-1);
  h_t1->Reset();
  h_e1 = new TH1D("e1","e1",S1K,0,S1K-1);
  h_e1->Reset();

  h_t2 = new TH1D("t2","t2",S1K/2,S1K/2,S1K-1);
  h_t2->Reset();
  h_e2 = new TH1D("e2","e2",S1K,0,S1K-1);
  h_e2->Reset();

  h_tdiff = new TH1D("tdiff","tdiff",100,0,99);
  h_tdiff->Reset();
  
  printf("Program sorts TTEABLim histograms for the GRIFFIN \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);
  
  if(name->flag.inp_data!=1)
    {
      printf("ERROR! Input data file not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.GRIFFIN_cal_par==1)
    {
      printf("GRIFFIN calibration read from: %s\n",name->fname.GRIFFIN_cal_par);
      initialize_GRIFFIN_calibration(&cal_par->gr,name->fname.GRIFFIN_cal_par);
    }
  else
    {
      printf("ERROR!! GRIFFIN calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }

  sort(name);

  sprintf(title,"GRIFFIN_TTCalEABLim.root");
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
  h_e1->Write();
  h_e2->Write();
  h_tdiff->Write();
  
}
