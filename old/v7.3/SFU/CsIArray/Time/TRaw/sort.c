#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos;
  double t;
  int tt;
  
 if((data->h.setupHP&RF_BIT)!=0)
  if(data->csiarray.h.Tfold>0)
    for(pos=1;pos<NCSI;pos++)
      if((data->csiarray.h.THP&(one<<pos))!=0)
	{
	  t=data->csiarray.csi[pos].cfd&0x00ffffff;
	  t-=(data->csiarray.csi[pos].timestamp*16)&0x00ffffff;
	  t-=data->rf.sin.t0;
	  t+=S16K;
	  t*=0.625;
	  tt=(int)rint(t);
	  if(tt<0) tt=S32K-2;

	  if(tt>S32K) tt=S32K-3;
	      
	  hist[pos][tt]++;
	  h->Fill(tt,pos);
	}
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
      printf("CsIArray_TRaw sfu_input_data\n");
      exit(-1);
    }
  h = new TH2D("Raw Time","Raw Time",S32K,0,S32K-1,NCSI+1,0,NCSI);
  h->Reset();
  
  printf("Program sorts raw time histogram for the CSIARRAY \n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  
  sort(name);
  
  if((output=fopen("CsIArray_TRaw.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file.\n");
      exit(EXIT_FAILURE);
    }
  for(int pos=0;pos<NCSI;pos++) fwrite(hist[pos],S32K*sizeof(int),1,output);
  fclose(output);

  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Raw Time", "Raw Time",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE); 
}

