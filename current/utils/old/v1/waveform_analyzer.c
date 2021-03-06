#include "waveform_analyzer.h"

/*======================================================*/
void print_WavePar(WaveFormPar *wpar)
{
  printf("== Currently established waveform parameters ============\n");
  printf("baseline         : %10.2f\n",wpar->baseline);
  printf("baseline st. dev.: %10.2f\n",wpar->baselineStDev);
  printf("max              : %10.2f\n",(double)wpar->max);
  printf("tmax             : %10.2f\n",(double)wpar->tmax);
  printf("temin            : %10.2f\n",(double)wpar->temin);
  printf("temax            : %10.2f\n",(double)wpar->temax);
}
  
/*======================================================*/
void print_ShapePar(ShapePar *par)
{
  printf("== Shape parameters from the fit ========================\n");
  printf("   Fit type : %12d \n",par->type);	
  printf("   NDF      : %12d \n",par->ndf);	
  printf("   Chisq    : %15.2f\n",par->chisq);	
  printf("   Chisq/NDF: %15.2f\n",par->chisq/par->ndf);
 	
  for(int i=0;i<5;i++)
    printf("Id %1d Amp. %10.3f  T %10.3f  RF %10.3f\n",i,(double)par->am[i],(double)par->t[i],(double)par->rf[i]);
} 

/*======================================================*/
void get_baseline(int N, short *waveform,WaveFormPar *wpar)
{
  wpar->baseline=0.;
  wpar->baselineStDev=0.;
 
  //error if waveform length N is shorter than baseline range
  if(N<wpar->baseline_range)
    {
      printf("Baseline range (%d) larger than waveform length!\n",
	     wpar->baseline_range);
      printf("Terminating program\n");
      exit(0);
    }
  
  for(int i=0;i<wpar->baseline_range;i++)
    {
      wpar->baseline+=waveform[i]; //running sum of baseline
      wpar->baselineStDev+=waveform[i]*waveform[i];
    }
  
  wpar->baselineStDev/=wpar->baseline_range;
  wpar->baseline/=wpar->baseline_range; //baseline = average value of the baseline over the range
  wpar->baselineStDev-=wpar->baseline*wpar->baseline;
  wpar->baselineStDev=sqrt(wpar->baselineStDev); //std dev of baseline baselineStDev = sqrt(<b^2> - <b>^2)
  wpar->bflag=1; //ensures baseline is successfully established
}

/*======================================================*/
void get_tmax(int N, short *waveform,WaveFormPar *wpar)
{
  int i,j,sum;
  int D=FILTER/2;

  wpar->max=0;

  //applies the filter to the waveform
  for(i=D;i<N-D;i++)
    {
      sum=0;
      for(j=i-D;j<i+D;j++)
	sum+=waveform[j];
      sum/=FILTER; //the value of the filtered waveform at i
      if(sum>wpar->max)
	{
	  //if the value of sum (filtered waveform at i) is larger than the current maximum, max=sum and tmax = i
	  wpar->max=sum;
	  wpar->tmax=i;
	}
    }
  wpar->mflag=1; //flag after finding tmax
}

/*======================================================*/
/* function name follows Phil's convention but Kris' definition */
void get_exclusion_zone_for_CsI(int N, short *waveform,WaveFormPar *wpar)
{
  int i,j;
  int D=FILTER/2; //filter half width
  double sum; //sum of waveform across filter
  lin_eq_type e; //lin_eq_type for fitting the rise

  //initilize the fit parameters for the risetime to 0 for safety
  wpar->afit=0.;
  wpar->bfit=0.;
  
  //make sure the baseline is established prior to finding the exclusion zone
  wpar->baseline_range=CSI_BASELINE_RANGE;
  get_baseline(wpar->baseline_range,waveform,wpar);
  if(wpar->bflag!=1)
    {
      printf("Baseline not established\n");
      printf("Terminating program\n");
      exit(0);
    }
  
  //Here we determine the x position temax of the upper limit for the exclusion zone.

  //find tmax and define baselineMax
  get_tmax(N,waveform,wpar);
  //If tmax is established, continue.
  if(wpar->mflag==1)
    {
      wpar->baselineMax=wpar->baseline+NOISE_LEVEL_CSI;
      //printf("baseline: %.2f NOISE_LEVEL_CSI: %.2d baselineMax: %.2f\n", wpar->b,NOISE_LEVEL_CSI,baselineMax);
  //baselineMax = wpar->b + wpar->baselineStDev;
  //printf("baseline: %.2f baseline variance: %.2f baselineMax: %.2f\n", wpar->b,wpar->baselineStDev,baselineMax);
  
  //Starting at tmax and working backwards along the waveform get the value of the filtered waveform at i and when the value of the filtered waveform goes below baselineMax, set the upper limit of the exclusion zone temax = i. The exclusion zone cannot be defined in the area of the waveform used to calculate the baseline.
      for(i=wpar->tmax;i>wpar->baseline_range;i--)
	{
	  sum=0.;
	  for(j=i-D;j<i+D;j++)
	    sum+=waveform[j];
	  sum/=FILTER;
	  if(sum<wpar->baselineMax)
	    {
	      wpar->temax=i;
	      break;
	    }
	}
      //End of the determination of the upper limit of the exclusion zone.
      
      //Here we determine the x position of the lower limit for the exclusion zone
      
      /********** Default method: Raw waveform **********/
      
      /* //Set baselineMin */
      /* wpar->baselineMin=wpar->baseline-0.25*NOISE_LEVEL_CSI; */
      /* //printf("baseline: %.2f 0.25*NOISE_LEVEL_CSI: %.2f baselineMin: %.2f\n", wpar->b,0.25*NOISE_LEVEL_CSI,baselineMin); */
      /* //baselineMin = wpar->b - wpar->baselineStDev; */
      /* //printf("baseline: %.2f baseline variance: %.2f baselineMin: %.2f\n", wpar->b,wpar->baselineStDev,baselineMin); */

      /* //Starting at tmax and working backwards along the waveform get the value of the raw waveform at i and when the value of the raw waveform goes below baselineMin, set the lower limit of the exclusion zone tmin = i. The exclusion zone cannot be defined in the area of the waveform used to calculate the baseline. */
      /*  for(i=wpar->temax;i>wpar->baseline_range;i--) */
      /*    if(waveform[i]<wpar->baselineMin) */
      /*      break; */
      /*  wpar->temin=i; */

      /***** Alternative method: Fitting the risetime *****/

      //Set baselineMin
      wpar->baselineMin=wpar->baseline-NOISE_LEVEL_CSI;
      //printf("baseline: %.2f NOISE_LEVEL_CSI: %.2f baselineMin: %.2f\n", wpar->b,NOISE_LEVEL_CSI,baselineMin);
      //baselineMin = wpar->b - wpar->baselineStDev;
      //printf("baseline: %.2f baseline variance: %.2f baselineMin: %.2f\n", wpar->b,wpar->baselineStDev,baselineMin);
      
      //Here we fit a line y=ax+b from temax to temax + 3*FILTER and find the intersection with baselineMin. The x coordinate of this intersection becomes temin.
      //Matrix for the fit
      memset(&e,0,sizeof(lin_eq_type));
      e.dim=2;
      for(i=wpar->temax;i<=wpar->temax+3*FILTER;i++)
      	{
      	  e.matrix[0][0]+=1;
      	  e.matrix[0][1]+=i;
      	  e.matrix[1][1]+=i*i;
      	  e.vector[0]+=waveform[i];
      	  e.vector[1]+=waveform[i]*i;
      	}
      e.matrix[1][0]=e.matrix[0][1];
      
      //solve_lin_eq returns 0 if the determinant of the matrix is 0
      //=> the system is unsolvable. If there is no solution, set temin to the upper limit of the baseline range.
      if(solve_lin_eq(&e)==0)
      	wpar->temin=wpar->baseline_range;
      else
      	{
      	  wpar->bfit=e.solution[0];
      	  wpar->afit=e.solution[1];
      	  //solve baselineMin = afit*x + bfit for x to find the crossing point. If the crossing point is outside the acceptable range, set temin to the upper limit of the baseline range.
      	  wpar->temin=(int)floor((wpar->baselineMin-wpar->bfit)/wpar->afit);
      	  if(wpar->temin<wpar->baseline_range)
      	    wpar->temin=wpar->baseline_range;
      	  if(wpar->temin>wpar->temax)
      	    wpar->temin=wpar->baseline_range;
      	}
      //End of the determination of the lower limit of the exclusion zone.
    }
}

