#ifndef SORT_H
#define SORT_H

#define RANGE 64

#include "SFU-common.h"
#include "sort_but_not_assemble.h"
#include "waveform_analyzer.h"

#include "TH1D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"
#include "TF1.h"
#include "TROOT.h"
#include "math.h"

TH1D *h,*g,*base,*fit;
TCanvas *c;
ShapePar* par;
int  chn;
double chmin,chmax;
TApplication *theApp;
#endif
