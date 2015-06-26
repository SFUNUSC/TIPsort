#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double chi;
 
  pos=det_id;
    if((data->csiarray.h.TSHP&(one<<pos))!=0)
      {
	type=data->csiarray.wfit[pos].type;
	chi=data->csiarray.wfit[pos].chisq;
	chi/=data->csiarray.wfit[pos].ndf;
	
	h->Fill(chi,type);
	if(type>=0)
	  if(type<=4)
	    if(chi>=0)
	      if(chi<S32K)		
	      	hist[type][(int)rint(chi)]++;
	    
	}
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;


  if(argc!=3)
    {
      printf("\n ./CSIARRAY_FitChisqDet sfu_input_data_file_name det_number\n");
      exit(-1);
    }
  h = new TH2D("ChiSq/Fit type","Chisq/Fit type",S32K,0,S32K-1,5,0,4);
  h->Reset();

  printf("Program sorts Chisq histogram for the CSIARRAY \n");
  det_id=atoi(argv[2]);
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);

  if((output=fopen("CSIArray_fit_chisq.mca","w"))==NULL)
    {
      printf("\nI can't open file %s\n","CSIArray_fit_chisq.spn");
      exit(EXIT_FAILURE);
    }
  for(int pos=0;pos<5;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);

  fclose(output);
 theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Raw Time", "Raw Time",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}
