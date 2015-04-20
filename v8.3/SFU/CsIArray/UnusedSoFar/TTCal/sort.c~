#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos;
  double t;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  if(cev->csiarray.h.FT>0)
    for(pos=1;pos<NCSI;pos++)
      if((cev->csiarray.h.THP&(one<<pos))!=0)
	{
	  t=cev->csiarray.csi[pos].T/cal_par->csiarray.contr_t;
	  if(t>0)
	    if(t<S32K)
	      hist[pos][(int)rint(t)]++;
	  h->Fill(t,pos);
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
      printf("\n ./CSIARRAY_TCal master_file_name\n");
      exit(-1);
    }
  

  h = new TH2D("Cal Time","Cal Time",S2K,0,S32K,NCSI+1,0,NCSI);
  h->Reset();
 
  printf("Program sorts calibrated time histogram for the CSIARRAY \n");
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

  
  if(name->flag.CSIARRAY_cal_par==1)
        {
          printf("\nCSIARRAY calibration read from the file:\n %s\n",name->fname.CSIARRAY_cal_par);
          initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
	  
        }
      else
        {
          printf("\nCSIARRAY calibration parameters not defined\n");
          exit(EXIT_FAILURE);
        }



  sort(name);
 
  if((output=fopen("CSIArray_cal_time.mca","w"))==NULL)
    {
      printf("\nI can't open file %s\n","CSIArray_cal_time.mca");
      exit(EXIT_FAILURE);
    }
  for(int pos=0;pos<NCSI;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);

  fclose(output);
 
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Cal Time", "Cal Time",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

