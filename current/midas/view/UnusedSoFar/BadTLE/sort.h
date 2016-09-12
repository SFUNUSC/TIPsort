#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "sort_but_not_assemble.h"
#include "TH1D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"

TH1D *h,*d,*cfd;
TCanvas *c;  
TApplication *theApp;
int  chn;
int  window,gap;
int  clip_delay,hit_threshold;
int  delay;
float fraction;

#endif
