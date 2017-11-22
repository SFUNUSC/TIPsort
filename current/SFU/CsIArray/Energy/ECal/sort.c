#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  uint64_t one=1;
  int pos;
  double e;

 //Need to ignore RF bit for analysis of triple alpha calibration sources
 /*if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;
*/
 if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);

  if(cev->csiarray.h.FE>0)
    for(pos=1;pos<NCSI;pos++)
      if((cev->csiarray.h.EHP[pos/64]&(one<<pos%64))!=0)
	{
	  e=cev->csiarray.csi[pos].E/cal_par->csiarray.contr_e;
	  /* printf("CsIArray pos %d ene = %f\n",pos,e); */
	  /* getc(stdin); */
          
	  if(e>=0 && e<S32K)
	    {
	      hist[pos][(int)(e)]++;
	      h->Fill(e,pos);
	    }
	  else
	    {
	      hist[pos][S32K-1000]++;
	      h->Fill(S32K-1000,pos);
	    }
	}
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;

  if(argc!=2)
    {
      printf("CsIArray_ECal master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("ECal","ECal",S4K,0,S4K-1,NCSI+1,0,NCSI);
  h->Reset();

  printf("Program sorts ECal histograms for the CsIArray.\n");
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
  
  if((output=fopen("CsIArray_ECal.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!");
      exit(EXIT_FAILURE);
    }
  for(int pos=0;pos<NCSI;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);
  
  fclose(output);

  /* theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("ECal","ECal",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);*/
}
