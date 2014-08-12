#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double chisq;
  double AF,AS,x,TF,TS,t0,ta;
  double beta,delta;
  
  if((data->h.setupHP&RF_BIT)!=0)
    for(pos=1;pos<NCSI;pos++)
      if((data->csiarray.h.THP&(one<<pos))!=0)
	{
	  //check fit type and chisq
	  type=data->csiarray.wfit[pos].type;
	  chisq=data->csiarray.wfit[pos].chisq/data->csiarray.wfit[pos].ndf;
	  	  
	  if(type==1)
	    if(chisq>=chimin && chisq<=chimax)
	      {
		AF = data->csiarray.wfit[pos].am[2];
		AS = data->csiarray.wfit[pos].am[3];

		x = AF/(AF+AS);

		TF  = data->csiarray.wfit[pos].t[2];
		TS = data->csiarray.wfit[pos].t[3];
		t0 = data->csiarray.wfit[pos].t[0];

		//printf("AF: %.2f AS: %.2f x: %.2f TF: %.2f TS: %.2f t0: %.2f chisq: %.2f alpha: %.2f\n",AF,AS,x,TF,TS,t0,chisq,alpha);

		ta = t0 + alpha/( x*( (1./TF)-(1./TS) ) + (1./TS) );

		ta*=16.;

		ta-=data->rf.sin.t0;

		ta*=0.625;
		ta-=1500.;

		if(x>0.65)
		  {
		    beta = 300.;
		    delta = 165.;
		  }
		else
		  {
		    beta=500.;
		    delta=0.;
		  }


		ta+=beta*x+delta;

	      }
	  //printf("x: %.2f ta: %.2f\n",x,ta);
	  h->Fill(x,ta);
	  
	}
  
  return SEPARATOR_DISCARD;
}
/*===============================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  
  if(argc!=5)
    {
      printf("CsIArray_TFitCFD sfu_input_data alpha chimin chimax\n");
      exit(-1);
    }
  //histogram of CFD fit vs x=AF/(AF+AS)
  h = new TH2D("TFitCFD","TFitCFD",S1K,0,2,S1K,0,S1K-1);
  h->Reset();

  //min and max fit type from waveform fit
  alpha=atof(argv[2]);
  //min and max chisq from waveform fit
  chimin=atof(argv[3]);
  chimax=atof(argv[4]);
  
  printf("Program sorts TFitCFD histograms for the CsIArray.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  
  sort(name);
   
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("TFitCFD", "TFitCFD",10,10, 700, 500);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  theApp->Run(kTRUE);
}