/*================================================================*/
void show_CsI_exclusion_zone(int N,short* waveform,WaveFormPar *wpar,TApplication* theApp)
{

  TH1D *h=NULL,*g=NULL,*k=NULL;
  TF1 *b=NULL,*baselineMin=NULL,*baselineMax=NULL;
  TF1 *f=NULL;
  TCanvas *c=NULL;
  
  //define some root objects which are needed to draw...
  if((h=(TH1D*)gROOT->FindObject("Waveform"))==NULL)
    h=new TH1D("Waveform","Waveform",N,0,N);
  else
    h->Reset();

 if((g=(TH1D*)gROOT->FindObject("Fit"))==NULL)
    g=new TH1D("Fit","Fit",N,0,N);
  else
    g->Reset();

 if((k=(TH1D*)gROOT->FindObject("Fit2"))==NULL)
    k=new TH1D("Fit2","Fit2",N,0,N);
  else
    k->Reset();

 //b is the baseline, baselineMin and baselineMax show the exclusion region defined by the baseline limits
 if(b!=NULL) delete b;
 b=new TF1("base","[0]",0,wpar->baseline_range);

 if(baselineMin!=NULL) delete baselineMin;
 baselineMin=new TF1("basemin","[0]",wpar->temin,wpar->temax);

if(baselineMax!=NULL) delete baselineMax;
 baselineMax=new TF1("basemax","[0]",wpar->temin,wpar->temax);
 
 //f is the fit function for the risetime
 if(f!=NULL) delete f;
 f=new TF1("fit","[0]*x+[1]",wpar->temin,wpar->temax+3*FILTER);

 //Done defining ROOT objects
 
 c=(TCanvas*)gROOT->FindObject("WaveformFit");
 if(c!=NULL) delete c;
 c = new TCanvas("WaveformFit", "WaveformFit",10,10, 700, 500);

 //h is the waveform
 for(Int_t i=0;i<N;i++)
   h->Fill(i,waveform[i]);
 
 //g is the waveform in the exclusion zone
 for(Int_t i=wpar->temin;i<wpar->temax;i++)
   g->Fill(i,waveform[i]);
 
 //k is the filtered waveform
 int D=FILTER/2; //filter half width
 double sum; //sum of waveform across filter
 for(Int_t i=wpar->temin;i<wpar->temax;i++)
   {
     sum=0.;
     for(Int_t j=i-D;j<i+D;j++)
       sum+=waveform[j];
     sum/=FILTER;
     //printf("filtered waveform = %.2f at x = %d\n",sum,i);
     k->Fill(i,sum);  
   }
 
 h->Draw();

 g->SetLineColor(kOrange);
 g->Draw("same");

 b->SetParameter(0,wpar->baseline);
 b->SetLineColor(kRed);
 b->SetLineWidth(2);
 b->Draw("same");

 f->SetParameter(0,wpar->afit);
 f->SetParameter(1,wpar->bfit);
 f->SetLineColor(6);
 f->SetLineWidth(2);
 f->Draw("same");

 baselineMin->SetParameter(0,wpar->baselineMin);
 baselineMin->SetLineColor(kRed);
 baselineMin->SetLineWidth(2);
 baselineMin->SetLineStyle(2);
 baselineMin->Draw("same");

 baselineMax->SetParameter(0,wpar->baselineMax);
 baselineMax->SetLineColor(kRed);
 baselineMax->SetLineWidth(2);
 baselineMax->SetLineStyle(2);
 baselineMax->Draw("same");

 k->SetLineColor(kGreen);
 k->SetLineWidth(2);
 k->Draw("same");
 
 theApp->Run(kTRUE);
}

