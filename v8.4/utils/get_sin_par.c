#include "get_sin_par.h"

int get_sin_par(double T,int N, short *wave,StructSinPar *par)
{
  int i;
  lin_eq_type e;
  double s,sn,snm,s2,s2n,s2nm,c,cn,cnm,c2,c2n,c2nm,w;
  e.dim=3;

  w=TWOPI/T;

  s=sin(w);
  sn=sin(N*w);
  snm=sin((N-1)*w);
  s2=sin(2*w);
  s2n=sin(2*N*w);
  s2nm=sin(2*(N-1)*w);

  c=cos(w);
  cn=cos(N*w);
  cnm=cos((N-1)*w);
  c2=cos(2*w);
  c2n=cos(2*N*w);
  c2nm=cos(2*(N-1)*w);
 
  e.matrix[0][0]=0.5*N-0.25*(1-c2-c2n+c2nm)/(1-c2);
  e.matrix[0][1]=0.25*(s2+s2nm-s2n)/(1-c2);
  e.matrix[1][0]=e.matrix[0][1];
  e.matrix[0][2]=0.5*(s+snm-sn)/(1-c);
  e.matrix[2][0]=e.matrix[0][2];
  e.matrix[1][1]=0.5*N+0.25*(1-c2-c2n+c2nm)/(1-c2);
  e.matrix[1][2]=0.5*(1-c-cn+cnm)/(1-c);
  e.matrix[2][1]=e.matrix[1][2];
  e.matrix[2][2]=N;

  for(i=0;i<e.dim;i++)
    e.vector[i]=0;
  
  for(i=0;i<N;i++)
    {
      e.vector[0]+=wave[i]*sin(w*i);
      e.vector[1]+=wave[i]*cos(w*i);
      e.vector[2]+=wave[i];
    }
  if(solve_lin_eq(&e)==0)
    {
      //      printf("No solution for chi^2 fit of sin wave parameters \n");
      return 0;
    }
  par->A=sqrt(e.solution[0]*e.solution[0]+e.solution[1]*e.solution[1]);
  par->C=e.solution[2];
 
  s=-e.solution[1]/par->A;
  c=e.solution[0]/par->A;

  if(s>=0)
    par->t0=acos(c)*T/TWOPI;
  else
    par->t0=(1-acos(c)/TWOPI)*T;
  
  return 1.;
}
