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
  
  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
      {
	memset(&wpar,0,sizeof(WaveFormPar));
	wpar.baseline_range=RANGE;
	get_t10t90(d,waveform,&wpar);
	if(wpar.t10t90_flag==1)
	  if(wpar.t10t90>low)
	    if(wpar.t10t90<high)
	      {
	
		if(h!=NULL) delete h;
		h=new TH1D("Waveform","Waveform",ptr->waveform_length,0,ptr->waveform_length);
		
		for(Int_t i=0;i<ptr->waveform_length;i++)
		  h->Fill(i,waveform[i]);
		
		f->SetParameter(0,wpar.b);
		f->SetParameter(1,wpar.max-wpar.b);
		f->SetParameter(2,wpar.t10);
		f->SetParameter(3,wpar.t10t90);
		f->SetParameter(4,750);
		
  
		h->Fit("wfit","QN","QN",0,d-1);
		
		g->Fill(f->GetParameter(4),f->GetChisquare()/f->GetNDF());
	      }
    }
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

  int ac;
  char *av[10];

 if(argc!=5)
    {
      printf("\n ./wfit_fast_risetimes midas_input_data_file_name channel low high\n");
      exit(-1);
    }

  chn=atoi(argv[2]);
  low=atof(argv[3]);
  high=atof(argv[4]);

  f=new TF1("wfit",myfunction,0,MAX_SAMPLES,5);
  g=new TH2D("TRC/chisq","TRC/chisq",S1K,0,S1K,1000,0,10);

  theApp=new TApplication("App", &ac, av);
  printf("Program fits fast risetime waveforms for selected channel\n");

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */

  if(c!=NULL) delete c;
  c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);
  g->Draw();
  theApp->Run(kTRUE);
 
}
