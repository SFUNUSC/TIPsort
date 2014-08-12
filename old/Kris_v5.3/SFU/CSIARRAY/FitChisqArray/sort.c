#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos;
  double chi;
 
  for(pos=1;pos<NCSI;pos++)
    if((data->csiarray.h.TSHP&(one<<pos))!=0)
      {
	chi=data->csiarray.wfit[pos].chisq;
	chi/=data->csiarray.wfit[pos].ndf;

	h->Fill(pos,chi);
	if(chi>S32K) chi=S32K-4;
	if(chi<0) chi=S32K-2;

	hist[pos][(int)rint(chi)]++;
	    
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


  if(argc!=2)
    {
      printf("\n ./CSIARRAY_FitChisqArray sfu_input_data_file_name\n");
      exit(-1);
    }
  h = new TH2D("CsI ChiSq","CsI Chisq",NCSI+1,0,NCSI,S2K,0,S32K);
  h->Reset();

  printf("Program sorts Chisq histogram for the CSIARRAY \n");
 
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);

  if((output=fopen("CSIArray_fit_chisq.mca","w"))==NULL)
    {
      printf("\nI can't open file %s\n","CSIArray_fit_chisq.mca");
      exit(EXIT_FAILURE);
    }
  for(int pos=0;pos<NCSI;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);

  fclose(output);
 theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("CsI Chisq", "CsI Chisq",10,10, 500, 300);
  //  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

