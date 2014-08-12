#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "sort_but_not_assemble.h"
#include "waveform_analyzer.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TH1I.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"
#include "TF1.h"
#include "TROOT.h"
#include "math.h"

int hist1[S32K];

TH1D *h,*r;
TH2D *er;
TCanvas *c;
ShapePar* par;
int  chn;
float a;
TApplication *theApp;
#endif