/*================================================================*/
int get_shape(int dim, int N, short *waveform,ShapePar* par, WaveFormPar *wpar)
{
  long double sum,tau,tau_i,tau_j;
  int i,j,p,q,d;
  lin_eq_type e;
  memset(&e,0,sizeof(lin_eq_type));
 
  /* q is the low limit of the signal section */
  q=wpar->temax;
 
  /* p is the high limit of the baseline section */
  p=wpar->temin;
  d=N;
  e.dim=dim;

  //initialize amplitudes to 0
  for(int i=0;i<NSHAPE;i++) 
    par->am[i]=0.;

  //initialize chi square 0 and ndf = n-k to -k where k=dim
  par->chisq=0.;
  par->ndf=-e.dim;

  //linearized chi square fit is Mu = v where M is a matrix u, v are vectors
  //u is the parameter vector (solution)
  //note that in this formulation, chisq_min = y_i^2-sum(u_iv_i)

  //create matrix for linearized fit
  for(i=1;i<e.dim;i++)
    {
      tau=get_CsI_tau(i,par);
      tau_i=tau;

      sum=-((double)q)/tau+log(1.-exp(-((double)(d-q))/tau));
      sum-=log(1.-exp(-1./tau));
      e.matrix[i][0]=exp(sum);
      e.matrix[0][i]=exp(sum);
      
      tau/=2.;
      sum=-((double)q)/tau+log(1.-exp(-((double)(d-q))/tau));
      sum-=log(1.-exp(-1./tau));
      e.matrix[i][i]=exp(sum);
      
      for(j=i+1;j<e.dim;j++)
  	{
	  tau_j=get_CsI_tau(j,par);
  	  tau=tau_i*tau_j/(tau_i+tau_j);
  	  sum=-((double)q)/tau+log(1.-exp(-((double)(d-q))/tau));
  	  sum-=log(1.-exp(-1./tau));
  	  e.matrix[i][j]=exp(sum);
  	  e.matrix[j][i]=exp(sum);
  	}
    }

  e.matrix[0][0]=0.; 
  e.vector[0]=0.;

  for(j=q;j<d;j++)
    {
      e.vector[0]+=waveform[j];
      e.matrix[0][0]+=1;
      par->chisq+=waveform[j]*waveform[j];
      par->ndf+=1;
    }
  
  for(i=1;i<e.dim;i++)
    {
      tau=get_CsI_tau(i,par);
      e.vector[i]=0;

      for(j=q;j<d;j++)
  	e.vector[i]+=waveform[j]*exp(-(double(j))/tau);
    }
  
  for(j=0;j<p;j++)
    {
      e.vector[0]+=waveform[j];
      e.matrix[0][0]+=1;
      par->chisq+=waveform[j]*waveform[j];
      par->ndf+=1;
    }

  //solve the matrix equation Mu = v -> u = M^(-1)v where M^(-1) is the inverse
  //of M. note this has no solution if M is not invertable! 

  //error if the matrix cannot be inverted
  if(solve_lin_eq(&e)==0)
    	{
	  par->chisq=BADCHISQ_MAT;
	  par->ndf=1;
	  return BADCHISQ_MAT;
	}

  //else try and find t0 and calculate amplitudes
  else
    {
      //see the function comments for find t0 for details
      par->t[0]=get_t0(N,par,wpar,e);

      //if t0 is less than 0, return a T0FAIL
      if(par->t[0]<=0)
	{
	  par->chisq=BADCHISQ_T0;
	  par->ndf=1;
	  return BADCHISQ_T0;
	}
	
      //calculate amplitudes	       
      par->am[0]=e.solution[0];

      for(i=1;i<e.dim;i++)
	{
	  tau=get_CsI_tau(i,par);
	  par->am[i]=e.solution[i]*exp(-par->t[0]/tau);
	}

      for(i=0;i<e.dim;i++)	
	par->chisq-=e.solution[i]*e.vector[i];      
      //done claculating amplitudes
   
      //check chisq, return BADCHISQ_NEG if less than 0
      if(par->chisq<0)
      	{
	  par->chisq=BADCHISQ_NEG;
	  par->ndf=1;
	  return BADCHISQ_NEG;
	}

      for(i=2;i<e.dim;i++)
	par->am[i]*=-1;
     }
   
  //return BADCHISQ_AMPL if a component amplitude is less than 0
  for(i=0;i<e.dim;i++)
    if(par->am[i]<0)
      	{
	  par->chisq=BADCHISQ_AMPL;
	  par->ndf=1;
	  return BADCHISQ_AMPL;
	}

  return par->chisq;
}

/*================================================================*/
double get_t0(int N,ShapePar*par,WaveFormPar *wpar,lin_eq_type e)
{

  /*************************************************************************
  This function calculates t0 given the matrix solution from the get_shape.
  In this case, the fit function is written as follows:
  
  f(t) = C + (Af+As)*exp(t0/tRC)*exp(-t/tRC) - Af*exp(t0/tF')*exp(-t/tF')
           - As*exp(t0/tS')*exp(-t/tS')
  
  This can be re-written as:

  f(t) = C' + alpha*exp(-t/tRC) + beta*exp(-t/tF') + gamma*exp(-t/tS')

  Where:
  C = C'
  alpha = (Af+As)*exp(t0/tRC)
  beta = Af*exp(t0/tF')
  gamma = As*exp(t0/tS')

  Ignoring the constant, we have: 

  f'(t0) = alpha*exp(-t0/tRC) + beta*exp(-t0/tF') + gamma*exp(-t0/tS') = 0

  For t<t0, f'(t)< 0, and for t>t0, f'(t)>0. This function finds the
  intersection of f'(t) and 0 by linear interpolation from these endpoints.
  *************************************************************************/

  double fa,fb,fc; //value of the fit function at points a,b,c
  double ta,tb,tc; //corresponding time (x-)axis values
  double slope; //linear interpolation slope
  double delta; //checks how close is the interpolated f(t0) is to 0
  int i;

  //printf("In fit_t0\n");
  ta=wpar->baseline_range;
  fa=0.;
  
  //t0 must be between the baseline and the max
  //calculates fit value (no constant) at the end of the baseline range
  //this is the t<t0 point
  for(i=1;i<e.dim;i++)
    fa+=e.solution[i]*exp(-ta/par->t[i]);
  
  tb=wpar->tmax;
  fb=0.;
  
  //calculates fit value (no constant) at tmax
  //this is the t>t0 point
  for(i=1;i<e.dim;i++)
    fb+=e.solution[i]*exp(-tb/par->t[i]);

    delta=1;

    if( (fa<0) && (fb>0) )
	{
	  //keep the interpolation going until you get below epsilon
	  /* |f(t0) - 0| = |f(t0)|< epsilon */
	  while(delta>EPS)
	  { 
	    slope=-fa/(fb-fa); //interpolation slope for dependent variable t

	      //reason for this part?
	      //ensures that tc > tb or tc < ta?
	      //"reasonable" interpolation slopes?
	      /* if(slope>0.99) */
	      /* 	slope=0.99; */

	      /* if(slope<0.01) */
	      /* 	slope=0.01; */
	      //?
	      //its pretty harmless computationally

	      //tc is the estimate for t0
	      tc=ta+slope*(tb-ta);
	      fc=0.;
	      for(i=1;i<e.dim;i++)
	  	fc+=e.solution[i]*exp(-tc/par->t[i]);
	      
	      //really should have this, just to be safe
	      if(fc==0)
		{
		  return tc;
		}
	      
	      else if(fc>0)
	  	{
	  	  tb=tc;
	  	  fb=fc; 
	  	}
	      else
	  	{
	  	  ta=tc;
	  	  fa=fc; 
	  	}
	      delta=fabs(fc);
	      //printf("fa: %10.4f fb: %10.4f fc: %10.4f ta: %10.4f tb: %10.4f tc: %10.4f delta: %10.4f\n",(double)fa,(double)fb,(double)fc,(double)ta,(double)tb,(double)tc,(double)delta);
	    }
	}
      else
	{
	  /* printf("a %10.4f b %10.4f ta %10.4f tb %10.4f \n",(double)a,(double)b,(double)ta,(double)tb); */
	  /* printf("Bad initial condition in t0 solver\n"); */
	  return -1;
	}

    //printf("out of fit_t0\n");
    wpar->t0=tc;
    
    return wpar->t0;
}

