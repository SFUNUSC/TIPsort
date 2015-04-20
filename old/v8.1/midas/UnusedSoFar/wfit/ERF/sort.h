#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "sort_but_not_assemble.h"

#include "TH1D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"
#include "TF1.h"
#include "TROOT.h"
#include "math.h"
#include "TMath.h"

TF1  *f;
TH1D *h,*s;
TCanvas *c;  
TApplication *theApp;
int  chn,base;

#endif
