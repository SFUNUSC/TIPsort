#include "sort.h"
double get_tm(ShapePar* par)
{
  double eps=0.00001;
  double a,af,as,tf,ts,trc,cf,cs,tauf,taus;
  double p,fp,q,fq,r,fr;
 
  trc=par->t[1];
  tf=par->t[2];
  ts=par->t[3];
  a=par->am[1];
  af=par->am[2];
  as=par->am[3];

  cf=af*trc/tf/a;
  cs=as*trc/ts/a;
  tauf=trc*tf/(trc-tf);
  taus=trc*ts/(trc-ts);
  p=0.;
  fp=1.-af-as;
  

  q=5;
  while(1)
    {
      fq=1-cf*exp(-q/tauf)-cs*exp(-q/taus);
      if(fq<0)
	q+=5;
      else
	break;
    }
  while(1)
    {
  
      r=p-(q-p)/(fq-fp)*fp;
      fr=1-cf*exp(-r/tauf)-cs*exp(-r/taus);
      if(fr>0)
	{
	  q=r;
	  fq=fr;
	}
      else
	{
	  p=r;
	  fp=fr;
	}
      if(fabs(fr)<eps)
	break;
    }

  return r;
}
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
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
 
  Int_t d;
  WaveFormPar wpar;
  double ch;
  double rise;

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      wpar.baseline_range=RANGE;
      get_t10t90(d,waveform,&wpar);
      if(wpar.t10t90_flag==1)
	{
	  get_shape(4,d,waveform,par,&wpar);
	  ch=par->chisq/par->ndf;
	  if(ch>=chmin) 
	    if(ch<=chmax)
	      if((rise=get_tm(par))>limit)
	      {
		
		print_fragment_info(ptr,S16K);
		printf("Computed risetime %10.1f\n",rise);
		printf("   Waveform    b: %10.1f\n",wpar.b);
		printf("   Waveform  max: %10.1f\n",wpar.max);
		printf("   Waveform tmax: %10.4f\n",wpar.tmax);
		printf("   Waveform  t10: %10.4f\n",wpar.t10);
		printf("   Waveform  t90: %10.4f\n",wpar.t90);
		printf(" Risetime T10T90: %10.4f\n",wpar.t10t90);
		printf("------ Chisq/NDF: %10.4f\n",ch);		
		for(Int_t i=0;i<5;i++)
		  printf("Id %1d Amp. %10.3f  T %10.3f\n",i,par->am[i],par->t[i]);
		if(h!=NULL) delete h;		
		h=new TH1D("Waveform","Waveform",ptr->waveform_length,0,ptr->waveform_length);
		if(g!=NULL) delete g;
		g=new TH1D("Fit","Fit",ptr->waveform_length,0,ptr->waveform_length);
		if(c!=NULL) delete c;
		c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);
		for(Int_t i=0;i<ptr->waveform_length;i++)
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
      printf("\n ./wfit_show_lin_two_components_long_risetime midas_input_data_file_name channel trc tf ts min max low_rise_limit");
      exit(-1);
    }
  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));
  chn=atoi(argv[2]);
  par->t[1]=atof(argv[3]);
  par->t[2]=atof(argv[4]);
  par->t[3]=atof(argv[5]);
  chmin=atof(argv[6]);
  chmax=atof(argv[7]);
  limit=atof(argv[8]);
  theApp=new TApplication("App", &ac, av);

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */

 


}
