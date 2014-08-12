#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos,col;
  double e,t;
  long long int one=1;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  if(cev->tg.h.FH>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.HHP&(one<<(pos-1)))!=0)
  	if(cev->tg.det[pos].hge.FH>0)
  	  for(col=0;col<NCOL;col++)
  	    if((cev->tg.det[pos].hge.HHP&(one<<col))!=0)
  	      if(cev->tg.det[pos].ge[col].h.FH>0)
  		if((cev->tg.det[pos].ge[col].h.HHP&1)!=0)
  		  {
		    t=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;
  		    e=cev->tg.det[pos].ge[col].seg[0].E/cal_par->tg.contr_e;

		    h_t->Fill(t);
		    h_e->Fill(e);
		    h->Fill(t,e);
			    
  		  }
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  FILE *cluster;
  char n[132];
  TCanvas *c1,*c2,*c3;
  TApplication *theApp;

  if(argc!=2)
    {
      printf("\n ./TIGRESS_TECal master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("TTCal","TTCal",S4K,0,S16K,S4K,0,S4K-1);
  h->Reset();

  h_t = new TH1D("TTCal_t","TTCal_t",S8K,0,S8K);
  h_t->Reset();
  h_e = new TH1D("TTCal_e","TTCal_e",S4K,0,S4K-1);
  h_e->Reset();

  printf("Program sorts calibrated energy histogram for the TIGRESS \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

  read_master(argv[1],name);

  if(name->flag.cluster_file==1)
        {
          printf("\nSorting data from cluster file:\n %s\n",name->fname.cluster_file);
	  if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	    {
	      printf("\nI can't open input file %s\n",name->fname.cluster_file);
	      exit(-2);
	    }
        }
      else
        {
          printf("\nCluster file not defined\n");
          exit(-1);
        }

 
  if(name->flag.TIGRESS_cal_par==1)
        {
          printf("\nTIGRESS calibration read from the file:\n %s\n",name->fname.TIGRESS_cal_par);
          initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
	  
        }
      else
        {
          printf("\nTIGRESS calibration parameters not defined\n");
          exit(EXIT_FAILURE);
        }


  name->flag.inp_data=1; 
  while(fscanf(cluster,"%s",n)!=EOF)
    {
      strcpy(name->fname.inp_data,n);
      sort(name);
    }

  fclose(cluster);
 
 
  theApp=new TApplication("App", &argc, argv);
  c1 = new TCanvas("TTCal", "TTCal",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");

  c2 = new TCanvas("TTCal_t", "TTCal_t",10,10, 500, 300);
  gPad->SetLogz(1);
  h_t->Draw("");
  
  c3 = new TCanvas("TTCal_e", "TTCal_e",10,10, 500, 300);
  gPad->SetLogz(1);
  h_e->Draw("");
  
  theApp->Run(kTRUE);

}