/*================================================================*/
double get_CsI_tau(int i,ShapePar *par)
{
  //t[1] = tRC
  if(i==1)
   return par->t[1];

  //1/ti' = (ti+tRC)/(ti*tRC)
  if(i>=2)
    if(i<NSHAPE)
      return par->t[i]*par->t[1]/(par->t[i]+par->t[1]);

  return -1.;
}

/*================================================================*/
//this function displays the CsI waveform fit on a ROOT canvas.
void display_CsI_Fit(Int_t N,short* waveform,ShapePar *par, TApplication* theApp)
{
  TH1D *h=NULL,*g=NULL;
  TCanvas *c=NULL;
   
  if((h=(TH1D*)gROOT->FindObject("Waveform"))==NULL)	
    h=new TH1D("Waveform","Waveform",N,0,N);
  else
    h->Reset();

 if((g=(TH1D*)gROOT->FindObject("Fit"))==NULL)	
    g=new TH1D("Fit","Fit",N,0,N);
  else
    g->Reset();

 c=(TCanvas*)gROOT->FindObject("WaveformFit");
 if(c!=NULL) delete c;
 c = new TCanvas("WaveformFit", "WaveformFit",10,10, 700, 500);

  for(Int_t i=0;i<N;i++)
    {
      h->Fill(i,waveform[i]);
      g->Fill(i,CsI_FitFunction(i,par));
    }
  h->Draw();
  g->SetLineColor(kRed);
  g->Draw("same");

  theApp->Run(kTRUE);
}

/*================================================================*/
//two component fit function for CsI without RF
Double_t CsI_FitFunction(Int_t i,ShapePar *par)
{
  Double_t  x,s,e;

  x=i-par->t[0];
  e=exp(-x/par->t[1]);
  if(x<=0) 
    return par->am[0];
  else
    {   
      s=par->am[0];
      s+=par->am[2]*(1-exp(-x/par->t[2]))*e;
      s+=par->am[3]*(1-exp(-x/par->t[3]))*e;
      s+=par->am[4]*(1-exp(-x/par->t[4]))*e;
      return s;
    }
}

/*================================================================*/
//displays CsI fit with waveform and shape parameters
//use this function to display fit in external testing programs
void show_CsI_Fit(Int_t N,short* waveform,ShapePar *par,WaveFormPar *wpar, TApplication* theApp)
{
  print_WavePar(wpar);
  print_ShapePar(par);
  display_CsI_Fit(N,waveform,par,theApp);	
 
}

