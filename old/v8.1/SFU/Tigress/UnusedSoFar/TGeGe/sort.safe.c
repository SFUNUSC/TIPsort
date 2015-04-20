#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos1,pos2;
  int col1,col2;
  double t1,t2;
    
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
				    h->Fill(t1,t2);
				    h->Fill(t2,t1);
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
      printf("\n ./TIGRESS_TGeGe master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts calibrated time histogram for TIGRESS core coincidences \n");
  h = new TH2D("TGeGe","TGeGe",S2K,0,S2K,S2K,0,S2K);
  h->Reset();

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
          exit(EXIT_FAILURE);
        }



  sort(name);

  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("TGeGe", "TGeGe",10,10, 500, 500);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

