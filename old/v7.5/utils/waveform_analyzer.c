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
  long double sum,tau;
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
      tau=par->t[i];
      sum=-((double)q)/tau+log(1.-exp(-((double)(d-q))/tau));
      sum-=log(1.-exp(-1./tau));
      e.matrix[i][0]=exp(sum);
      e.matrix[0][i]=exp(sum);
      
      tau=par->t[i]/2.;
      sum=-((double)q)/tau+log(1.-exp(-((double)(d-q))/tau));
      sum-=log(1.-exp(-1./tau));
      e.matrix[i][i]=exp(sum);
      for(j=i+1;j<e.dim;j++)
  	{
  	  tau=par->t[i]*par->t[j]/(par->t[i]+par->t[j]);
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
  
  for(i=1;i<e.dim;i++)
    {
      tau=par->t[i];
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
	  printf("No solution\n");
	  printf("*******Press enter");
	  getc(stdin);
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
  	par->am[i]=e.solution[i]*exp(-par->t[0]/par->t[i]);
      //done claculating amplitudes
 
      for(i=0;i<e.dim;i++)
  	par->chisq-=e.solution[i]*e.vector[i];

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

  par->type=dim-2;

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
	      if(slope>0.99)
	      	slope=0.99;

	      if(slope<0.01)
	      	slope=0.01;
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

    //set wpar->t0 here
    wpar->t0=tc;
    
    return tc;
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
  TH1D *h=NULL,*g=NULL, *k=NULL;
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
 if(c==NULL) {//delete c;
   c = new TCanvas("WaveformFit", "WaveformFit",10,10, 700, 500);
 }

  for(Int_t i=0;i<N;i++)
    {
      h->Fill(i,waveform[i]);
      g->Fill(i,CsI_FitFunction(i,par));
    }
  h->Draw();
  g->SetLineColor(kRed);
  g->Draw("same");
  c->Modified();
  c->Update();

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
  get_CsI_t0_local(N,waveform,par,wpar);
  display_CsI_Fit(N,waveform,par,theApp);	
 
}

/*================================================================*/
//fit waveforms with the RF included
int get_shape_with_RF(int dim, int N, short *waveform,ShapePar* par, WaveFormPar *wpar)
{
  
  long double tau,l;
  int i,j,p,q,d;
  lin_eq_type e;
  double s,sn,snm,s2,s2n,s2nm,c,cn,cnm,c2,c2n,c2nm,w,x,y;

  memset(&e,0,sizeof(lin_eq_type));

  if(dim!=4)
    {
      printf("currently working for CsI fast+slow fit only\n");
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
      tau=par->t[i];
      s=-((double)q)/tau+log(1.-exp(-((double)(d-q))/tau));
      s-=log(1.-exp(-1./tau));
      e.matrix[i][0]=exp(s);
      e.matrix[0][i]=exp(s);
      
      tau=par->t[i]/2.;
      s=-((double)q)/tau+log(1.-exp(-((double)(d-q))/tau));
      s-=log(1.-exp(-1./tau));
      e.matrix[i][i]=exp(s);
      for(j=i+1;j<dim;j++)
  	{
  	  tau=par->t[i]*par->t[j]/(par->t[i]+par->t[j]);
  	  s=-((double)q)/tau+log(1.-exp(-((double)(d-q))/tau));
  	  s-=log(1.-exp(-1./tau));
  	  e.matrix[i][j]=exp(s);
  	  e.matrix[j][i]=exp(s);
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
      tau=par->t[i];
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
      l=1/par->t[i];
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
      l=1/par->t[i];
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
      l=1/par->t[i];
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
      l=1/par->t[i];
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
	  printf("No solution of the linear equation\n");
	  par->chisq=BADCHISQ_MAT;
	  par->ndf=1;
	  return BADCHISQ_MAT;
	}
  else
    {
      for(i=0;i<e.dim;i++)
	par->chisq-=(e.solution[i]*e.vector[i]);

      /* if(par->chisq<0) */
      /* 	{ */
      /* 	  par->chisq=BADCHISQ; */
      /* 	  par->ndf=1; */
      /* 	  return BADCHISQ; */
      /* 	} */

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
  	par->am[i]=e.solution[i]*exp(-par->t[0]/par->t[i]);

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
  par->type=dim-2;
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
  ShapePar *par_directHit, *par_slowAmp; //formerly store_d, store_s
  long double chisq_directHit, chisq_slowAmp;
  int dim_directHit, dim_slowAmp;
  int ndf;
  //int j;

  wpar->baseline_range=CSI_BASELINE_RANGE;

  get_exclusion_zone_for_CsI(N,waveform,wpar);

  //this loop should function as a normal fit.
  if(get_shape(4,N,waveform,par,wpar)>0)
    {
      par->type=1;
      //chi_directHit=0.;
      chisq_slowAmp=0.;
      ndf=-4;

      //what are these next two loops doing?!?
      for(int i=0;i<wpar->temin;i++)
	{
	  ndf++;
	  chisq_directHit=waveform[i]-CsI_FitFunction(i,par);
	  chisq_directHit*=chisq_directHit;
	  chisq_slowAmp+=chisq_directHit;
	}
      for(int i=wpar->temax;i<N;i++)
	{
	  ndf++;
	  chisq_directHit=waveform[i]-CsI_FitFunction(i,par);
	  chisq_directHit*=chisq_directHit;
	  chisq_slowAmp+=chisq_directHit;
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
      
      // try a fit with the slow component only
      par_slowAmp=(ShapePar*)malloc(sizeof(ShapePar));
      memcpy(par_slowAmp,par,sizeof(ShapePar));
      dim_slowAmp=3;
      //par_slowAmp->t[2]=par->t[3];
      //par_slowAmp->t[3]=par->t[2];
      chisq_slowAmp=get_shape(dim_slowAmp,N,waveform,par_slowAmp,wpar);

      // try a fit of a direct hit
      par_directHit=(ShapePar*)malloc(sizeof(ShapePar));
      memcpy(par_directHit,par,sizeof(ShapePar)); 
      dim_directHit=3;
      par_directHit->t[2]=par->t[4];
      par_directHit->t[4]=par->t[2];
      chisq_directHit=get_shape(dim_directHit,N,waveform,par_directHit,wpar);
     
      /* printf("chi_s %10.3f chisq_directHit %10.3f\n",chi_s,chisq_directHit); */
      /* getc(stdin); */

      if( (chisq_slowAmp<=0) && (chisq_directHit<=0) )
	return BADCHISQ_FAIL_DIRECT;
      
      if( (chisq_slowAmp>0) && (chisq_directHit<=0) )
	{
	  memcpy(par,par_slowAmp,sizeof(ShapePar));
	  //par->t[2]=par_directHit->t[3];
	  //par->am[2]=par_directHit->am[3];
	  //par->t[3]=par_directHit->t[2];
	  //par->am[3]=par_directHit->am[2]; 
	  par->type=3;
	  return par->chisq;
	}
   
      if( (chisq_slowAmp<=0) && (chisq_directHit>0) )
	{
	  memcpy(par,par_directHit,sizeof(ShapePar));
	  par->t[2]=par_directHit->t[4];
	  par->am[2]=par_directHit->am[4];
	  par->t[4]=par_directHit->t[2];
	  par->am[4]=par_directHit->am[2]; 
	  par->type=2; //gamma only par type = 2
	  return par->chisq;
	}

      if( (chisq_slowAmp>0) && (chisq_directHit>0) )
	{
	  if(chisq_directHit<chisq_slowAmp)
	    {
	      memcpy(par,par_directHit,sizeof(ShapePar));
	      par->t[2]=par_directHit->t[4];
	      par->am[2]=par_directHit->am[4];
	      par->t[4]=par_directHit->t[2];
	      par->am[4]=par_directHit->am[2]; 
	      par->type=2; //gamma only par type = 2
	      return par->chisq;
	    }
	  
	  else
	    {
	      memcpy(par,par_slowAmp,sizeof(ShapePar)); 
	      //par->t[2]=par_directHit->t[3];
	      //par->am[2]=par_directHit->am[3];
	      //par->t[3]=par_directHit->t[2];
	      //par->am[3]=par_directHit->am[2]; 
	      par->type=3;
  	      return par->chisq;
	    }
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

/*================================================================*/
/*            NEW STUFF FOR KRIS' PIN DIODE PID ALGORITHM         */
/*================================================================*/
int fit_smooth_parabola(short* waveform,int low, int high, double x0, ParPar* pp)
{
  int i,ndf,k;
  lin_eq_type e;
  double chisq;
  double x;
  memset(&e,0,sizeof(lin_eq_type));
  memset(pp,0,sizeof(ParPar));
  e.dim=2;
  chisq=0.;
  ndf=0;
  k=(int)rint(x0);

  for(i=low;i<k;i++)
    {
      e.matrix[0][0]+=1;
      e.vector[0]+=waveform[i];
      ndf++;
      chisq+=waveform[i]*waveform[i];
    }

  for(i=k;i<high;i++)
    {
      x=(i-x0)*(i-x0);
      e.matrix[0][0]+=1;
      e.matrix[0][1]+=x;
      e.matrix[1][1]+=x*x;
      e.vector[0]+=waveform[i];
      e.vector[1]+=waveform[i]*x;
      ndf++;
      chisq+=waveform[i]*waveform[i];
    }
  e.matrix[1][0]=e.matrix[0][1];
 
 
  if(solve_lin_eq(&e)==0)
    {
      pp->chisq=LARGECHISQ;
      return -1;
    }
  else
    {
      chisq-=e.vector[0]*e.solution[0];
      chisq-=e.vector[1]*e.solution[1];
    
      pp->constant=e.solution[0];
      pp->linear=0.;
      pp->quadratic=e.solution[1];
      pp->chisq=chisq;
      pp->ndf=ndf;
      
      return 1;
    }

  return -1;
}
/*================================================================*/
int fit_parabola(short* waveform,int low, int high,ParPar* pp)
{
  int i,ndf;
  lin_eq_type e;
  double chisq;
  memset(&e,0,sizeof(lin_eq_type));
  memset(pp,0,sizeof(ParPar));
  e.dim=3;
  chisq=0.;
  ndf=0;
  for(i=low;i<high;i++)
    {
      e.matrix[0][0]+=1;
      e.matrix[0][1]+=i;
      e.matrix[0][2]+=i*i;
      e.matrix[1][2]+=i*i*i;
      e.matrix[2][2]+=i*i*i*i;
      e.vector[0]+=waveform[i];
      e.vector[1]+=waveform[i]*i;
      e.vector[2]+=waveform[i]*i*i;
      ndf++;
      chisq+=waveform[i]*waveform[i];
    }
  e.matrix[1][0]=e.matrix[0][1];
  e.matrix[1][1]=e.matrix[0][2];
  e.matrix[2][0]=e.matrix[0][2];
  e.matrix[2][1]=e.matrix[1][2];
 
  if(solve_lin_eq(&e)==0)
    {
      pp->chisq=LARGECHISQ;
      return -1;
    }
  else
    {
      chisq-=e.vector[0]*e.solution[0];
      chisq-=e.vector[1]*e.solution[1];
      chisq-=e.vector[2]*e.solution[2];
      pp->constant=e.solution[0];
      pp->linear=e.solution[1];
      pp->quadratic=e.solution[2];
      pp->chisq=chisq;
      pp->ndf=ndf;
      return 1;
    }
}
/*================================================================*/
int fit_line(short* waveform,int low, int high,LinePar* lp)
{
  int i,ndf;
  lin_eq_type e;
  double chisq;
  memset(&e,0,sizeof(lin_eq_type));
  memset(lp,0,sizeof(LinePar));
  e.dim=2;
  chisq=0.;
  ndf=0;
  for(i=low;i<high;i++)
    {
      e.matrix[0][0]+=1;
      e.matrix[0][1]+=i;
      e.matrix[1][1]+=i*i;
      e.vector[0]+=waveform[i];
      e.vector[1]+=waveform[i]*i;
      ndf++;
      chisq+=waveform[i]*waveform[i];
    }
  e.matrix[1][0]=e.matrix[0][1];
 
  if(solve_lin_eq(&e)==0)
    {
      lp->chisq=LARGECHISQ;
      return -1;
    }
  else
    {
      chisq-=e.vector[0]*e.solution[0];
      chisq-=e.vector[1]*e.solution[1];
      lp->slope=e.solution[1];
      lp->intercept=e.solution[0];
      lp->chisq=chisq;
      lp->ndf=ndf;
      return 1;
    }
}
/*================================================================*/
double  get_linear_T0(int N, short* waveform, WaveFormPar* wpar)
{

  LinePar lp,lpl;
  int k,kmin;
  double chit,chitmin;
  double b,c,t;
 
 
  chitmin=LARGECHISQ;
  kmin=0;
  for(k=T0RANGE/2;k<wpar->thigh-T0RANGE/2;k++)
    {
      //fit line to the baseline
      fit_line(waveform,0,k,&lp);
      
      //fit line to the risetime
      fit_line(waveform,k,wpar->thigh,&lpl);
      
      
      chit=lp.chisq+lpl.chisq;
	  
      if(chit<chitmin)
	{
	  chitmin=chit;
	  wpar->b0=lp.intercept;
	  wpar->b1=lp.slope;
	  wpar->s0=lpl.intercept;
	  wpar->s1=lpl.slope;
	  wpar->s2=0.;
	  kmin=k;
	}
    }	// end of the loop over k
  
  b=wpar->s1-wpar->b1;
  c=wpar->s0-wpar->b0;
  t=-c/b;

  wpar->t0=-1;
  wpar->temin=0;
  wpar->temax=wpar->thigh;
  if(t>0)
    if(t<N)
      {
	wpar->t0=t;
	wpar->temin=wpar->t0-2;
	wpar->temax=wpar->t0+2;
      }
  
  return (double)(chitmin/(wpar->thigh-5));

}
/*================================================================*/
double  get_smooth_T0(int N, short* waveform, WaveFormPar* wpar)
{

  ParPar pp,ppmin;
  int k,kmin;
  double chit,chitmin;
  double c,t;
 
 
  chitmin=LARGECHISQ;
  kmin=0;
  //corse search first
  for(k=T0RANGE/2;k<wpar->thigh-T0RANGE/2;k++)
    {
      fit_smooth_parabola(waveform,0,wpar->thigh,(double)k,&pp);
      chit=pp.chisq;
      if(chit<chitmin)
	{
	  chitmin=chit;
	  kmin=k;
	}
    }	// end of the corse search loop over k
 
  c=kmin;
  chitmin=LARGECHISQ;
  //fine search next
  for(t=kmin-1;t<kmin+1;t+=0.1)
    {
      fit_smooth_parabola(waveform,0,wpar->thigh,t,&pp);
      chit=pp.chisq;
      if(chit<chitmin)
	{
      	  memcpy(&ppmin,&pp,sizeof(ParPar));
	  chitmin=chit;
	  c=t;
	}
    }	// end of the corse search loop over k
 

  memcpy(&pp,&ppmin,sizeof(ParPar));
  t=c;
  wpar->s0=pp.constant+pp.quadratic*t*t;
  wpar->s1=-2.*pp.quadratic*t;
  wpar->s2=pp.quadratic;
  wpar->b0=pp.constant;
  wpar->b1=0.;

  wpar->t0=-1;
  wpar->temin=0;
  wpar->temax=wpar->thigh;
  if(t>0)
    if(t<N)
      {
	wpar->t0=t;
	wpar->temin=(int)rint(wpar->t0);
	wpar->temax=(int)rint(wpar->t0);
	return (double)(chitmin/(wpar->thigh-2));
      } 
  return LARGECHISQ;

}
/*================================================================*/
double  get_parabolic_T0(int N, short* waveform, WaveFormPar* wpar)
{

  LinePar lp;
  ParPar pp;
  int k,kmin;
  double chit,chitmin;
  double a,b,c,d,t;
 
  chitmin=LARGECHISQ;
  kmin=0;
  for(k=T0RANGE/2;k<wpar->thigh-T0RANGE/2;k++)
    {
      //fit line to the baseline
      fit_line(waveform,0,k,&lp);
      
      //fit parabola to the risetime
      fit_parabola(waveform,k,wpar->thigh,&pp);
      
      chit=lp.chisq+pp.chisq;
      
      if(chit<chitmin)
	{
	  chitmin=chit;
	  wpar->b0=lp.intercept;
	  wpar->b1=lp.slope;
	  wpar->s0=pp.constant;
	  wpar->s1=pp.linear;
	  wpar->s2=pp.quadratic;
	  kmin=k;
	}
    }//end loop through k
  
  a=wpar->s2;
  b=wpar->s1-wpar->b1;
  c=wpar->s0-wpar->b0;
  d=b*b-4*a*c;
  

  t=-1.;
  if(a==0.)
    t=-c/b;
  else
    {
      if(d>=0)
	{
	  if(d==0.)
	    t=-0.5*b/a;
	  else
	    {
	      d=sqrt(d);
	      t=0.5*(-b+d)/a;
	    }
	}
      else
	{
	  return LARGECHISQ;
	}
    }

  wpar->t0=-1;
  wpar->temin=0;
  wpar->temax=wpar->thigh;
  if(t>0)
    if(t<N)
      {
	wpar->t0=t;
	wpar->temin=wpar->t0-2;
	wpar->temax=wpar->t0+2;
	return (double)(chitmin/(wpar->thigh-5));
      } 
  return LARGECHISQ;
 	
}
/*================================================================*/
double  fit_newT0(int N, short* waveform, WaveFormPar* wpar)
{
  double chisq[3],chimin;
  WaveFormPar w[3];
  size_t swp;
  int  i,imin;

  swp=sizeof(WaveFormPar);

  wpar->baseline_range=T0RANGE;
  get_baseline(wpar->baseline_range,waveform,wpar);
  get_tmax(N,waveform,wpar); 
  if(wpar->max>=16380)
    return LARGECHISQ;

  if(wpar->tmax<PIN_BASELINE_RANGE)
    return LARGECHISQ;

  get_t30(N,waveform,wpar);
  get_t50(N,waveform,wpar);
  wpar->thigh=wpar->t30;

  for(i=0;i<3;i++)
    chisq[i]=LARGECHISQ;
  memcpy(&w[0],wpar,swp);
  chisq[0]=get_smooth_T0(N,waveform,&w[0]);
  memcpy(&w[1],wpar,swp);
  chisq[1]=get_parabolic_T0(N,waveform,&w[1]);
  memcpy(&w[2],wpar,swp);
  chisq[2]=get_linear_T0(N,waveform,&w[2]);

  chimin=LARGECHISQ;
  imin=0;
  for(i=0;i<3;i++)
    if(chisq[i]<chimin)
      {
	chimin=chisq[i];
	imin=i;
      }
  /* for(i=0;i<3;i++) */
  /*   printf("   i %2d %10.2f\n",i,chisq[i]);  */
  

  memcpy(wpar,&w[imin],swp);

  // printf("min i %2d %10.2f b0 %10.2f\n",imin,chisq[imin],wpar->b0); 
 
  return chisq[imin];
 
}
/*================================================================*/
void  display_newT0_fit(int N, short* waveform, WaveFormPar* wpar,TApplication* theApp)
{
  TH1D *h=new TH1D("Waveform","Waveform",N,0,N); 
  h->Reset();
  for(Int_t i=0;i<N;i++)
    h->Fill(i,waveform[i]);

  TCanvas *c=(TCanvas*)gROOT->FindObject("WaveformFit");
  if(c!=NULL) delete c;
  c = new TCanvas("WaveformFit", "WaveformFit",10,10, 700, 500);
  
  h->Draw();
  
  
  TH1D* g=new TH1D("FitB","FitB",N,0,N);
  
  for(Int_t i=0;i<wpar->temax;i++)
    g->Fill(i,wpar->b0+wpar->b1*i);
  
  TH1D *f=new TH1D("FitR","FitR",N,0,N);
  
  for(Int_t i=wpar->temin;i<wpar->thigh;i++)
    f->Fill(i,wpar->s0+wpar->s1*i+wpar->s2*i*i);
  
  g->SetLineColor(kRed);
  g->Draw("same");
  f->SetLineColor(kBlue);
  f->Draw("same");
  
  theApp->Run(kTRUE); 	      
  h->Delete();
  f->Delete();
  g->Delete();
}
/*================================================================*/
Double_t OneComponentFitFunction(Double_t *y, Double_t *par)
{
 
  Double_t x; 
  x=y[0]-par[1];
  if(x<=0) 
    return par[0];
  else
    return par[0]+par[2]*(1-exp(-x/par[4]))*exp(-x/par[3]);

}
/*================================================================*/
Double_t OneComponentPlotFunction(Double_t y, ShapePar *par)
{
 
  Double_t x; 
  x=y-par->t[0];
  if(x<=0) 
    return par->am[0];
  else
    return par->am[0]+par->am[2]*(1-exp(-x/par->t[2]))*exp(-x/par->t[1]);

}
/*================================================================*/
Double_t OneComponentChiSq(int N, short *waveform,ShapePar* par)
{
  long double chisq,x;
  int ndf;
  
  ndf=0;
  chisq=0.;
  for(ndf=0;ndf<N;ndf++)
    {
      x=waveform[ndf]-OneComponentPlotFunction((Double_t)ndf,par);
      chisq+=x*x;
    }
  ndf-=4;
  par->chisq=chisq;
  par->ndf=ndf;
  par->chisq_f=chisq;
  par->ndf_f=ndf;
  par->chisq_ex=0;
  par->ndf_ex=-1;
  par->type=11;
  return (Double_t)chisq;
}
/*================================================================*/
Double_t OneComponentFitWaveform(int N, short *waveform,ShapePar* par,WaveFormPar* wpar)
{

  TH1D *h;  
  TF1 *f;

  memset(wpar,0,sizeof(WaveFormPar));
  wpar->baseline_range=PIN_BASELINE_RANGE;
  get_baseline(wpar->baseline_range,waveform,wpar);
  get_tmax(N,waveform,wpar); 
  get_t10(N,waveform,wpar);
  get_t30(N,waveform,wpar);
  get_t90(N,waveform,wpar);

  f=new TF1("OneComponentFit",OneComponentFitFunction,0,MAX_SAMPLES,5);  
  f->SetParName(0,"C");
  f->SetParName(1,"t0");
  f->SetParName(2,"A");
  f->SetParName(3,"TRC");
  f->SetParName(4,"T");
  
  f->SetParameter(0,wpar->baseline);
  f->SetParameter(1,0.5*(3.*wpar->t10-wpar->t30));
  f->SetParameter(2,wpar->max-wpar->baseline);
  f->SetParameter(4,0.45512*(wpar->t90-wpar->t10));
  f->FixParameter(3,par->t[1]);

  h=new TH1D("Wf","Wf",N,0,N); 
  h->Reset(); 
  for(Int_t i=0;i<N;i++)
    h->Fill(i,waveform[i]); 
  h->Fit("OneComponentFit","WQ","WQ",0,N-1);
  
  par->am[0]=f->GetParameter(0);
  par->am[1]=f->GetParameter(2);
  par->am[2]=f->GetParameter(2);
  par->t[0]=f->GetParameter(1);
  par->t[1]=f->GetParameter(3);
  par->t[2]=f->GetParameter(4);
  h->Delete();
  f->Delete();
  return OneComponentChiSq(N, waveform, par);
}

/*===========================================================*/
double get_tfrac(int N, short *waveform,WaveFormPar *wpar,double frac,double fraclow, double frachigh)
{
  int t;
  double f,flow,fhigh;
  lin_eq_type e;
  int i,imax,imin;
  long long int a;
  double p,q,r,d;

  if(wpar->bflag!=1)
    {
      printf("Baseline not deterimned for the tfraction\n");
      exit(1);
    }

  if(wpar->mflag!=1)
    {
      printf("Maximum not deterimned for the tfraction\n");
      exit(1);
    }
  t=wpar->tmax;
  f=wpar->baseline+frac*(wpar->max-wpar->baseline);
  flow=wpar->baseline+fraclow*(wpar->max-wpar->baseline);
  fhigh=wpar->baseline+frachigh*(wpar->max-wpar->baseline);

  while(waveform[t]>f)
    {
      t--;
      if(t<=4) break;
    }
  imin=t;
  while(waveform[imin]>flow)
    {
      imin--;
      if(imin<=1) break;
    }
  
  imax=t;

   while(waveform[imax]<fhigh)
    {
      imax++;
      if(imax>=N-1) break;
    }

  e.dim=3;

  i=imax-imin;
  a=i;
  e.matrix[0][0]=a+1;
  e.matrix[0][1]=0.5*a;
  e.matrix[2][0]=a/6.;
  e.matrix[2][2]=-a/30.;
  a*=i;
  e.matrix[0][1]+=0.5*a;
  e.matrix[2][0]+=0.5*a;
  e.matrix[2][1]=0.25*a;
  a*=i;
  e.matrix[2][0]+=a/3.;
  e.matrix[2][1]+=0.5*a;
  e.matrix[2][2]+=a/3.;
  a*=i;
  e.matrix[2][1]+=0.25*a;
  e.matrix[2][2]+=0.5*a;
  a*=i;
  e.matrix[2][2]+=0.2*a;

  e.matrix[1][0]=e.matrix[0][1];
  e.matrix[1][1]=e.matrix[2][0];
  e.matrix[0][2]=e.matrix[2][0];
  e.matrix[1][2]=e.matrix[2][1];

 for(i=0;i<e.dim;i++)
    e.vector[i]=0;

  for(i=imin;i<imax+1;i++)
    {
      a=i-imin;
      e.vector[0]+=waveform[i];
      e.vector[1]+=waveform[i]*a;
      e.vector[2]+=waveform[i]*a*a;
    }
     

    if(solve_lin_eq(&e)==0)
      return -4;
    else
      {
    	p=e.solution[0]-f;
    	q=e.solution[1];
    	r=e.solution[2];

    	if(r!=0)
    	  {
    	    d=q*q-4*r*p;
    	    if(d<0)
		return -5;
    	    else
    	      {

    		f=-q+sqrt(d);
    		f*=0.5;
    		f/=r;
    		f+=imin;
	      
    		return f;
    	      }
    	  }
    	else
    	  {
    	    if(q!=0)
    	      {
    		f=-p/q;
    		return f;
    	      }
    	    else
    	      return -6;
    	  }
      }
    return -7;
}

/* ==================================================== */
void get_t50(int N, short *waveform,WaveFormPar *wpar)
{
  int t;
  
  t=get_tfrac( N, waveform, wpar,0.5,0.3,0.8);

  if((t>0)&&(t<MAX_SAMPLES))
    {
      wpar->t50_flag=1;
      wpar->t50=t;
    }
  else
    {
      wpar->t50_flag=-1;
      wpar->t50=-1;
    }
}
/* ==================================================== */
void get_t90(int N, short *waveform,WaveFormPar *wpar)
{
  int t;
  
  t=get_tfrac( N, waveform, wpar,0.9,0.8,0.98);
    
  if((t>0)&&(t<MAX_SAMPLES))
    {
      wpar->t90_flag=1;
      wpar->t90=t;
    }
  else
    {
      wpar->t90_flag=-1;
      wpar->t90=-1;
    }
}
/*===========================================================*/
void get_t10(int N, short *waveform,WaveFormPar *wpar)
{
  int t;

  t=get_tfrac( N, waveform, wpar,0.1,0.05,0.2);

  if((t>0)&&(t<MAX_SAMPLES))
    {
      wpar->t10_flag=1;
      wpar->t10=t;
    }
  else
    {
      wpar->t10_flag=-1;
      wpar->t10=-1;
    }
}
/*===========================================================*/
void get_t30(int N, short *waveform,WaveFormPar *wpar)
{
  int t;

  t=get_tfrac( N, waveform, wpar,0.3,0.15,0.45);
  if((t>0)&&(t<MAX_SAMPLES))
    {
      wpar->t30_flag=1;
      wpar->t30=t;
    }
  else
    {
      wpar->t30_flag=-1;
      wpar->t30=-1;
    }
}

/*===========================================================*/
void  get_CsI_t0_local(int N, short *waveform,ShapePar* par,WaveFormPar* wpar)
{
  int i,tlmin,tlmax;
  lin_eq_type e;
  double b,c,t;

  wpar->t0_local=-1.;

  get_t30(N,waveform,wpar);
  if(wpar->t30_flag==1)
    tlmax=(int)ceil(wpar->t30);
  else
    return;

  //use t0 from the amplitude fit
  tlmin=(int)ceil(par->t[0]+2);
  wpar->baseline_range=(int)ceil(par->t[0]-10);

  //not enough baseline
  if(wpar->baseline_range<10)
    return;
  //fit line to the baseline

  memset(&e,0,sizeof(lin_eq_type));
  e.dim=2;
  for(i=0;i<=wpar->baseline_range;i++)
    {
      e.matrix[0][0]+=1;
      e.matrix[0][1]+=i;
      e.matrix[1][1]+=i*i;
      e.vector[0]+=waveform[i];
      e.vector[1]+=waveform[i]*i;
    }
  e.matrix[1][0]=e.matrix[0][1];
 
  if(solve_lin_eq(&e)==0)
      return;
  else
    {
      wpar->b0=e.solution[0];
      wpar->b1=e.solution[1];
    }


  // if parabola fails try a line
  //  if(d<0)
    {
      memset(&e,0,sizeof(lin_eq_type));
      e.dim=2;
      for(i=tlmin;i<=tlmax;i++)
	{
	  e.matrix[0][0]+=1;
	  e.matrix[0][1]+=i;
	  e.matrix[1][1]+=i*i;
	  e.vector[0]+=waveform[i];
	  e.vector[1]+=waveform[i]*i;
	}
      e.matrix[1][0]=e.matrix[0][1];
      
      if(solve_lin_eq(&e)==0)
	return;
      else
	{
	  wpar->s0=e.solution[0];
	  wpar->s1=e.solution[1];
	  wpar->s2=0;
	}
      b=wpar->s1-wpar->b1;
      c=wpar->s0-wpar->b0;
      t=-c/b;
    }

  if(t>=0)
    if(t<N)
      {
	wpar->t0_local=t;
	return;
      }
}

/*===========================================================*/
void get_t10t90(int N, short *waveform,WaveFormPar *wpar)
{
  get_sig2noise(N,waveform,wpar);

  if(wpar->sig2noise>SIGN2NOISE)
    {
      get_t50(N,waveform,wpar);
      if(wpar->t50_flag==1)
	{
	  if(wpar->t50>T50LOW)
	    {
	      if(wpar->t50<T50HIGH)
		{
		  get_t90(N,waveform,wpar);
		  if(wpar->t90_flag==1)
		    {
		      if(wpar->t90>T90LOW)
			{
			  if(wpar->t90<T90HIGH)
			    {
			      get_t10(N,waveform,wpar);
			      if(wpar->t10_flag==1)
				{
				  if(wpar->t10>T10LOW)
				    {
				      if(wpar->t10<T10HIGH)
					{
					  wpar->t10t90_flag=1;
					  wpar->t10t90=wpar->t90-wpar->t10;
					}
				      else
					wpar->t10t90_flag=-10;
				    }
				  else
				    wpar->t10t90_flag=-9;
				}
			      else
				wpar->t10t90_flag=-8;
			    }
			  else 
			    wpar->t10t90_flag=-7;
			}
		      else
			wpar->t10t90_flag=-6;
		    }
		  else
		    wpar->t10t90_flag=-5;
		}
	      else
		wpar->t10t90_flag=-4;
	    }
	  else
	    wpar->t10t90_flag=-3;
	}
      else
	wpar->t10t90_flag=-2;
    }
  else
    wpar->t10t90_flag=-1;
}

/*======================================================*/
void get_sig2noise(int N, short *waveform,WaveFormPar *wpar)
{
  get_baseline(wpar->baseline_range,waveform,wpar);
  get_tmax(N,waveform,wpar);
  wpar->sig2noise=(wpar->max-wpar->baseline)/wpar->baselineStDev;
 
}