/*================================================================*/
//fit waveforms with the RF included
int get_shape_with_RF(int dim, int N, short *waveform,ShapePar* par, WaveFormPar *wpar)
{
  long double tau,tau_i,tau_j,l;
  int i,j,p,q,d;
  lin_eq_type e;
  double sum,s,sn,snm,s2,s2n,s2nm,c,cn,cnm,c2,c2n,c2nm,w,x,y;
  memset(&e,0,sizeof(lin_eq_type));
  
  if((dim!=4)&&(dim!=3))
    {
      printf("currently working for CsI fast+slow fit\n");
      printf("or CsI fast fit, only\n");
      printf("terminating execution\n");
      printf("or executing termination\n");
      exit(0);
    }

  /* q is the low limit of the signal section */
  q=wpar->temax;
 
  /* p is the high limit of the baseline section */
  p=wpar->temin;
  d=N;
  e.dim=dim+4;
  for(int i=0;i<NSHAPE;i++) par->am[i]=0.;
  for(int i=1;i<5;i++) par->rf[i]=0.;

  par->chisq=0.;
  par->ndf=-e.dim;

  for(i=1;i<dim;i++)
    {
      if(i==1)
	tau=par->t[1];
      else
	tau=par->t[i]*par->t[1]/(par->t[i]+par->t[1]);

      tau_i=tau;
      sum=-((double)q)/tau+log(1.-exp(-((double)(d-q))/tau));
      sum-=log(1.-exp(-1./tau));
      e.matrix[i][0]=exp(sum);
      e.matrix[0][i]=exp(sum);
      
      tau/=2.;
      sum=-((double)q)/tau+log(1.-exp(-((double)(d-q))/tau));
      sum-=log(1.-exp(-1./tau));
      e.matrix[i][i]=exp(sum);
      for(j=i+1;j<dim;j++)
  	{
	  if(j==1)
	    tau_j=par->t[1];
	  else
	    tau_j=par->t[j]*par->t[1]/(par->t[j]+par->t[1]);

  	  tau=tau_i*tau_j/(tau_i+tau_j);
  	  sum=-((double)q)/tau+log(1.-exp(-((double)(d-q))/tau));
  	  sum-=log(1.-exp(-1./tau));
  	  e.matrix[i][j]=exp(sum);
  	  e.matrix[j][i]=exp(sum);
  	}
  	
    }
  
  e.vector[0]=0;
  e.matrix[0][0]=0;
  for(j=q;j<d;j++)
    {
      e.vector[0]+=waveform[j];
      e.matrix[0][0]+=1;
      par->chisq+=waveform[j]*waveform[j];
      par->ndf+=1;
    }
  
  for(i=1;i<dim;i++)
    {
      if(i==1)
	tau=par->t[1];
      else
	tau=par->t[i]*par->t[1]/(par->t[i]+par->t[1]);

      e.vector[i]=0;
      for(j=q;j<d;j++)
  	e.vector[i]+=waveform[j]*exp(-(double(j))/tau);
    }
  
  for(j=0;j<p;j++)
    {
      e.vector[0]+=waveform[j];
      e.matrix[0][0]+=1;
      par->chisq+=waveform[j]*waveform[j];
      par->ndf+=1;
    }

  //RF on the baseline section
  w=6.283185307/par->rf[0];
  s=sin(w);
  sn=sin(p*w);
  snm=sin((p-1)*w);
  s2=sin(2*w);
  s2n=sin(2*p*w);
  s2nm=sin(2*(p-1)*w);

  c=cos(w);
  cn=cos(p*w);
  cnm=cos((p-1)*w);
  c2=cos(2*w);
  c2n=cos(2*p*w);
  c2nm=cos(2*(p-1)*w);

  e.matrix[dim][dim]+=0.5*p-0.25*(1-c2-c2n+c2nm)/(1-c2);
  e.matrix[dim][dim+1]+=0.25*(s2+s2nm-s2n)/(1-c2);
  e.matrix[dim+1][dim]=e.matrix[dim][dim+1];
  e.matrix[dim][0]+=0.5*(s+snm-sn)/(1-c);
  e.matrix[0][dim]=e.matrix[dim][0];
  e.matrix[dim+1][dim+1]+=0.5*p+0.25*(1-c2-c2n+c2nm)/(1-c2);
  e.matrix[dim+1][0]+=0.5*(1-c-cn+cnm)/(1-c);
  e.matrix[0][dim+1]=e.matrix[dim+1][0];
  
  for(i=0;i<p;i++)
    {
      e.vector[dim]+=waveform[i]*sin(w*i);
      e.vector[dim+1]+=waveform[i]*cos(w*i);
    }
  //end RF on the baseline section 

 //RF on the signal section
  sn=sin(N*w);
  snm=sin((N-1)*w);
  s2n=sin(2*N*w);
  s2nm=sin(2*(N-1)*w);

  cn=cos(N*w);
  cnm=cos((N-1)*w);
  c2n=cos(2*N*w);
  c2nm=cos(2*(N-1)*w);

  e.matrix[dim][dim]+=0.5*N-0.25*(1-c2-c2n+c2nm)/(1-c2);
  e.matrix[dim][dim+1]+=0.25*(s2+s2nm-s2n)/(1-c2);
  e.matrix[dim+1][dim]=e.matrix[dim][dim+1];
  e.matrix[dim][0]+=0.5*(s+snm-sn)/(1-c);
  e.matrix[0][dim]=e.matrix[dim][0];
  e.matrix[dim+1][dim+1]+=0.5*N+0.25*(1-c2-c2n+c2nm)/(1-c2);
  e.matrix[dim+1][0]+=0.5*(1-c-cn+cnm)/(1-c);
  e.matrix[0][dim+1]=e.matrix[dim+1][0];

 for(i=1;i<dim;i++)
    {	
     if(i==1)
	tau=par->t[1];
      else
	tau=par->t[i]*par->t[1]/(par->t[i]+par->t[1]);
      l=1/tau;
      x=(exp(-l)*s+exp(-l*(N+1))*snm-exp(-l*N)*sn)/(1-2*exp(-l)*c+exp(-2.*l));
      e.matrix[i][dim]+=x;
      e.matrix[dim][i]+=x;
      x=(1-exp(-l)*c-exp(-l*N)*cn+exp(-l*(N+1))*cnm)/(1-2*exp(-l)*c+exp(-2.*l));
      e.matrix[i][dim+1]+=x;
      e.matrix[dim+1][i]+=x;
    }

  sn=sin(q*w);
  snm=sin((q-1)*w);
  s2n=sin(2*q*w);
  s2nm=sin(2*(q-1)*w);

  cn=cos(q*w);
  cnm=cos((q-1)*w);
  c2n=cos(2*q*w);
  c2nm=cos(2*(q-1)*w);

  e.matrix[dim][dim]-=0.5*q-0.25*(1-c2-c2n+c2nm)/(1-c2);
  e.matrix[dim][dim+1]-=0.25*(s2+s2nm-s2n)/(1-c2);
  e.matrix[dim+1][dim]=e.matrix[dim][dim+1];
  e.matrix[dim][0]-=0.5*(s+snm-sn)/(1-c);
  e.matrix[0][dim]=e.matrix[dim][0];
  e.matrix[dim+1][dim+1]-=0.5*q+0.25*(1-c2-c2n+c2nm)/(1-c2);
  e.matrix[dim+1][0]-=0.5*(1-c-cn+cnm)/(1-c);
  e.matrix[0][dim+1]=e.matrix[dim+1][0];
  
 for(i=1;i<dim;i++)
    {
     if(i==1)
	tau=par->t[1];
      else
	tau=par->t[i]*par->t[1]/(par->t[i]+par->t[1]);
      l=1/tau;
      x=(exp(-l)*s+exp(-l*(q+1))*snm-exp(-l*q)*sn)/(1-2*exp(-l)*c+exp(-2.*l));
      e.matrix[i][dim]-=x;
      e.matrix[dim][i]-=x;
      x=(1-exp(-l)*c-exp(-l*q)*cn+exp(-l*(q+1))*cnm)/(1-2*exp(-l)*c+exp(-2.*l));
      e.matrix[i][dim+1]-=x;
      e.matrix[dim+1][i]-=x;
    }
  for(i=q;i<N;i++)
    {
      e.vector[dim]+=waveform[i]*sin(w*i);
      e.vector[dim+1]+=waveform[i]*cos(w*i);
    }
  //end RF on the signal section 

 //3*wRF on the baseline section
  w=3*6.283185307/par->rf[0];
  s=sin(w);
  sn=sin(p*w);
  snm=sin((p-1)*w);
  s2=sin(2*w);
  s2n=sin(2*p*w);
  s2nm=sin(2*(p-1)*w);

  c=cos(w);
  cn=cos(p*w);
  cnm=cos((p-1)*w);
  c2=cos(2*w);
  c2n=cos(2*p*w);
  c2nm=cos(2*(p-1)*w);

  e.matrix[dim+2][dim+2]+=0.5*p-0.25*(1-c2-c2n+c2nm)/(1-c2);
  e.matrix[dim+2][dim+3]+=0.25*(s2+s2nm-s2n)/(1-c2);
  e.matrix[dim+3][dim+2]=e.matrix[dim+2][dim+3];
  e.matrix[dim+2][0]+=0.5*(s+snm-sn)/(1-c);
  e.matrix[0][dim+2]=e.matrix[dim+2][0];
  e.matrix[dim+3][dim+3]+=0.5*p+0.25*(1-c2-c2n+c2nm)/(1-c2);
  e.matrix[dim+3][0]+=0.5*(1-c-cn+cnm)/(1-c);
  e.matrix[0][dim+3]=e.matrix[dim+3][0];
  
  for(i=0;i<p;i++)
    {
      e.vector[dim+2]+=waveform[i]*sin(w*i);
      e.vector[dim+3]+=waveform[i]*cos(w*i);
    }

  w=2*6.283185307/par->rf[0];
  s=sin(w);
  sn=sin(p*w);
  snm=sin((p-1)*w);
 
  c=cos(w);
  cn=cos(p*w);
  cnm=cos((p-1)*w);
 
  x=0.5*(s+snm-sn)/(1-c);//sum of sin2wt
  y=0.5*(1-c-cn+cnm)/(1-c);//sum of cos2wt

  e.matrix[dim][dim+2]+=0.5*y;
  e.matrix[dim][dim+3]-=0.5*x;
  e.matrix[dim+1][dim+2]+=0.5*x;
  e.matrix[dim+1][dim+3]+=0.5*y;

  w=4*6.283185307/par->rf[0];
  s=sin(w);
  sn=sin(p*w);
  snm=sin((p-1)*w);
 
  c=cos(w);
  cn=cos(p*w);
  cnm=cos((p-1)*w);
 
  x=0.5*(s+snm-sn)/(1-c);//sum of sin4wt
  y=0.5*(1-c-cn+cnm)/(1-c);//sum of cos4wt

  e.matrix[dim][dim+2]-=0.5*y;
  e.matrix[dim][dim+3]+=0.5*x;
  e.matrix[dim+1][dim+2]+=0.5*x;
  e.matrix[dim+1][dim+3]+=0.5*y;
  //end 3*wRF on the baseline section 

 //3*wRF on the signal section
  w=3*6.283185307/par->rf[0];

  sn=sin(N*w);
  snm=sin((N-1)*w);
  s2n=sin(2*N*w);
  s2nm=sin(2*(N-1)*w);

  cn=cos(N*w);
  cnm=cos((N-1)*w);
  c2n=cos(2*N*w);
  c2nm=cos(2*(N-1)*w);

  e.matrix[dim+2][dim+2]+=0.5*N-0.25*(1-c2-c2n+c2nm)/(1-c2);
  e.matrix[dim+2][dim+3]+=0.25*(s2+s2nm-s2n)/(1-c2);
  e.matrix[dim+3][dim+2]=e.matrix[dim+2][dim+3];
  e.matrix[dim+2][0]+=0.5*(s+snm-sn)/(1-c);
  e.matrix[0][dim+2]=e.matrix[dim+2][0];
  e.matrix[dim+3][dim+3]+=0.5*N+0.25*(1-c2-c2n+c2nm)/(1-c2);
  e.matrix[dim+3][0]+=0.5*(1-c-cn+cnm)/(1-c);
  e.matrix[0][dim+3]=e.matrix[dim+3][0];


 for(i=1;i<dim;i++)
    {
     if(i==1)
	tau=par->t[1];
      else
	tau=par->t[i]*par->t[1]/(par->t[i]+par->t[1]);
      l=1/tau;
      x=(exp(-l)*s+exp(-l*(N+1))*snm-exp(-l*N)*sn)/(1-2*exp(-l)*c+exp(-2.*l));
      e.matrix[i][dim+2]+=x;
      e.matrix[dim+2][i]+=x;
      x=(1-exp(-l)*c-exp(-l*N)*cn+exp(-l*(N+1))*cnm)/(1-2*exp(-l)*c+exp(-2.*l));
      e.matrix[i][dim+3]+=x;
      e.matrix[dim+3][i]+=x;
    }

  sn=sin(q*w);
  snm=sin((q-1)*w);
  s2n=sin(2*q*w);
  s2nm=sin(2*(q-1)*w);

  cn=cos(q*w);
  cnm=cos((q-1)*w);
  c2n=cos(2*q*w);
  c2nm=cos(2*(q-1)*w);

  e.matrix[dim+2][dim+2]-=0.5*q-0.25*(1-c2-c2n+c2nm)/(1-c2);
  e.matrix[dim+2][dim+3]-=0.25*(s2+s2nm-s2n)/(1-c2);
  e.matrix[dim+3][dim+2]=e.matrix[dim+2][dim+3];
  e.matrix[dim+2][0]-=0.5*(s+snm-sn)/(1-c);
  e.matrix[0][dim+2]=e.matrix[dim+2][0];
  e.matrix[dim+3][dim+3]-=0.5*q+0.25*(1-c2-c2n+c2nm)/(1-c2);
  e.matrix[dim+3][0]-=0.5*(1-c-cn+cnm)/(1-c);
  e.matrix[0][dim+3]=e.matrix[dim+3][0];
  
 for(i=1;i<dim;i++)
   {
     if(i==1)
       tau=par->t[1];
     else
       tau=par->t[i]*par->t[1]/(par->t[i]+par->t[1]);
     l=1./tau;
      x=(exp(-l)*s+exp(-l*(q+1))*snm-exp(-l*q)*sn)/(1-2*exp(-l)*c+exp(-2.*l));
      e.matrix[i][dim+2]-=x;
      e.matrix[dim+2][i]-=x;
      x=(1-exp(-l)*c-exp(-l*q)*cn+exp(-l*(q+1))*cnm)/(1-2*exp(-l)*c+exp(-2.*l));
      e.matrix[i][dim+3]-=x;
      e.matrix[dim+3][i]-=x;
    }
  for(i=q;i<N;i++)
    {
      e.vector[dim+2]+=waveform[i]*sin(w*i);
      e.vector[dim+3]+=waveform[i]*cos(w*i);
    }


  w=2*6.283185307/par->rf[0];
  s=sin(w);
  sn=sin(N*w);
  snm=sin((N-1)*w);
 
  c=cos(w);
  cn=cos(N*w);
  cnm=cos((N-1)*w);
 
  x=0.5*(s+snm-sn)/(1-c);//sum of sin2wt
  y=0.5*(1-c-cn+cnm)/(1-c);//sum of cos2wt

  e.matrix[dim][dim+2]+=0.5*y;
  e.matrix[dim][dim+3]-=0.5*x;
  e.matrix[dim+1][dim+2]+=0.5*x;
  e.matrix[dim+1][dim+3]+=0.5*y;

  w=4*6.283185307/par->rf[0];
  s=sin(w);
  sn=sin(N*w);
  snm=sin((N-1)*w);
 
  c=cos(w);
  cn=cos(N*w);
  cnm=cos((N-1)*w);
 
  x=0.5*(s+snm-sn)/(1-c);//sum of sin4wt
  y=0.5*(1-c-cn+cnm)/(1-c);//sum of cos4wt

  e.matrix[dim][dim+2]-=0.5*y;
  e.matrix[dim][dim+3]+=0.5*x;
  e.matrix[dim+1][dim+2]+=0.5*x;
  e.matrix[dim+1][dim+3]+=0.5*y;


  w=2*6.283185307/par->rf[0];
  s=sin(w);
  sn=sin(q*w);
  snm=sin((q-1)*w);
 
  c=cos(w);
  cn=cos(q*w);
  cnm=cos((q-1)*w);
 
  x=0.5*(s+snm-sn)/(1-c);//sum of sin2wt
  y=0.5*(1-c-cn+cnm)/(1-c);//sum of cos2wt

  e.matrix[dim][dim+2]-=0.5*y;
  e.matrix[dim][dim+3]+=0.5*x;
  e.matrix[dim+1][dim+2]-=0.5*x;
  e.matrix[dim+1][dim+3]-=0.5*y;

  w=4*6.283185307/par->rf[0];
  s=sin(w);
  sn=sin(q*w);
  snm=sin((q-1)*w);
 
  c=cos(w);
  cn=cos(q*w);
  cnm=cos((q-1)*w);
 
  x=0.5*(s+snm-sn)/(1-c);//sum of sin4wt
  y=0.5*(1-c-cn+cnm)/(1-c);//sum of cos4wt

  e.matrix[dim][dim+2]+=0.5*y;
  e.matrix[dim][dim+3]-=0.5*x;
  e.matrix[dim+1][dim+2]-=0.5*x;
  e.matrix[dim+1][dim+3]-=0.5*y;

  e.matrix[dim+2][dim]=e.matrix[dim][dim+2];
  e.matrix[dim+3][dim]=e.matrix[dim][dim+3];
  e.matrix[dim+2][dim+1]=e.matrix[dim+1][dim+2];
  e.matrix[dim+3][dim+1]=e.matrix[dim+1][dim+3];
  //end 3*wRF on the signal section 


  if(solve_lin_eq(&e)==0)
    	{
	  par->chisq=BADCHISQ_MAT;
	  par->ndf=1;
	  return BADCHISQ_MAT;
	}

  else
    {
      for(i=0;i<e.dim;i++)
	par->chisq-=e.solution[i]*e.vector[i];      
      
      if(par->chisq<0)
      	{
      	  par->chisq=BADCHISQ_NEG;
      	  par->ndf=1;
      	  return BADCHISQ_NEG;
      	}
      
      e.dim=dim;
      
      par->t[0]=get_t0(N,par,wpar,e);
      //if t0 is less than 0, return a T0FAIL
      if(par->t[0]<=0)
	{
	  par->chisq=BADCHISQ_T0;
	  par->ndf=1;
	  return BADCHISQ_T0;
	}

      par->am[0]=e.solution[0];
      
      for(i=1;i<dim;i++)
	{
	  if(i==1)
	    tau=par->t[1];
	  else
	    tau=par->t[i]*par->t[1]/(par->t[i]+par->t[1]);	  
	  par->am[i]=e.solution[i]*exp(-par->t[0]/tau);
	}
      
      for(i=2;i<dim;i++)
	par->am[i]*=-1;
  
      par->rf[1]=e.solution[dim];
      par->rf[2]=e.solution[dim+1];
      par->rf[3]=e.solution[dim+2];
      par->rf[4]=e.solution[dim+3];
    }
   
  for(i=0;i<dim;i++)
    if(par->am[i]<0)
      	{
  	  par->chisq=BADCHISQ_AMPL;
  	  par->ndf=1;
  	  return BADCHISQ_AMPL;
  	}
  
  return par->chisq;
}

