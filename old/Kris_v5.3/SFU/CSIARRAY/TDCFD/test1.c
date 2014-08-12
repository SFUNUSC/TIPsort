#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double p1,p2,t1,t2,l1,l2,r,z,td;
double f(double t)
{
  double alpha[10],sgn[10],fact[10],l1p[10],l2p[10],tp[10];
  double a1,a2,d;
  double v;
  int i;
  a1=-r*(exp(l1*td)-z)/(1-z);
  a2=-(1-r)*(exp(l2*td)-z)/(1-z);
  d=a1*l1+a2*l2;

  sgn[0]=1.;
  fact[0]=1.;
  l1p[0]=1.;
  l2p[0]=1.;
  tp[0]=1.;
  for(i=1;i<10;i++)
    {
      sgn[i]=sgn[i-1]*(-1);
      fact[i]=fact[i-1]*i;
      l1p[i]=l1p[i-1]*l1;
      l2p[i]=l2p[i-1]*l2;
      tp[i]=tp[i-1]*t;
    }

  alpha[0]=(1+a1+a2)/d;

  alpha[1]=0;
  
  for(i=2;i<10;i++)
    alpha[i]=sgn[i]/fact[i]*(a1*l1p[i]+a2*l2p[i])/d;

  v=0;
  for(i=0;i<10;i++)
    v+=alpha[i]*tp[i];

  return v;
  
}
double w(double t)
{
  return p1*(1-exp(-l1*t))+p2*(1-exp(-l2*t));
}
double wcfd(double t)
{
  return -z*w(t)+w(t-td);
}
int main(int argc, char *argv[])
{

  double t,s,u,w,v,e1,e2;
  int    i;
  double eps=0.001;
  if(argc!=6)
    {
      printf("\n./test a1 a2 t1 t2 td\n");
      exit(-1);
    }

  p1=atof(argv[1]);
  p2=atof(argv[2]);
  t1=atof(argv[3]);
  t2=atof(argv[4]);
  td=atof(argv[5]);
 
  l1=1./t1;
  l2=1./t2;
  r=p1/(p1+p2);

  e1=exp(l1*td);
  e2=exp(l2*td);

  z=(l1*e1*(1-e2)-l2*e2*(1-e1))/(l1*(1-e2)-l2*(1-e1));
  
 

 for(t=0.;t<300;t+=10)
    printf("%10.0f %10.4f\n",t,wcfd(t));

 s=1-e2;
  u=e2-e1;
  w=l2*(z-e2);
  v=z*(l1-l2)+l2*e2-l1*e1;

  t=(t2*(1-e2)-t1*(1-e1))/(e1-e2);
 
 
  printf("z %10.4f  t  %10.4f\n",z,t);



 
 while(fabs(wcfd(t))>eps)
   {
     printf("%10.4f  %10.4f %10.4f\n",t,f(t),wcfd(t));
     t=f(t);
   }
 printf("%10.4f  %10.4f %10.4f\n",t,f(t),wcfd(t));
}

