#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "sort_but_not_assemble.h"
#include "get_sin_par.h"

#include "TH1D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"
#include "TF1.h"

TF1  *f;
TH1D *h,*g;
TCanvas *c;  
TApplication *theApp;
StructSinPar spar;
int  chn;
double max_chisq;
double t,t2,n;


#endif