/*================================================================*/
//this function displays the CsI waveform fit with RF on a ROOT canvas.
void display_CsI_Fit_with_RF(Int_t N,short* waveform,ShapePar *par, TApplication* theApp)
{
  TH1D *h=NULL,*g=NULL,*q=NULL;
  TCanvas *c=NULL;
   
  if((h=(TH1D*)gROOT->FindObject("Waveform"))==NULL)	
    h=new TH1D("Waveform","Waveform",N,0,N);
  else
    h->Reset();

 if((g=(TH1D*)gROOT->FindObject("Fit_with_RF"))==NULL)	
    g=new TH1D("Fit_with_RF","Fit_with_RF",N,0,N);
  else
    g->Reset();

 if((q=(TH1D*)gROOT->FindObject("Fit"))==NULL)	
    q=new TH1D("Fit","Fit",N,0,N);
  else
    q->Reset();

 c=(TCanvas*)gROOT->FindObject("WaveformFit");
 if(c!=NULL) delete c;
 c = new TCanvas("WaveformFit", "WaveformFit",10,10, 700, 500);

  for(Int_t i=0;i<N;i++)
    {
      h->Fill(i,waveform[i]);
      g->Fill(i,CsI_FitFunction_with_RF(i,par));
      q->Fill(i,CsI_FitFunction(i,par));
    }
  h->Draw();
  g->SetLineColor(kRed);
  g->Draw("same");
  q->SetLineColor(kBlack);
  q->Draw("same");
 
  theApp->Run(kTRUE);
}

