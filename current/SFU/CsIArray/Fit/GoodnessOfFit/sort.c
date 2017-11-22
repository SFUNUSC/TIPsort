#include "sort.h"

int analyze_data(raw_event *data)
{
  uint64_t one=1;
  int pos,type;
  double chi;
  
  pos=5;
  for(pos=1;pos<NCSI;pos++)
  if((data->csiarray.h.THP[pos/64]&(one<<pos%64))!=0)
    {
      type=data->csiarray.wfit[pos].type;
      chi=data->csiarray.wfit[pos].chisq;
      chi/=data->csiarray.wfit[pos].ndf;
	
      //h->Fill(chi,type);
      //if(type=2)
      if(type>=0 && type<=4)
	//if(chi>=0 && chi<S32K)		
	if(chi>=0 && chi<10000)		
	  {
	    hist[type][(int)rint(chi)]++;
	    h->Fill(chi,type);
	  }

    }
  return SEPARATOR_DISCARD;
}
/*====================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  
  if(argc!=2)
    {
      printf("CsIArray_GoodnessOfFit sfu_input_data\n");
      exit(-1);
    }
  h = new TH2D("ChiSq/Fit type","Chisq/Fit type",S32K,0,S32K-1,5,0,4);
  h->Reset();
  
  printf("Program sorts Chisq histogram for the CSIARRAY \n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);
  
  if((output=fopen("CsIArray_GoodnessOfFit.mca","w"))==NULL)
    {
      printf("ERROR!!! I can't open the mca file!\n");
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
