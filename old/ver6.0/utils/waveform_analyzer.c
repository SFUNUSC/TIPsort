#include "waveform_analyzer.h"
/*======================================================*/
void get_baseline(int N, short *waveform,WaveFormPar *wpar)
{
  int i;
 
  /* first N channels to get the baseline */
 
  wpar->b=0.;
  wpar->bsq=0.;
  for(i=0; i<N;i++)
    {
      wpar->b+=waveform[i];
      wpar->bsq+=waveform[i]*waveform[i];
    }
  wpar->bsq/=N;
  wpar->b/=N;
  wpar->bsq-=wpar->b*wpar->b;
  wpar->bsq=sqrt(wpar->bsq);
  wpar->bflag=1;
}
/*======================================================*/
void get_tmax(int N, short *waveform,WaveFormPar *wpar)
{
  int i;
 

  wpar->max=0;
  for(i=0;i<N;i++)
    if(waveform[i]>wpar->max)
      {
	wpar->max=waveform[i];
	wpar->tmax=i;
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
  double t;

  if((t=get_tfrac( N, waveform, wpar,0.5,0.3,0.8))>0)
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
  double t;
  
  if((t=get_tfrac( N, waveform, wpar,0.9,0.8,0.98))>0)
    {
      wpar->t90_flag=1;
      wpar->t90=t;
    }
  else
    {
      //       printf("t is %10.4f\n",t);
      wpar->t90_flag=-1;
      wpar->t90=-1;
    }
}
/*===========================================================*/
void get_t10(int N, short *waveform,WaveFormPar *wpar)
{
  double t;

  if((t=get_tfrac( N, waveform, wpar,0.1,0.05,0.2))>0)
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
  double t;

  if((t=get_tfrac( N, waveform, wpar,0.3,0.15,0.45))>0)
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
  
  long double b,bsq,bmax,s,tau;
  int i,j,p,q,d;
  lin_eq_type e;
  memset(&e,0,sizeof(lin_eq_type));

  
  if(wpar->bflag!=1)
    {
      printf("Baseline not determined for the PID two component fit\n");
      exit(1);
    }

  if(wpar->t10t90_flag!=1)
    {
      printf("Risetime  not determied for the PID two component fit\n");
      exit(1);
    }

  b=wpar->b;
  bsq=wpar->bsq;
  bmax=b+5*bsq;

  i=N/2;
  while(waveform[i]>bmax)
    {
      i--;
      if(i<=0)
	{
	  par->chisq=BADCHISQ;
	  par->ndf=1;
	  return BADCHISQ;
	}
    }
  /* q is the low limit of the signal section */
  q=i;
  while(waveform[i]>b)
    {
      i--;
      if(i<=0)
	{
	  par->chisq=BADCHISQ;
	  par->ndf=1;
	  return BADCHISQ;
	}
    }
  i--;
  while(waveform[i]<b)
    {
      i--;
      if(i<=0)
       	{
	  par->chisq=BADCHISQ;
	  par->ndf=1;
	  return BADCHISQ;
	}
    }
  /* p is the high limit of the baseline section */
  p=i;
  d=N;
  e.dim=dim;
  for(int i=0;i<NSHAPE;i++) par->am[i]=0.;
  par->chisq=0.;
  par->ndf=-e.dim;
  for(i=1;i<e.dim;i++)
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
      for(j=i+1;j<e.dim;j++)
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

 
  if(solve_lin_eq(&e)==0)
    	{
	  par->chisq=BADCHISQ;
	  par->ndf=1;
	  return BADCHISQ;
	}
  else
    {
  
      par->t[0]=find_t0(N,par,wpar,e);
      par->am[0]=e.solution[0];
      for(i=1;i<e.dim;i++)
  	par->am[i]=e.solution[i]*exp(-par->t[0]/par->t[i]);
 
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
  par->type=dim-2;
  return par->chisq;
}
/*===================================================================================*/
double find_t0(int N,ShapePar*par,WaveFormPar *wpar,lin_eq_type e)
{
  double a,b,c,ta,tb,tc,delta,slope;
  int i;
 
  //  printf("In fit_t0\n");
  ta=wpar->t10;
  a=1.;
  delta=0.05*(wpar->t90-wpar->t10);
  if(delta<1) delta=1;
  while(a>=0)
    {
      
      ta-=delta;
      if(ta<=0) 
	{
	  par->chisq=BADCHISQ;
	  par->ndf=1;
	  return BADCHISQ;
	}
      a=0.;
      for(i=1;i<e.dim;i++)
	a+=e.solution[i]*exp(-ta/par->t[i]);
    }
  
  tb=wpar->t10;
  b=-1.;
  while(b<=0.)
    {
	  tb+=delta;
	  if(tb>=N)
	    {
	      par->chisq=BADCHISQ;
	      par->ndf=1;
	      return BADCHISQ;
	    }
	  b=0.;
	  for(i=1;i<e.dim;i++)
	    b+=e.solution[i]*exp(-tb/par->t[i]);
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
		c+=e.solution[i]*exp(-tc/par->t[i]);
	      
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
	  printf("a %10.4f b %10.4f ta %10.4f tb %10.4f \n",(double)a,(double)b,(double)ta,(double)tb);
	  printf("Bad initial condition in t0 solver\n");
	  exit(1);
	}
      //       printf("out of fit_t0\n");
      return tc;
}
/*------------------------------------------------------------*/
double  fit_waveform(int N, short *waveform,ShapePar* par,WaveFormPar* wpar)
{

  ShapePar    p[4];
  double      ch[4];
  double      chmin;
  int         i,imin;
  

  wpar->baseline_range=RANGE;
  get_t10t90(N,waveform,wpar);
  if(wpar->t10t90_flag==1)
    {
      imin=-1;
      chmin=BADCHISQ;
      for(i=0;i<3;i++)	
      	{
	  memcpy(&p[i],par,sizeof(ShapePar));
	  ch[i]=get_shape(i+3,N,waveform,&p[i],wpar);
	  ch[i]/=p[i].ndf;
	  /* take the 3 parameter fit only when significantly better than 2 parameter fit */
	  if(i==2)
	    ch[i]+=5;
	 
	  if(ch[i]<chmin)
	    {
	      imin=i;
	      chmin=ch[i];
	    }
	}
      i=3;
      memcpy(&p[i],par,sizeof(ShapePar));
      p[3].t[2]=par->t[4];
      p[3].t[4]=par->t[2];

      ch[i]=get_shape(3,N,waveform,&p[i],wpar);
      ch[i]/=p[i].ndf;
      p[i].type=4;
      if(ch[i]<chmin)
      	{
      	  imin=i;
      	  chmin=ch[i];
      	}

      memcpy(par,&p[imin],sizeof(ShapePar));

      return chmin;	
    }
  else
    return BADCHISQ;
}
/*------------------------------------------------------------*/
void  get_Griffin_t0(int N, short *waveform,WaveFormPar* wpar)
{
  int i;
  lin_eq_type e;
  double a,b,c,d,t;

  wpar->t00=-1.;

  wpar->baseline_range=GRIFFIN_BASELINE_RANGE;
  get_baseline(wpar->baseline_range,waveform,wpar);
  get_tmax(N,waveform,wpar);
  get_t30(N,waveform,wpar);
  if(wpar->t30_flag==1)
    wpar->tlmax=(int)ceil(wpar->t30);
  else
    return;

  i=wpar->tlmax;

 
  while(waveform[i]>waveform[i-1])
    {
      i--;
      if(i<wpar->baseline_range+3)
	return;
    }

 
  wpar->tlmin=i+3;
  if(wpar->tlmin>=wpar->tlmax)
    return;

  wpar->baseline_range=i-3;

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

  //fit parabola to the risetime

  memset(&e,0,sizeof(lin_eq_type));
  e.dim=3;
  for(i=wpar->tlmin;i<=wpar->tlmax;i++)
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
      return;
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
  
  if(d>0)
    {
      if(d==0.)
	t=-0.5*b/a;
      
      d=sqrt(d);
      
      if(a>0)
	t=0.5*(-b+d)/a;
      
      if(a<0)
	t=0.5*(-b-d)/a;
    }
  // if parabola fails try a line
  if(d<0)
    {
      memset(&e,0,sizeof(lin_eq_type));
      e.dim=2;
      for(i=wpar->tlmin;i<=wpar->tlmax;i++)
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
  int i;
  lin_eq_type e;
  double b,c,t;

  wpar->t00=-1.;

  get_t30(N,waveform,wpar);
  if(wpar->t30_flag==1)
    wpar->tlmax=(int)ceil(wpar->t30);
  else
    return;

  //use t0 from the amplitude fit
  wpar->tlmin=(int)ceil(par->t[0]+2);
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
      for(i=wpar->tlmin;i<=wpar->tlmax;i++)
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