/*================================================================*/
//two component fit function for CsI with RF
Double_t CsI_FitFunction_with_RF(Int_t i,ShapePar *par)
{
  Double_t  x,s,w,e;
  w=6.283185307/par->rf[0];

  x=i-par->t[0];
  e=exp(-x/par->t[1]);
  if(x<=0)
    {
      s=par->am[0];
      s+=par->rf[1]*sin(w*i);
      s+=par->rf[2]*cos(w*i);
      s+=par->rf[3]*sin(3*w*i);
      s+=par->rf[4]*cos(3*w*i);
    }
  else
    {   
      s=par->am[0];
      s+=par->am[2]*(1-exp(-x/par->t[2]))*e;
      s+=par->am[3]*(1-exp(-x/par->t[3]))*e;
      s+=par->am[4]*(1-exp(-x/par->t[4]))*e;
      s+=par->rf[1]*sin(w*i);
      s+=par->rf[2]*cos(w*i);
      s+=par->rf[3]*sin(3*w*i);
      s+=par->rf[4]*cos(3*w*i);
    }
  return s; 
}

/*================================================================*/
//displays CsI fit with RF with waveform and shape parameters
//use this function to display fit in external testing programs
void show_CsI_Fit_with_RF(Int_t N,short* waveform,ShapePar *par,WaveFormPar *wpar, TApplication* theApp)
{
  print_WavePar(wpar);
  print_ShapePar(par);
  display_CsI_Fit_with_RF(N,waveform,par,theApp);	
}

