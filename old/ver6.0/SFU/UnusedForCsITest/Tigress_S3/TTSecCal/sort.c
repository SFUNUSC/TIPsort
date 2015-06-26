#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos,col,sec;
  double ttg,tsec;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_S3sec(data,&cal_par->s3sec,&cev->s3);

  if(cev->tg.h.FT>0)
    if(cev->s3.sh.FT>0)
      for(pos=1;pos<NPOS;pos++)
	if((cev->tg.h.THP&(1<<(pos-1)))!=0)
	  if(cev->tg.det[pos].hge.FT>0)
	    for(col=0;col<NCOL;col++)
	      if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
		if(cev->tg.det[pos].ge[col].h.FT>0)
		  if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
		    {
		      ttg=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;
		      for(sec=1;sec<NS3SEC;sec++)
			if((cev->s3.sh.THP&(one<<sec))!=0)
			  {
			    tsec=cev->s3.sec[sec].T/cal_par->s3sec.contr_t;
			    h->Fill(ttg,tsec);
			  }
		    }
  free(cev);
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
      printf("\n ./TIGRESS_TTSecCal master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("Tigress S3sec Time","Tigress S3sec Time",S4K,0,S4K-1,S4K,0,S4K-1);
  h->Reset();

  printf("Program sorts calibrated 2D histogram for TIGRESS/S3 sector timing \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

  read_master(argv[1],name);

  if(name->flag.inp_data!=1)
    {
      printf("\nInput data file not defined\n");
      exit(EXIT_FAILURE);
    }

  if(name->flag.TIGRESS_cal_par==1)
        {
          printf("\nTIGRESS calibration read from the file:\n %s\n",name->fname.TIGRESS_cal_par);
          initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
	  
        }
      else
        {
          printf("\nTIGRESS calibration parameters not defined\n");
          exit(-1);
        }

 if(name->flag.S3sec_cal_par==1)
        {
          printf("\nS3 sectors calibration read from the file:\n %s\n",name->fname.S3sec_cal_par);
          initialize_S3sec_calibration(&cal_par->s3sec,name->fname.S3sec_cal_par);
	  
        }
      else
        {
          printf("\n S3 sector calibration parameters not defined\n");
          exit(EXIT_FAILURE);
        }



  sort(name);

 
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Cal. Tigress-S3 sector time", "Cal. Tigress-S3 sector",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}
