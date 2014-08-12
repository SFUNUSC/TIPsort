#include "sort.h"
/*================================================================*/
Double_t myfunction(Double_t *y, Double_t *par)
{
 
  if(y[0]<=par[2])
    return par[0];
  else
    return par[0]+par[1]*(1-exp(-(y[0]-par[2])/par[3]))*exp(-(y[0]-par[2])/par[4]);
}

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
  int chn_i;
  
  for(chn_i=chnlow;chn_i<=chnhigh;chn_i++)
{
  if(ptr->channel==chn_i)
    if((d=ptr->waveform_length)!=0)
      {
	memset(&wpar,0,sizeof(WaveFormPar));
	wpar.baseline_range=CSI_BASELINE_RANGE;

	fit_CsI_waveform(d,waveform,par,&wpar);
	
	//print_WavePar(&wpar);
	//print_ShapePar(par);
	
	if(par->type==2)
	  {
	    if(h!=NULL) delete h;
	    h=new TH1D("Waveform","Waveform",ptr->waveform_length,0,ptr->waveform_length);
	    
	    for(Int_t i=0;i<ptr->waveform_length;i++)
	      h->Fill(i,waveform[i]);
	    
	    //printf("baseline: %.2f max: %.2f t0: %.2f t10: %.2f t90: %.2f t10t90: %.2f\n",wpar.baseline,wpar.max,wpar.t0,wpar.t10,wpar.t90,wpar.t10t90);
	    
	    f->SetParameter(0,wpar.baseline);
	    f->SetParameter(1,wpar.max-wpar.baseline);
	    f->SetParameter(2,wpar.t0);
	    f->SetParameter(3,par->t[4]);
	    f->SetParameter(4,par->t[1]);
	    
	    //printf("\n par4 init: %.2f\n",f->GetParameter(4));
	    
	    h->Fit("wfit","QN","QN",0,d-1); //quiet fit
	    //h->Fit("wfit","N","N",0,d-1); //more verbose fit
	    
	      /* if(c!=NULL) */
	      /* 	delete c; */
	      /* c = new TCanvas("Waveform", "Waveform",10,10, 700, 500); */
	      /* h->Draw(); */
	      /* f->SetLineColor(kRed); */
	      /* f->Draw("same"); */
	      /* theApp->Run(kTRUE); */
	      
	      //printf("\n par4 fit: %.2f, chisq/ndf: %.2f\n",f->GetParameter(4),f->GetChisquare()/f->GetNDF());
	      
	    if(f->GetParameter(4)>=2000 && f->GetParameter(4)<=8000) //~ two sigma on the mean
	      g->Fill(f->GetParameter(4),f->GetChisquare()/f->GetNDF());
	    g1->Fill(f->GetParameter(3),f->GetChisquare()/f->GetNDF());
	  }
      }
 }
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

  int ac;
  char *av[10];

 if(argc!=8)
    {
      printf("\n ./wfit_RC midas_input_data_file_name channel_low channel_high TRC TF TS TGamma\n");
      exit(-1);
    }

 par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));

  chnlow=atoi(argv[2]);
  chnhigh=atoi(argv[3]);
  par->t[1]=atof(argv[4]); //set tRC
  par->t[2]=atof(argv[5]); //set tF
  par->t[3]=atof(argv[6]); //set tS
  par->t[4]=atof(argv[7]); //set tGamma

  f=new TF1("wfit",myfunction,0,MAX_SAMPLES,5);
  g=new TH2D("TRC/chisq","TRC/chisq",2*S16K,0,2*S16K-1,1000,0,1);
  g1=new TH2D("TGamma/chisq","TGamma/chisq",S1K,0,99,1000,0,1);

  theApp=new TApplication("App", &ac, av);
  printf("Program fits RC and PIN rise on waveforms for selected channel\n");

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */

  if(c!=NULL) delete c;
  c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);
  g->Draw();
  theApp->Run(kTRUE);
  g1->Draw();
  theApp->Run(kTRUE);
 
}
