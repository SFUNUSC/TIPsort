#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  double ttg,tpin;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_PINARRAY(data,&cal_par->pinarray,&cev->pinarray);
  
  if(cev->tg.h.FT>0 && cev->pinarray.h.FT>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.THP&(1<<(pos-1)))!=0)
	if(cev->tg.det[pos].hge.FT>0)
	  for(col=0;col<NCOL;col++)
	    if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
	      if(cev->tg.det[pos].ge[col].h.FT>0)
		if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
		  {
		    ttg=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;
		    
		    for(pin=1;pin<NPIN;pin++)
		      if((cev->pinarray.h.THP&(one<<pin))!=0)
			{
			  tpin=cev->pinarray.pin[pin].T/cal_par->pinarray.contr_t;
			  //printf("Tigress and PIN times %f %f\n",ttg, tpin);
			  h->Fill(ttg,tpin);
			  h1->Fill(ttg-tpin);
			}}
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*==========================================================================*/
int main(int argc, char *argv[])
{
  FILE *cluster;
  input_names_type* name;
  char title[64],DataFile[256];
  
  if(argc!=2)
    {
      printf("TigressPINArray_TTCalSum master_file_name\n");
      exit(-1);
    }
  
  sprintf(title,"Tigress_PINArray_Time");
  h = new TH2D(title,title,S4K,S16K-S2K,S16K+S2K-1,S4K,S16K-S2K,S16K+S2K-1);
  h->Reset();

  sprintf(title,"Tigress_Time_minus_PINArray_Time");
  h1 = new TH1D(title,title,S4K,S16K-S2K,S16K+S2K-1);
  h1->Reset();
  
  printf("Program sorts TTCalSum spectra for Tigress (x) and PINArray (y).\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);
  
  if(name->flag.cluster_file==1)
    {
      printf("Sorting 2D TCalSum spectra for Tigress and PINArray from: %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("ERROR! I can't open input file %s\n",name->fname.cluster_file);
	  exit(-2);
	}}
  else
    {
      printf("ERROR! Cluster file not defined\n");
      exit(-1);
    }
  
  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calibration read from: %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR!!! TIGRESS calibration parameters not defined!\n");
      exit(-1);
    }
  
  if(name->flag.PINARRAY_cal_par==1)
    {
      printf("PINArray calibration read from %s\n",name->fname.PINARRAY_cal_par);
      initialize_PINARRAY_calibration(&cal_par->pinarray,name->fname.PINARRAY_cal_par);
    }
  else
    {
      printf("ERROR! PINArray calibration parameters not defined\n");
      exit(EXIT_FAILURE);
    }
  
  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting data from file %s\n",DataFile);
      sort(name);
    }
  
  sprintf(title,"TigressPINArray_TTCal.root");
  TFile f(title, "new");
  h->GetXaxis()->SetTitle("Tigress Rel. TCal [ns]");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("PINArray Rel. TCal [ns]");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  gStyle->SetPalette(1);
  
  h->Write();
  h1->Write();
}
