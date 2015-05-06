#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double chisq;
  double AF,AS,x,TF,TS,t0,ta;
  double beta,delta;

  //initialize some things
  AF=0.;
  AS=0.;
  x=0.;
  TF=0.;
  TS=0.;
  t0=0.;
  ta=0.;
  
  pos=position;
  
  if((data->h.setupHP&RF_BIT)!=0)
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
	      
	      if(x>xbar)
		{
		  beta = 300.;
		  delta = 165.;
		  ta+=beta*x+delta;
		  talpha_proj->Fill(ta);
		}
	      else
		{
		  beta=500.;
		  delta=0.;
		  ta+=beta*x+delta;
		  tproton_proj->Fill(ta);
		}
	      
	      //fill histograms
	      h->Fill(x,ta);
	      x_proj->Fill(x);
	      }
	//printf("x: %.2f ta: %.2f\n",x,ta);
	
      }
  
  return SEPARATOR_DISCARD;
}
/*===============================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  char title[132];
  
  if(argc!=7)
    {
      printf("CsIArray_TFitCFDChannel sfu_input_data alpha xbar chimin chimax position\n x > xbar are alpha particles (more fast component)\n");
      exit(-1);
    }
  //histogram of CFD fit vs x=AF/(AF+AS)
  h = new TH2D("TFitCFD","TFitCFD",S2K,0,2,S2K,0,S2K-1);
  h->Reset();

  //talpha projection
  talpha_proj = new TH1D("Talpha","Talpha",S2K,0,S2K-1);
  talpha_proj->Reset();

  //tproton projection
  tproton_proj = new TH1D("Tproton","Tproton",S2K,0,S2K-1);
  tproton_proj->Reset();

  //t0 projection
  x_proj = new TH1D("T0","T0",S1K,0,2);
  x_proj->Reset();

  //min and max fit type from waveform fit
  alpha=atof(argv[2]);
  xbar=atof(argv[3]);;
  
  //min and max chisq from waveform fit
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  position=atoi(argv[6]);
  
  printf("Program sorts TFitCFD histograms for the CsIArray.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  
  sort(name);
   
  //here we write a ROOT file with all the histograms
  sprintf(title,"CsIArray_TFitCFD.root");
  TFile f(title, "recreate");

  h->GetXaxis()->SetTitle("AF/(AF+AS) [arb.]");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("Talpha-TRF [CFD units]");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  gStyle->SetPalette(1);

  x_proj->GetXaxis()->SetTitle("AF/(AF+AS) [arb.]");
  x_proj->GetXaxis()->CenterTitle(true);
  x_proj->GetYaxis()->SetTitleOffset(1.5);
  gStyle->SetPalette(1);

  talpha_proj->GetXaxis()->SetTitle("Talpha-TRF [CFD units]");
  talpha_proj->GetXaxis()->CenterTitle(true);
  talpha_proj->GetYaxis()->SetTitleOffset(1.5);
  gStyle->SetPalette(1);

  tproton_proj->GetXaxis()->SetTitle("Tproton-TRF [CFD units]");
  tproton_proj->GetXaxis()->CenterTitle(true);
  tproton_proj->GetYaxis()->SetTitleOffset(1.5);
  gStyle->SetPalette(1);

  h->Write();
  talpha_proj->Write();
  tproton_proj->Write();
  x_proj->Write();

  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("TFitCFD", "TFitCFD",10,10, 700, 500);
  h->Draw();
  theApp->Run(kTRUE);

}
