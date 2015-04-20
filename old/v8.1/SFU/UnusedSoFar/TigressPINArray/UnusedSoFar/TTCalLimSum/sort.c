#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  double ttg,tpin;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_PINArray(data,&cal_par->pinarray,&cev->pinarray);

  if(cev->tg.h.FH>0 && cev->pinarray.h.FH>0)
    for(pos=1;pos<NPOS;pos++)
      if((cev->tg.h.HHP&(1<<(pos-1)))!=0)
	if(cev->tg.det[pos].hge.FH>0)
	  for(col=0;col<NCOL;col++)
	    if((cev->tg.det[pos].hge.HHP&(1<<col))!=0)
	      if(cev->tg.det[pos].ge[col].h.FH>0)
		if((cev->tg.det[pos].ge[col].h.HHP&1)!=0)
		  {
		    ttg=cev->tg.det[pos].ge[col].seg[0].T;

		    if(ttg>cal_par->tg.ctlow[pos][col])
		      if(ttg<cal_par->tg.cthigh[pos][col])
			for(pin=1;pin<NPIN;pin++)
			  if((cev->pinarray.h.HHP&(one<<pin))!=0)
			    {
			      tpin=cev->pinarray.pin[pin].T;
			
			      if(tpin>cal_par->pinarray.tlow[pin])
				if(tpin<cal_par->pinarray.thigh[pin])
				  {
				    ttg/=cal_par->tg.contr_t;
				    tpin/=cal_par->pinarray.contr_t;
				    h[pin]->Fill(ttg,tpin);
				  }}}
  free(cev);
  return SEPARATOR_DISCARD;
}
/*=========================================================================*/
int main(int argc, char *argv[])
{
    FILE *cluster;
    input_names_type* name;
    char title[64],DataFile[256];

  if(argc!=2)
    {
      printf("TigressPINArray_TCalLimSum master_file_name\n");
      exit(-1);
    }
  
  for(pin=1;pin<NPIN;pin++)
    {
      sprintf(title,"TigressPINArray Pin%.2d Gated",pin);
      h[pin] = new TH2D(title,title,S4K,0,S4K-1,S4K,0,S4K-1);
      h[pin]->Reset();
    }

  printf("Program sorts 2D TCalLim spectra for Tigress (x) and PINArray (y).\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);
  
  if(name->flag.cluster_file==1)
    {
      printf("Sorting 2D TCalLimSum spectra for Tigress and PINArray from: %s\n",name->fname.cluster_file);
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
      printf("ERROR! TIGRESS calibration parameters not defined.\n");
      exit(-1);
    }
  
  if(name->flag.PINArray_cal_par==1)
    {
      printf("PINArray calibration read from: %s\n",name->fname.PINArray_cal_par);
      initialize_PINArray_calibration(&cal_par->pinarray,name->fname.PINArray_cal_par);
    }
  else
    {
      printf("ERROR! PINArray calibration parameters not defined.\n");
      exit(-1);
    }

  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting data from file %s\n",DataFile);
      sort(name);
    }

  for(pin=1;pin<NPIN;pin++)
    {
      sprintf(title,"TigressPIN%.2d_TCal.root",pin);
      TFile f(title, "new");
      h[pin]->GetXaxis()->SetTitle("Tigress Rel. TCal [ns]");
      h[pin]->GetXaxis()->CenterTitle(true);
      h[pin]->GetYaxis()->SetTitle("PINArray Rel. TCal [ns]");
      h[pin]->GetYaxis()->CenterTitle(true);
      h[pin]->GetYaxis()->SetTitleOffset(1.5);
      h[pin]->SetOption("COLZ");
      gStyle->SetPalette(1);
      
      h[pin]->Write();
    }}

