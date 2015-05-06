#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos;
  double t;
  int tt;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  
  if(cev->csiarray.h.FT>0)
    for(pos=1;pos<NCSI;pos++)
      if((cev->csiarray.h.THP&(one<<pos))!=0)
	{
	  //In CFD units, then scaled by contraction
	  t=cev->csiarray.csi[pos].T/cal_par->csiarray.contr_t;
	  //In ns
	  //t*=0.625;
	  
	  tt=(int)rint(t);
	  //printf("t %10.3f tt %d\n",t,tt);

	  //getc(stdin);

	  if(tt<0) tt=S32K-1000;
	  if(tt>S32K) tt=S32K-2000;
	  hist[pos][tt]++;

	  h->Fill(tt,pos);
	}
  free(cev);
  return SEPARATOR_DISCARD;
}
/*==================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  
  if(argc!=2)
    {
      printf("CsIArray_TCal master_file_name\n");
      exit(-1);
    }
  h = new TH2D("CsI TCal","CsI TCal",S32K,0,S32K-1,NCSI+1,0,NCSI);
  h->Reset();
  
  printf("Program sorts TCal histograms for the CSIARRAY.\n");
  
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
  
  if((output=fopen("CsIArray_TCal.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file.\n");
      exit(EXIT_FAILURE);
    }
  for(int pos=0;pos<NCSI;pos++) fwrite(hist[pos],S32K*sizeof(int),1,output);
  fclose(output);
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("CsI_TCal", "CsI_TCal",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE); 
}

