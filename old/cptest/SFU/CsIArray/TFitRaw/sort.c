#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double t,chi;

  for(pos=1;pos<NCSI;pos++)
    if((data->csiarray.h.THP&(one<<pos))!=0)
      {
	type=data->csiarray.wfit[pos].type;
	chi=data->csiarray.wfit[pos].chisq;
	chi/=data->csiarray.wfit[pos].ndf;
	
	if(type>=idmin && type<=idmax)
	  if(chi>=chimin && chi<=chimax)
	    {
	      t=16.*data->csiarray.wfit[pos].t[0]; 
	      if((data->h.setupHP&RF_BIT)!=0)
		{
		  t-=data->rf.sin.t0;
		  t+=S16K;
		  
		  if(t<0)t=S32K-2000;
		  if(t>S32K) t=S32K-3000;
		}
	      else t=S32K-4000;
	      
	      hist[pos][(int)rint(t)]++;
	      h->Fill(t,pos);
	    }}  

  return SEPARATOR_DISCARD;
}
/*===============================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;

  if(argc!=6)
    {
      printf("CsIArray_TFitRaw sfu_input_data idmin idmax chimin chimax\n");
      exit(-1);
    }
  h = new TH2D("TFitRaw","TFitRaw",S32K,0,S32K-1,NCSI+1,0,NCSI);
  h->Reset();
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  
  printf("Program sorts TFitRaw histograms for the CsIArray.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);
  
  if((output=fopen("CsIArray_TFitRaw.mca","w"))==NULL)
    {
      printf("ERROR!!! I can't open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  for(int pos=0;pos<NCSI;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);
  
  fclose(output);

  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("TFitRaw", "TFitRaw",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  theApp->Run(kTRUE);
}
