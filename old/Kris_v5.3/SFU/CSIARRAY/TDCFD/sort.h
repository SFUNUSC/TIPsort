#ifndef SORT_H
#define SORT_H


#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"

TH2D *h;
double chimin,chimax;
double del;
int  hist[NCSI][S32K];

typedef struct par_tdcfd{
  double t0,t1,t2;
  double p1,p2;
  double q1,q2;
  double l1,l2;
  double a1,a2;
  double r;
  double td,z;
  double e1,e2;
} par_tdcfd;

par_tdcfd par;

double get_tdcfd(ShapePar*);
double f(double t){return 1+par.a1*exp(-t/par.t1)+par.a2*exp(-t/par.t2);};
double fp(double t){return -par.l1*par.a1*exp(-t/par.t1)-par.l2*par.a2*exp(-t/par.t2);};
#endif
