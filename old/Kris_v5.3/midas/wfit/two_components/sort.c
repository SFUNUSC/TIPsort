#include "sort.h"
/*================================================================*/
Double_t myfunction(Double_t *y, Double_t *par)
{
 
  Double_t x;
 
  x=y[0]-par[1];

  if(x<=0) 
    return par[0];
  else
  return par[0]+(par[2]+par[3])*exp(-x/par[4])-par[2]*exp(-x/par[5])-par[3]*exp(-x/par[6]);

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
		print_fragment_info(ptr,S16K);
		printf("   Waveform    b: %10.1f\n",wpar.b);
		printf("   Waveform  max: %10.1f\n",wpar.max);
		printf("   Waveform tmax: %10.4f\n",wpar.tmax);
		printf("   Waveform  t10: %10.4f\n",wpar.t10);
		printf("   Waveform  t90: %10.4f\n",wpar.t90);
		printf(" Risetime T10T90: %10.4f\n",wpar.t10t90);		
		if(h!=NULL) delete h;
		h=new TH1D("Waveform","Waveform",ptr->waveform_length,0,ptr->waveform_length);
		if(c!=NULL) delete c;
		c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);
		
		for(Int_t i=0;i<ptr->waveform_length;i++)
		  h->Fill(i,waveform[i]);
		
		f->SetParameter(0,wpar.b);
		f->SetParameter(1,wpar.t10);
		f->SetParameter(2,0.5*(wpar.max-wpar.b));
		f->SetParameter(3,0.5*(wpar.max-wpar.b));
		f->FixParameter(4,TRC);
		f->FixParameter(5,TF);		
		f->FixParameter(6,TS);  
		h->Fit("wfit","","",0,d-1);
		
		h->Draw();
		theApp->Run(kTRUE);
		
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
      printf("\n ./wfit_two_components midas_input_data_file_name channel low high TRC TF TS\n");
      exit(-1);
    }

  chn=atoi(argv[2]);
  low=atof(argv[3]);
  high=atof(argv[4]);
  TRC=atof(argv[5]);
  TF=atof(argv[6]);
  TS=atof(argv[7]);

  f=new TF1("wfit",myfunction,0,MAX_SAMPLES,7);
  f->SetParName(0,"C");
  f->SetParName(1,"t0");
  f->SetParName(2,"A1");
  f->SetParName(3,"A2");
  f->SetParName(4,"TRC");
  f->SetParName(5,"TF");
  f->SetParName(6,"TS");
  f->SetLineColor(kRed);
  f->SetLineWidth(0.3);

  theApp=new TApplication("App", &ac, av);
  printf("Program fits fast risetime waveforms for selected channel\n");

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
 
}