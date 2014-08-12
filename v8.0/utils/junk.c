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
	  par->am[i]=e.solution[i]*exp(-par->t[0]/t[i]);
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
