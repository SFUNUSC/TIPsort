#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos,col,csi;
  double ttg,tcsi;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_GRIFFIN(data,&cal_par->gr,&cev->gr);
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);

  if(cev->gr.h.FT>0)
    if(cev->csiarray.h.FT>0)
      for(pos=1;pos<NPOSGRIF;pos++)
	if((cev->gr.h.THP&(1<<(pos-1)))!=0)
	  if(cev->gr.det[pos].hge.FT>0)
	    for(col=0;col<NCOL;col++)
	      if((cev->gr.det[pos].hge.THP&(1<<col))!=0)
		if(cev->gr.det[pos].ge[col].h.FT>0)
		  if((cev->gr.det[pos].ge[col].h.THP&1)!=0)
		    {
		      ttg=cev->gr.det[pos].ge[col].seg[0].T/cal_par->gr.contr_t;
		      for(csi=1;csi<NCSI;csi++)
			if((cev->csiarray.h.THP&(one<<csi))!=0)
			  {
			    tcsi=cev->csiarray.csi[csi].T/cal_par->csiarray.contr_t;
			    h->Fill(ttg,tcsi);
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
      printf("Griffin_CsIArray TTCal master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("Griffin Csiarray Time","Griffin Csiarray Time",S1K,0,S1K,S1K,0,S1K);
  h->Reset();

  printf("Program sorts calibrated 2D histogram for GRIFFIN/CSIARRAY timing \n");
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

  if(name->flag.GRIFFIN_cal_par==1)
        {
          printf("\nGRIFFIN calibration read from the file:\n %s\n",name->fname.GRIFFIN_cal_par);
          initialize_GRIFFIN_calibration(&cal_par->gr,name->fname.GRIFFIN_cal_par);
	  
        }
      else
        {
          printf("\nGRIFFIN calibration parameters not defined\n");
          exit(-1);
        }

  if(name->flag.CSIARRAY_cal_par==1)
        {
          printf("\nCSIARRAY calibration read from the file:\n %s\n",name->fname.CSIARRAY_cal_par);
          initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
	  
        }
      else
        {
          printf("\nCSIARRAY calibration parameters not defined\n");
          exit(-1);
        }



  sort(name);

 
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("TTCal", "TTCal",10,10, 500, 500);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

