#include "sort.h"
/*================================================================*/
Double_t myfunction(Int_t i)
{
 
  Double_t x,s;


  x=i-par->t[0];
  if(x<=0) 
    return par->am[0];
  else
    {   
      s=par->am[0];
      s+=par->am[1]*exp(-x/par->t[1]);
      s-=par->am[2]*exp(-x/par->t[2]);
      s-=par->am[3]*exp(-x/par->t[3]);
      return s;
    }
}
/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* original_waveform)
{
 
  Int_t d;
  WaveFormPar wpar;
  double ch;
  short waveform[S4K];

  memset(waveform,0,sizeof(waveform));

  for(Int_t i=0;i<ptr->waveform_length/contr;i++)
    for(Int_t j=0;j<contr;j++)
      waveform[i]+=original_waveform[contr*i+j];
  
  d=ptr->waveform_length/contr;

  if(ptr->channel==chn)
    if(d!=0)
      {
	wpar.baseline_range=RANGE/contr;
	get_t10t90(d,waveform,&wpar);
	if(wpar.t10t90_flag==1)
	  {
	    get_shape(4,d,waveform,par,&wpar);
	    ch=par->chisq/par->ndf;
	    /* if(ch>=chmin) */
	    /*   if(ch<=chmax) */
	    {
		
		print_fragment_info(ptr,S16K);
		printf("   Waveform    b: %10.1f\n",wpar.b);
		printf("   Waveform  max: %10.1f\n",wpar.max);
		printf("   Waveform tmax: %10.4f\n",wpar.tmax);
		printf("        Flag t10: %10d\n",wpar.t10_flag);
		printf("   Waveform  t10: %10.4f\n",wpar.t10);
		printf("        Flag t90: %10d\n",wpar.t90_flag);
		printf("   Waveform  t90: %10.4f\n",wpar.t90);
		printf("     Flag t10t90: %10d\n",wpar.t10t90_flag);
		printf(" Risetime T10T90: %10.4f\n",wpar.t10t90);
		printf("------ Chisq/NDF: %10.4f\n",ch);		
		for(Int_t i=0;i<5;i++)
		  printf("Id %1d Amp. %10.3f  T %10.3f\n",i,par->am[i],par->t[i]);
		if(h!=NULL) delete h;		
		h=new TH1D("Waveform","Waveform",d,0,d);
		if(g!=NULL) delete g;
		g=new TH1D("Fit","Fit",d,0,d);
		if(c!=NULL) delete c;
		c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);
		for(Int_t i=0;i<d;i++)
		  {
		    h->Fill(i,waveform[i]);
		    g->Fill(i,myfunction(i));
		  }
		h->Draw();
		g->SetLineColor(kRed);
		g->Draw("same");
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

 if(argc!=9)
    {
      printf("\n ./wfit_lin_chisq midas_input_data_file_name channel trc tf ts min max contr");
      exit(-1);
    }
  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));
  chn=atoi(argv[2]);
  contr=atoi(argv[8]);
  par->t[1]=atof(argv[3])/(double)contr;
  par->t[2]=atof(argv[4])/(double)contr;
  par->t[3]=atof(argv[5])/(double)contr;
  chmin=atof(argv[6]);
  chmax=atof(argv[7]);

  theApp=new TApplication("App", &ac, av);

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */

 


}