/*================================================================*/
//this function fits the waveform with two components first and then with
//a direct hit (tGamma = 12 samples). 
double  fit_CsI_waveform(int N, short *waveform,ShapePar* par,WaveFormPar* wpar)
{
  ShapePar *par_directHit; //formerly store_d
  long double chisq_directHit;
  int dim_directHit;
  int ndf;
  //int j;

  wpar->baseline_range=CSI_BASELINE_RANGE;

  get_exclusion_zone_for_CsI(N,waveform,wpar);

  //this loop should function as a normal fit.
  if(get_shape(4,N,waveform,par,wpar)>0)
    {
      par->type=1;
      //chi_s=0.;
      ndf=-4;

      //what are these next two loops doing?!?
      for(int i=0;i<wpar->temin;i++)
	{
	  ndf++;
	  chisq_directHit=waveform[i]-CsI_FitFunction(i,par);
	  chisq_directHit*=chisq_directHit;
	  //chi_s+=chisq_directHit;
	}
      for(int i=wpar->temax;i<N;i++)
	{
	  ndf++;
	  chisq_directHit=waveform[i]-CsI_FitFunction(i,par);
	  chisq_directHit*=chisq_directHit;
	  //chi_s+=chisq_directHit;
	}
      //to here

      //printf("chi comp %10.3Lf %10.3Lf\n",par->chisq,chi_s);
      //printf("ndf comp %10d %10d\n",par->ndf,ndf);
      //getc(stdin);

      return par->chisq;
    }

else
    {
      par->type=-1;
      
      // try a fit of a direct hit
      par_directHit=(ShapePar*)malloc(sizeof(ShapePar));
      memcpy(par_directHit,par,sizeof(ShapePar)); 
      dim_directHit=3;
      par_directHit->t[2]=par->t[4];
      par_directHit->t[4]=par->t[2];
      chisq_directHit=get_shape(dim_directHit,N,waveform,par_directHit,wpar);
     
      /* printf("chi_s %10.3f chisq_directHit %10.3f\n",chi_s,chisq_directHit); */
      /* getc(stdin); */

      if(chisq_directHit<0)
	return BADCHISQ_FAIL_DIRECT;
         
      else
	{
	  memcpy(par,par_directHit,sizeof(ShapePar));
	  par->t[2]=par_directHit->t[4];
	  par->am[2]=par_directHit->am[4];
	  par->t[4]=par_directHit->t[2];
	  par->am[4]=par_directHit->am[2]; 
	  par->type=2;
	  return par->chisq;
	}
    }
  
  return BADCHISQ_FAIL_DIRECT;
}

/*================================================================*/
//this function displays the CsI waveform fit on a ROOT canvas.
void display_CsI_exclusion_zone_and_fit(Int_t N,short* waveform,ShapePar *par, WaveFormPar *wpar, TApplication* theApp)
{
  TH1D *h=NULL,*g=NULL,*k=NULL;
  TF1 *b=NULL,*f=NULL,*baselineMin=NULL,*baselineMax=NULL;
  TCanvas *c=NULL;
   
  if((h=(TH1D*)gROOT->FindObject("Waveform"))==NULL)	
    h=new TH1D("Waveform","Waveform",N,0,N);
  else
    h->Reset();

  if((g=(TH1D*)gROOT->FindObject("Fit"))==NULL)	
    g=new TH1D("Fit","Fit",N,0,N);
  else
    g->Reset();
  
  if((k=(TH1D*)gROOT->FindObject("Fit2"))==NULL)
    k=new TH1D("Fit2","Fit2",N,0,N);
  else
    k->Reset();
  
 c=(TCanvas*)gROOT->FindObject("WaveformFit");
 if(c!=NULL) delete c;
 c = new TCanvas("WaveformFit", "WaveformFit",10,10, 700, 500);

//b is the baseline, baselineMin and baselineMax show the exclusion region defined by the baseline limits
 if(b!=NULL) delete b;
 b=new TF1("base","[0]",0,wpar->baseline_range);

 if(baselineMin!=NULL) delete baselineMin;
 baselineMin=new TF1("basemin","[0]",wpar->temin,wpar->temax);

if(baselineMax!=NULL) delete baselineMax;
 baselineMax=new TF1("basemax","[0]",wpar->temin,wpar->temax);
 
 //f is the fit function for the risetime
 if(f!=NULL) delete f;
 f=new TF1("fit","[0]*x+[1]",wpar->temin,wpar->temax+3*FILTER);

  for(Int_t i=0;i<N;i++)
    {
      h->Fill(i,waveform[i]);
      g->Fill(i,CsI_FitFunction(i,par));
    }

  //k is the waveform in the exclusion zone
  for(Int_t i=wpar->temin;i<wpar->temax;i++)
   k->Fill(i,waveform[i]);

  h->Draw();
  h->SetStats(0);
  g->SetLineColor(kRed);
  g->Draw("same");
  
  k->SetLineColor(kOrange);
  k->Draw("same");
  
  b->SetParameter(0,wpar->baseline);
  b->SetLineColor(kBlack);
  b->SetLineWidth(2);
  b->Draw("same");
  
  f->SetParameter(0,wpar->afit);
  f->SetParameter(1,wpar->bfit);
  f->SetLineColor(6);
  f->SetLineWidth(2);
  f->Draw("same");
  
  baselineMin->SetParameter(0,wpar->baselineMin);
  baselineMin->SetLineColor(kGreen);
  baselineMin->SetLineWidth(2);
  baselineMin->SetLineStyle(2);
  baselineMin->Draw("same");
  
  baselineMax->SetParameter(0,wpar->baselineMax);
  baselineMax->SetLineColor(kGreen);
  baselineMax->SetLineWidth(2);
  baselineMax->SetLineStyle(2);
  baselineMax->Draw("same");
  
  theApp->Run(kTRUE);
}

/*================================================================*/
//displays CsI fit with waveform and shape parameters
//use this function to display fit in external testing programs
void show_CsI_exclusion_zone_and_fit(Int_t N,short* waveform,ShapePar *par,WaveFormPar *wpar, TApplication* theApp)
{
  print_WavePar(wpar);
  print_ShapePar(par);
  display_CsI_exclusion_zone_and_fit(N,waveform,par,wpar,theApp);	
 
}

/*================================================================*/
//no function
//required for compilation of map.c
//placeholder for now
void  get_Griffin_t0(int N, short *waveform,WaveFormPar* wpar)
{
}
