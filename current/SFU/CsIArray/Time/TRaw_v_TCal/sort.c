#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos;
  double traw,tcal;
  //int take=0;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  
 if(cev->csiarray.h.FT>0)
    for(pos=1;pos<NCSI;pos++)
      if((cev->csiarray.h.THP&(1<<pos))!=0)
	{
	  traw=data->csiarray.csi[pos].cfd&0x00ffffff;
	  traw-=(data->csiarray.csi[pos].timestamp*16)&0x00ffffff;
	  traw-=data->rf.sin.t0;
	  traw+=S16K;
		      
	  tcal=cev->csiarray.csi[pos].T/cal_par->csiarray.contr_t;
	  
	  h->Fill(traw,tcal);
	}
  
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
      printf("CsIArray_TRaw_v_TCal master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("CsI Array TRaw vs TCal","CsI Array TRaw vs TCal",S8K,0,S16K-1,S8K,0,S16K-1);
  h->Reset();
  
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
  if(name->flag.CSIARRAY_cal_par==1)
    {
      printf("CsIArray calpar read from: %s\n",name->fname.CSIARRAY_cal_par);
      initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
    }
  else
    {
      printf("ERROR! CsI Array calibration parameters not defined.\n");
      exit(EXIT_FAILURE);
    }
  
  sort(name);
  
  sprintf(title,"CsIArray_TRaw_v_TCal.root");
  TFile f(title, "recreate");
  h->GetXaxis()->SetTitle("CsI Array TRaw [arb]");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("CsI Array TCal [ns]");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  gStyle->SetPalette(1);
  h->Write();
}
