#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  Int_t i,d;
  Double_t chisq;
  WaveFormPar wpar;

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      memset(&wpar,0,sizeof(WaveFormPar));
      wpar.baseline_range=RANGE;
      get_t10t90(d,waveform,&wpar);
      if(wpar.t10t90_flag==1)
	  if(wpar.t10t90>80)
	    if(wpar.t10t90<10000)
	      {
		get_two_risetime_par(d,waveform,par,&wpar);
		chisq=par->chisq/par->ndf;
		if(chisq>chisq_min)
		  if(chisq<chisq_max)
		    {
		      printf("chisq  %10.4f ndf %10d chisq/ndf %10.4f\n",par->chisq,par->ndf,par->chisq/par->ndf);
		      h->Reset();
		      for(i=0;i<d;i++)
			h->Fill(i,waveform[i]);
		      h->Draw();
		      TF1 *base=new TF1("base","[0]",0,par->t[0]);
		      base->SetParameter(0,par->am[0]);
		      base->SetLineColor(kRed);
		      base->SetLineWidth(2);
	    
		      TF1 *signal=new TF1("signal","[0]+[1]*exp(-(x-[7])/[4])-[2]*exp(-(x-[7])/[5])-[3]*exp(-(x-[7])/[6])",par->t[0],d);
		      for(i=0;i<4;i++)
			signal->SetParameter(i,par->am[i]);
		      for(i=1;i<4;i++)
			signal->SetParameter(i+3,par->t[i]);
		      signal->SetParameter(7,par->t[0]);
		      signal->SetLineColor(kRed);
		      signal->SetLineWidth(2);
		      base->Draw("same");
		      signal->Draw("same");
		      
		      theApp->Run(kTRUE);
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
      printf("\n ./wfit_test_lin midas_input_data_file_name channel trc tf ts chisq_min chisq_max\n");
      exit(-1);
    }

  par=(TwoRiseTimePar*)malloc(sizeof(TwoRiseTimePar));
  chn=atoi(argv[2]);
  par->t[0]=0;
  par->t[1]=atof(argv[3]);
  par->t[2]=atof(argv[4]);
  par->t[3]=atof(argv[5]);
  chisq_min=atof(argv[6]);
  chisq_max=atof(argv[7]);
  theApp=new TApplication("App", &ac, av);
  h=new TH1D("Waveform","Waveform",MAX_SAMPLES,0,MAX_SAMPLES);

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("Program shows linearized chisq waveform fit for selected channel\n");



}
