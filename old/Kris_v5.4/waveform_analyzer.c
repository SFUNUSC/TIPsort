#include "waveform_analyzer.h"
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

  wpar->t00=-1;
  wpar->temin=0;
  wpar->temax=wpar->thigh;
  if(t>0)
    if(t<N)
      {
	wpar->t00=t;
	wpar->temin=wpar->t00-2;
	wpar->temax=wpar->t00+2;
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

  wpar->t00=-1;
  wpar->temin=0;
  wpar->temax=wpar->thigh;
  if(t>0)
    if(t<N)
      {
	wpar->t00=t;
	wpar->temin=(int)rint(wpar->t00);
	wpar->temax=(int)rint(wpar->t00);
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

  wpar->t00=-1;
  wpar->temin=0;
  wpar->temax=wpar->thigh;
  if(t>0)
    if(t<N)
      {
	wpar->t00=t;
	wpar->temin=wpar->t00-2;
	wpar->temax=wpar->t00+2;
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
  
  f->SetParameter(0,wpar->b);
  f->SetParameter(1,0.5*(3.*wpar->t10-wpar->t30));
  f->SetParameter(2,wpar->max-wpar->b);
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

/*================================================================*/
double CsI_get_tau(int i,ShapePar *par)
{
  if(i==1)
   return par->t[1];

  if(i>=2)
    if(i<NSHAPE)
      return par->t[i]*par->t[1]/(par->t[i]+par->t[1]);

  return -1.;
}
/*================================================================*/
void CsI_ShowFit(Int_t N,short* waveform,ShapePar *par,WaveFormPar *wpar, TApplication* theApp)
{
  print_WavePar(wpar);
  print_ShapePar(par);
  CsI_DisplayFit(N,waveform,par,theApp);	
 
}
/*================================================================*/
void CsI_ShowFitWithRF(Int_t N,short* waveform,ShapePar *par,WaveFormPar *wpar, TApplication* theApp)
{
  print_WavePar(wpar);
  print_ShapePar(par);
  CsI_DisplayFitWithRF(N,waveform,par,theApp);	
 
}
/*================================================================*/
void CsI_DisplayFit(Int_t N,short* waveform,ShapePar *par, TApplication* theApp)
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
void CsI_show_exclusion_zone(Int_t N,short* waveform,WaveFormPar *wpar,TApplication* theApp)
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
      h->Fill(i,waveform[i]);

  for(Int_t i=wpar->temin;i<wpar->temax;i++)
    g->Fill(i,waveform[i]);


  h->Draw();
  g->SetLineColor(kRed);
  g->Draw("same");


  theApp->Run(kTRUE);
}
/*================================================================*/
void display_waveform(Int_t N,short* waveform,TApplication* theApp)
{

  TH1D *h=NULL;
  TCanvas *c=NULL;
  
 
  if((h=(TH1D*)gROOT->FindObject("Waveform"))==NULL)	
    h=new TH1D("Waveform","Waveform",N,0,N);
  else
    h->Reset();

 c=(TCanvas*)gROOT->FindObject("WaveformDisplay");
 if(c!=NULL) delete c;
 c = new TCanvas("WaveformDispla", "WaveformDislpay",10,10, 700, 500);

  for(Int_t i=0;i<N;i++)
    {
      h->Fill(i,waveform[i]);
     }
  h->Draw();

  theApp->Run(kTRUE);
}
/*================================================================*/
void CsI_DisplayFitWithRF(Int_t N,short* waveform,ShapePar *par, TApplication* theApp)
{

  TH1D *h=NULL,*g=NULL,*q=NULL;
  TCanvas *c=NULL;
  
 
  if((h=(TH1D*)gROOT->FindObject("Waveform"))==NULL)	
    h=new TH1D("Waveform","Waveform",N,0,N);
  else
    h->Reset();

 if((g=(TH1D*)gROOT->FindObject("FitRF"))==NULL)	
    g=new TH1D("FitRF","FitRF",N,0,N);
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
      g->Fill(i,CsI_FitFunctionWithRF(i,par));
      q->Fill(i,CsI_FitFunction(i,par));
    }
  h->Draw();
  g->SetLineColor(kRed);
  g->Draw("same");
  q->SetLineColor(kBlue);
  q->Draw("same");
 
  theApp->Run(kTRUE);
}
/*================================================================*/
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
Double_t CsI_FitFunctionWithRF(Int_t i,ShapePar *par)
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
/*======================================================*/
void print_WavePar(WaveFormPar *wpar)
{
  printf("== Currently established waveform parameters ============\n");
  printf(" b    : %10.2f\n",wpar->b);
  printf(" bsq  : %10.2f\n",wpar->bsq);
  printf(" max  : %10.2f\n",(double)wpar->max);
  printf("tmax  : %10.2f\n",(double)wpar->tmax);
  printf("temin : %10.2f\n",(double)wpar->temin);
  printf("temax : %10.2f\n",(double)wpar->temax);
  printf("thigh : %10.2f\n",(double)wpar->thigh);
  printf("  t00 : %10.2f\n",(double)wpar->t00);
  printf("  t30 : %10.2f\n",(double)wpar->t30);
  printf("  t50 : %10.2f\n",(double)wpar->t50);
  printf("  t90 : %10.2f\n",(double)wpar->t90);
  printf("   b0 : %10.2f\n",(double)wpar->b0);
  printf("   b1 : %10.2f\n",(double)wpar->b1);
  printf("   s0 : %10.2f\n",(double)wpar->s0);
  printf("   s1 : %10.2f\n",(double)wpar->s1);
  printf("   s2 : %10.2f\n",(double)wpar->s2);
}
/*======================================================*/
void print_ShapePar(ShapePar *par)
{
  printf("== Shape parameters from the fit ========================\n");
  printf("   Fit type : %12d \n",par->type);	
  printf("   NDF      : %12d \n",par->ndf);	
  printf("   Chisq    : %15.2Lf\n",par->chisq);	
  printf("   Chisq/NDF: %15.2Lf\n",par->chisq/par->ndf);
  printf("Ex NDF      : %12d \n",par->ndf_ex);	
  printf("Ex Chisq    : %15.2Lf\n",par->chisq_ex);	
  printf("Ex Chisq/NDF: %15.2Lf\n",par->chisq_ex/par->ndf_ex);		
  for(int i=0;i<5;i++)
    printf("Id %1d Amp. %10.3f  T %10.3f  RF %10.3f\n",i,(double)par->am[i],(double)par->t[i],(double)par->rf[i]);
}
/*======================================================*/
void get_baseline(int N, short *waveform,WaveFormPar *wpar)
{
  int i,n;
 
  if(N<wpar->baseline_range)
    {
      printf("Baseline range larger than waveform length\n");
      printf("Terminating program\n");
      exit(0);
    }
  wpar->b=0.;
  wpar->bsq=0.;
  n=0;
  for(i=0; i<wpar->baseline_range;i++)
    {
      wpar->b+=waveform[i];
      wpar->bsq+=waveform[i]*waveform[i];
      n++;
    }
  wpar->bsq/=n;
  wpar->b/=n;
  wpar->bsq-=wpar->b*wpar->b;
  wpar->bsq=sqrt(wpar->bsq);
  wpar->bflag=1;
}
/*======================================================*/
void get_tmax(int N, short *waveform,WaveFormPar *wpar)
{
  int i,j,sum;
  int D=FILTER/2;
  
  wpar->max=0;
  for(i=D;i<N-D;i++)
    {
      sum=0;
      for(j=i-D;j<i+D;j++)
	sum+=waveform[j];
      sum/=FILTER;
      if(sum>wpar->max)
	{
	  wpar->max=sum;
	  wpar->tmax=i;
	}
    }
  wpar->mflag=1;

}
/*======================================================*/
void get_sig2noise(int N, short *waveform,WaveFormPar *wpar)
{
  get_baseline(wpar->baseline_range,waveform,wpar);
  get_tmax(N,waveform,wpar);
  wpar->sig2noise=(wpar->max-wpar->b)/wpar->bsq;
 
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
  f=wpar->b+frac*(wpar->max-wpar->b);
  flow=wpar->b+fraclow*(wpar->max-wpar->b);
  fhigh=wpar->b+frachigh*(wpar->max-wpar->b);

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


/*------------------------------------------------------------*/
int get_shape(int dim, int N, short *waveform,ShapePar* par, WaveFormPar *wpar)
{
  
  long double s,tau,tau_i,tau_j;
  int i,j,p,q,d;
  lin_eq_type e;
  memset(&e,0,sizeof(lin_eq_type));

  
 
  /* q is the low limit of the signal section */
  q=wpar->temax;
 
  /* p is the high limit of the baseline section */
  p=wpar->temin;
  d=N;
  e.dim=dim;
  for(int i=0;i<NSHAPE;i++) par->am[i]=0.;
  par->chisq=0.;
  par->ndf=-e.dim;
  for(i=1;i<e.dim;i++)
    {
      tau=CsI_get_tau(i,par);
      tau_i=tau;

      s=-((double)q)/tau+log(1.-exp(-((double)(d-q))/tau));
      s-=log(1.-exp(-1./tau));
      e.matrix[i][0]=exp(s);
      e.matrix[0][i]=exp(s);
      
      tau/=2.;
      s=-((double)q)/tau+log(1.-exp(-((double)(d-q))/tau));
      s-=log(1.-exp(-1./tau));
      e.matrix[i][i]=exp(s);
      for(j=i+1;j<e.dim;j++)
  	{
	  tau_j=CsI_get_tau(j,par);
  	  tau=tau_i*tau_j/(tau_i+tau_j);
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
  
  for(i=1;i<e.dim;i++)
    {
      tau=CsI_get_tau(i,par);
      
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

 
  if(solve_lin_eq(&e)==0)
    	{
	  par->chisq=BADCHISQ;
	  par->ndf=1;
	  return BADCHISQ;
	}
  else
    {
  
      par->t[0]=find_t0(N,par,wpar,e);
      if(par->t[0]<=0)
	{
	  par->chisq=T0FAIL;
	  par->ndf=1;
	  return T0FAIL;
	}
		       
      par->am[0]=e.solution[0];
      for(i=1;i<e.dim;i++)
	{
	  tau=CsI_get_tau(i,par);
	  par->am[i]=e.solution[i]*exp(-par->t[0]/tau);
	}

      for(i=0;i<e.dim;i++)	
	par->chisq-=e.solution[i]*e.vector[i];      
   
    

      if(par->chisq<0)
      	{
	  par->chisq=BADCHISQ;
	  par->ndf=1;
	  return BADCHISQ;
	}
      for(i=2;i<e.dim;i++)
	par->am[i]*=-1;
 
    }
 
  
  for(i=0;i<e.dim;i++)
    if(par->am[i]<0)
      	{
	  par->chisq=BADCHISQ;
	  par->ndf=1;
	  return BADCHISQ;
	}

  return par->chisq;
}
/*===================================================================================*/
double find_t0(int N,ShapePar*par,WaveFormPar *wpar,lin_eq_type e)
{
  double a,b,c,ta,tb,tc,delta,slope,tau;
  int i;
 
  //  printf("In fit_t0\n");
  ta=wpar->baseline_range;
  a=0.;
  for(i=1;i<e.dim;i++)
    {
      tau=CsI_get_tau(i,par);
      a+=e.solution[i]*exp(-ta/tau);
    }
  
  tb=wpar->tmax;
  b=0.;
  for(i=1;i<e.dim;i++)
    {
      tau=CsI_get_tau(i,par);
      b+=e.solution[i]*exp(-tb/tau);
    }

    delta=1;



      if((a<0)&&b>0)
	{
	  while(delta>EPS)
	    {
	      slope=-a/(b-a);
	      if(slope>0.99)
		slope=0.99;

	      if(slope<0.01)
		slope=0.01;

	      tc=ta+slope*(tb-ta);
	      c=0.;
	      for(i=1;i<e.dim;i++)
		{
		  tau=CsI_get_tau(i,par);		 
		  c+=e.solution[i]*exp(-tc/tau);
		}
	      if(c>0)
		{
		  tb=tc;
		  b=c;
		}
	      else
		{
		  ta=tc;
		  a=c;
		}
	      delta=fabs(c);
	      //	         printf("a %10.4f b %10.4f c %10.4f ta %10.4f tb %10.4f tc %10.4f delta %10.4f\n",(double)a,(double)b,(double)c,(double)ta,(double)tb,(double)tc,(double)delta);
	    }
	}
      else
	{
	  /* printf("a %10.4f b %10.4f ta %10.4f tb %10.4f \n",(double)a,(double)b,(double)ta,(double)tb); */
	  /* printf("Bad initial condition in t0 solver\n"); */
	  return -1;
	}
      //       printf("out of fit_t0\n");
      return tc;
}
/*------------------------------------------------------------*/
double  fit_waveform(int N, short *waveform,ShapePar* par,WaveFormPar* wpar,TApplication* theApp)
{
  ShapePar *store_s,*store_d;
  long double  chi_s,chi_d;
  int     ndf;
  wpar->baseline_range=CSI_BASELINE_RANGE;
  CsI_get_exclusion_zone(N,waveform,wpar);
  if(get_shape_with_RF(4,N,waveform,par,wpar)>0)
    {
      par->type=1;
      chi_s=0.;
      ndf=-8;
      for(Int_t i=0;i<wpar->temin;i++)
	{
	  ndf++;
	  chi_d=waveform[i]-CsI_FitFunctionWithRF(i,par);
	  chi_d*=chi_d;
	  chi_s+=chi_d;
	}
      for(Int_t i=wpar->temax;i<N;i++)
	{
	  ndf++;
	  chi_d=waveform[i]-CsI_FitFunctionWithRF(i,par);
	  chi_d*=chi_d;
	  chi_s+=chi_d;
	}
      printf("chi comp %10.3Lf %10.3Lf\n",par->chisq,chi_s);
      printf("ndf comp %10d %10d\n",par->ndf,ndf);
      getc(stdin);
      return par->chisq;
    }
  else
    {
      par->type=-1;
      //      CsI_ShowFitWithRF(N,waveform,par,wpar,theApp);
      // try a fit with the slow component amplitude fixed to 0
      store_s=(ShapePar*)malloc(sizeof(ShapePar));
      memcpy(store_s,par,sizeof(ShapePar)); 
      chi_s=get_shape_with_RF(3,N,waveform,store_s,wpar);
      //     CsI_ShowFitWithRF(N,waveform,store_s,wpar,theApp);
      // try a fit of a direct hit
      store_d=(ShapePar*)malloc(sizeof(ShapePar));
      memcpy(store_d,par,sizeof(ShapePar)); 
      store_d->t[2]=par->t[4];
      store_d->t[4]=par->t[2];
      chi_d=get_shape_with_RF(3,N,waveform,store_d,wpar);
      //     CsI_ShowFitWithRF(N,waveform,store_d,wpar,theApp);
      /* printf("chi_s %10.3f chi_d %10.3f\n",chi_s,chi_d); */
      /* getc(stdin); */
      if((chi_s<=0)&&(chi_d<=0))
	return BADCHISQ;
    
      if((chi_s>0)&&(chi_d<=0))
	{
	  memcpy(par,store_s,sizeof(ShapePar)); 
	  par->type=3;
	  CsI_ShowFitWithRF(N,waveform,store_s,wpar,theApp);
	  CsI_ShowFitWithRF(N,waveform,store_d,wpar,theApp);
	  return par->chisq;
	}

      if((chi_s<=0)&&(chi_d>0))
	{
	  memcpy(par,store_d,sizeof(ShapePar));
	  par->t[2]=store_d->t[4];
	  par->am[2]=store_d->am[4];
	  par->t[4]=store_d->t[2];
	  par->am[4]=store_d->am[2];
	  par->type=2;
	  return par->chisq;
	}

      if((chi_s>0)&&(chi_d>0))
	{
	  if(chi_d<chi_s)
	    {
	      memcpy(par,store_d,sizeof(ShapePar));
	      par->t[2]=store_d->t[4];
	      par->am[2]=store_d->am[4];
	      par->t[4]=store_d->t[2];
	      par->am[4]=store_d->am[2]; 
	      par->type=2;
	      return par->chisq;
	    }
	  else
	    {
	      memcpy(par,store_s,sizeof(ShapePar)); 
	      par->type=3;

	      CsI_ShowFitWithRF(N,waveform,store_s,wpar,theApp);
	      CsI_ShowFitWithRF(N,waveform,store_d,wpar,theApp);
  
	      return par->chisq;
	    }
	}
	
      
    }
 
  return BADCHISQ;
}
/*------------------------------------------------------------*/
double  fit_waveform_with_RF(int N, short *waveform,ShapePar* par,WaveFormPar* wpar)
{
 
  long double chi_d,chi_s;
  int    D;
  ShapePar *store_s,*store_d;
 


  par->type=-1;
  //first attempt is a regular fit with a fast and slow component
  wpar->baseline_range=CSI_BASELINE_RANGE;
  CsI_get_exclusion_zone(N,waveform,wpar);
 
  if(get_shape_with_RF(4,N,waveform,par,wpar)>0)
    {
      //get the chisq and ndf contribution in the exclusion zone
      CsI_AddChiSqInTheExclusionZone(waveform,par,wpar);
    }
  //if you are here fitting failed, bad luck
  //fit is assigned the parameter -1
  return -1;
}
/*------------------------------------------------------------*/
double  fit_TIFFIN_waveform(int N, short *waveform,ShapePar* par,WaveFormPar* wpar)
{
 
  wpar->baseline_range=TIFFIN_BASELINE_RANGE;
  get_exclusion_zone_for_TIFFIN(N,waveform,wpar);
  get_TIFFIN_t0(N,waveform,wpar);
  return get_TIFFIN_shape(N,waveform,par,wpar);

}
/*------------------------------------------------------------*/
void  get_Griffin_t0(int N, short *waveform,WaveFormPar* wpar)
{
  int i,try_a_line=0;
  lin_eq_type e;
  double a,b,c,d,t;
  wpar->t00=-1;
  get_exclusion_zone_for_Griffin(N,waveform,wpar);
  get_tmax(N,waveform,wpar);
  get_t30(N,waveform,wpar);
  wpar->thigh=wpar->t30;
  
  //fit line to the baseline

  memset(&e,0,sizeof(lin_eq_type));
  e.dim=2;
  for(i=0;i<=wpar->temin;i++)
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

  //fit parabola to the risetime

  memset(&e,0,sizeof(lin_eq_type));
  e.dim=3;
  for(i=wpar->temax;i<=wpar->thigh;i++)
    {
      e.matrix[0][0]+=1;
      e.matrix[0][1]+=i;
      e.matrix[0][2]+=i*i;
      e.matrix[1][2]+=i*i*i;
      e.matrix[2][2]+=i*i*i*i;
      e.vector[0]+=waveform[i];
      e.vector[1]+=waveform[i]*i;
      e.vector[2]+=waveform[i]*i*i;
    }
  e.matrix[1][0]=e.matrix[0][1];
  e.matrix[1][1]=e.matrix[0][2];
  e.matrix[2][0]=e.matrix[0][2];
  e.matrix[2][1]=e.matrix[1][2];

  if(solve_lin_eq(&e)==0)
      try_a_line=1;
  else
    {
      wpar->s0=e.solution[0];
      wpar->s1=e.solution[1];
      wpar->s2=e.solution[2];
    }
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
	  
	  d=sqrt(d);
	  t=0.5*(-b+d)/a;
	}
      else
	try_a_line=1;
    }

  if(try_a_line!=0)
    {
      memset(&e,0,sizeof(lin_eq_type));
      e.dim=2;
      for(i=wpar->temax;i<=wpar->thigh;i++)
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
	wpar->t00=t;
	return;
      }

}
/*------------------------------------------------------------*/
void  get_CsI_t0(int N, short *waveform,ShapePar* par,WaveFormPar* wpar)
{
  int i,tlmin,tlmax;
  lin_eq_type e;
  double b,c,t;

  wpar->t00=-1.;

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
	wpar->t00=t;
	return;
      }


}
/*------------------------------------------------------------*/
void    CsI_get_exclusion_zone(int N, short *waveform,WaveFormPar *wpar)
{
  int i,j,D;
  double s,bmax,bmin;
 

  wpar->baseline_range=CSI_BASELINE_RANGE;
  get_baseline(wpar->baseline_range,waveform,wpar);
  if(wpar->bflag!=1)
    {
      printf("Baseline not established\n");
      printf("Terminating program\n");
      exit(0);
    }
  //find the high limit
  //values averaged over the FILTER region should be above b + noise level
  //for all samples above the limit

  get_tmax(N,waveform,wpar);
  D=FILTER/2;
  bmax=wpar->b+NOISE_LEVEL_CSI;
  bmin=wpar->b-0.25*NOISE_LEVEL_CSI;
  for(i=wpar->tmax;i>wpar->baseline_range;i--)
    {
      s=0.;
      for(j=i-D;j<i+D;j++)
	s+=waveform[j];
      s/=FILTER;
      if(s<bmax)
	{
	  wpar->temax=i;
	  break;
	}
    }

 //find the low limit as the last sample before temax
  //at which the waveform is smaller than 0.25 of the noise below baseline

  for(i=wpar->temax;i>wpar->baseline_range;i--)
    if(waveform[i]<bmin)
      break;
  wpar->temin=i;

}

/*------------------------------------------------------------*/
int get_shape_with_RF(int dim, int N, short *waveform,ShapePar* par, WaveFormPar *wpar)
{
  
  long double tau,tau_i,tau_j,l;
  int i,j,p,q,d;
  lin_eq_type e;
  double s,sn,snm,s2,s2n,s2nm,c,cn,cnm,c2,c2n,c2nm,w,x,y;

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
      s=-((double)q)/tau+log(1.-exp(-((double)(d-q))/tau));
      s-=log(1.-exp(-1./tau));
      e.matrix[i][0]=exp(s);
      e.matrix[0][i]=exp(s);
      
      tau/=2.;
      s=-((double)q)/tau+log(1.-exp(-((double)(d-q))/tau));
      s-=log(1.-exp(-1./tau));
      e.matrix[i][i]=exp(s);
      for(j=i+1;j<dim;j++)
  	{
	  if(j==1)
	    tau_j=par->t[1];
	  else
	    tau_j=par->t[j]*par->t[1]/(par->t[j]+par->t[1]);

  	  tau=tau_i*tau_j/(tau_i+tau_j);
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
	  par->chisq=BADCHISQ;
	  par->ndf=1;
	  return BADCHISQ;
	}
  else
    {
  
      for(i=0;i<e.dim;i++)
	par->chisq-=e.solution[i]*e.vector[i];      

      if(par->chisq<0)
      	{
      	  par->chisq=BADCHISQ;
      	  par->ndf=1;
      	  return BADCHISQ;
      	}
     
      e.dim=dim;
      par->t[0]=find_t0(N,par,wpar,e);
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
/*------------------------------------------------------------*/
void    get_exclusion_zone_for_Griffin(int N, short *waveform,WaveFormPar *wpar)
{
 
  double bmax;



  wpar->baseline_range=GRIFFIN_BASELINE_RANGE;
  get_baseline(wpar->baseline_range,waveform,wpar);
  if(wpar->bflag!=1)
    {
      printf("Baseline not established\n");
      printf("Terminating program\n");
      exit(0);
    }
  //find the high limit
  //values averaged over the FILTER region should be above b+bsq
  //for all samples above the limit

  get_tmax(N,waveform,wpar);

  bmax=wpar->b+NOISE_LEVEL_GRIFFIN;

  wpar->temax=wpar->tmax;
  while(waveform[wpar->temax]>bmax)
    wpar->temax--;
  wpar->temax++;
  if(wpar->temax<wpar->baseline_range)
    wpar->temax=wpar->baseline_range;
  //find the low limit
  //go down below baseline
  wpar->temin=wpar->temax;
  while(waveform[wpar->temin]>wpar->b)
    wpar->temin--;
  wpar->temin--;
  /* if(wpar->temin>wpar->temax) */
  /*   wpar->temin=wpar->baseline_range; */


}
/*------------------------------------------------------------*/
void  get_TIFFIN_t0(int N, short *waveform,WaveFormPar* wpar)
{
  int i;
  lin_eq_type e;
  double b,c,t;
  wpar->t00=-1;
 
  //  get_exclusion_zone_for_TIFFIN(N,waveform,wpar);
  //  get_tmax(N,waveform,wpar);
 
  wpar->thigh=get_tfrac( N, waveform, wpar,0.2,0.15,0.25);
  if(wpar->thigh<=wpar->temax)
    wpar->thigh=wpar->temax+5;
  
  //fit line to the baseline

  memset(&e,0,sizeof(lin_eq_type));
  e.dim=2;
  for(i=0;i<=wpar->temin;i++)
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

  //fit line to the risetime
  memset(&e,0,sizeof(lin_eq_type));
  e.dim=2;
  for(i=wpar->temax;i<=wpar->thigh;i++)
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
  
      
  if(t>=0)
    if(t<N)
      {
	wpar->t00=t;
	return;
      }

}
/*------------------------------------------------------------*/
void    get_exclusion_zone_for_TIFFIN(int N, short *waveform,WaveFormPar *wpar)
{
 
  double bmax,bmin;

  wpar->baseline_range=TIFFIN_BASELINE_RANGE;
  get_baseline(wpar->baseline_range,waveform,wpar);
  if(wpar->bflag!=1)
    {
      printf("Baseline not established\n");
      printf("Terminating program\n");
      exit(0);
    }
  //find the high limit
  //values averaged over the FILTER region should be above b+bsq
  //for all samples above the limit

  get_tmax(N,waveform,wpar);

  bmax=wpar->b+NOISE_LEVEL_TIFFIN;

  wpar->temax=wpar->tmax;
  while(waveform[wpar->temax]>bmax)
    wpar->temax--;

  //find the low limit
  //go down below baseline
  wpar->temin=wpar->temax;
  bmin=wpar->b-0.25*NOISE_LEVEL_TIFFIN;
  while(waveform[wpar->temin]>bmin)
    {
      wpar->temin--;
    }
  wpar->temin-=TIFFIN_LL_SHIFT;
  if(wpar->temin<wpar->baseline_range)
    wpar->temin=wpar->baseline_range;
  if(wpar->temin>wpar->temax)
    wpar->temin=wpar->baseline_range;


}
/*------------------------------------------------------------*/
int get_TIFFIN_shape(int N, short *waveform,ShapePar* par, WaveFormPar *wpar)
{
  
  double t0,ene;
  long double x;
  int i,p,q,d;
  lin_eq_type e;
  memset(&e,0,sizeof(lin_eq_type));


  /* q is the low limit of the signal section 10 us with respect to the lower bound of the exclusion zone*/
  q=wpar->temin+TIFFIN_RISE;
 
  /* p is the high limit of the baseline section */
  p=wpar->temin-200;
  d=N;
  for(int i=0;i<NSHAPE;i++) par->am[i]=0.;
  par->chisq=0.;
  par->ndf=-e.dim;

  //line in the baseline section
  memset(&e,0,sizeof(lin_eq_type));
  e.dim=2;
  for(i=0;i<p;i++)
   {
     x=i/1000.;
     e.matrix[0][0]+=1;
     e.matrix[0][1]+=x;
     e.matrix[1][1]+=x*x;;
     e.vector[0]+=waveform[i];
     e.vector[1]+=waveform[i]*x;
     par->chisq+=waveform[i]*waveform[i];
     par->ndf++;
  }
  e.matrix[1][0]=e.matrix[0][1];
 

  
  if(solve_lin_eq(&e)==0)
    return BADCHISQ;
  else
    {
      wpar->s0=e.solution[0];
      wpar->s1=e.solution[1];
 
      for(i=0;i<e.dim;i++)
  	par->chisq-=e.solution[i]*e.vector[i];

    }

  //line in the signal section
  memset(&e,0,sizeof(lin_eq_type));
  e.dim=2;
  for(i=q;i<d;i++)
    {
      x=i/1000.;
      e.matrix[0][0]+=1;
      e.matrix[0][1]+=x;
      e.matrix[1][1]+=x*x;

      e.vector[0]+=waveform[i];
      e.vector[1]+=waveform[i]*x;
 
      par->chisq+=waveform[i]*waveform[i];
      par->ndf++;
    }
  e.matrix[1][0]=e.matrix[0][1];
 

  //parabola in the signal section
  /* memset(&e,0,sizeof(lin_eq_type)); */
  /* e.dim=3; */
  /* for(i=q;i<d;i++) */
  /*   { */
  /*     x=i/1000.; */
  /*     e.matrix[0][0]+=1; */
  /*     e.matrix[0][1]+=x; */
  /*     e.matrix[0][2]+=x*x; */
  /*     e.matrix[1][2]+=x*x*x; */
  /*     e.matrix[2][2]+=x*x*x*x; */
  /*     e.vector[0]+=waveform[i]; */
  /*     e.vector[1]+=waveform[i]*x; */
  /*     e.vector[2]+=waveform[i]*x*x; */
  /*     par->chisq+=waveform[i]*waveform[i]; */
  /*     par->ndf++; */
  /*   } */
  /* e.matrix[1][0]=e.matrix[0][1]; */
  /* e.matrix[1][1]=e.matrix[0][2]; */
  /* e.matrix[2][0]=e.matrix[0][2]; */
  /* e.matrix[2][1]=e.matrix[1][2]; */

	
  
  
  if(solve_lin_eq(&e)==0)
    return BADCHISQ;
  else
    {
      par->am[0]=e.solution[0];
      par->am[1]=e.solution[1];
      //      par->am[2]=e.solution[2];
      for(i=0;i<e.dim;i++)
  	par->chisq-=e.solution[i]*e.vector[i];

    }
  get_t90(N, waveform, wpar);
  t0=wpar->t90;
  get_t10(N,waveform,wpar);
  t0-=wpar->t10;
  par->t[2]=t0;
  get_t50(N, waveform, wpar);
  t0=wpar->t50;
  par->t[0]=wpar->t00;
  x=t0/1000.;
  ene=par->am[2]*x*x+par->am[1]*x+par->am[0];
  ene=ene-wpar->s2*x*x-wpar->s1*x-wpar->s0;
  par->t[1]=ene;
  return par->chisq;
}
/*------------------------------------------------------------*/
void    CsI_AddChiSqInTheExclusionZone(short *waveform,ShapePar* par,WaveFormPar* wpar)
{
  long double chi_sum,chi;
  int    ndf,i;
 
  ndf=0;
  chi_sum=0.;
  par->ndf_f=par->ndf;
  par->chisq_f=par->chisq;
  for(i=wpar->temin;i<wpar->temax;i++)
    {
      ndf++;
      chi=waveform[i]-CsI_FitFunctionWithRF(i,par);
      chi=chi*chi;
      chi_sum+=chi;
    }
  par->ndf_ex=ndf;
  par->chisq_ex=chi_sum;
  par->ndf+=ndf;
  par->chisq+=chi_sum;
}
