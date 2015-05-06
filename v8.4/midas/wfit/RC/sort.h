#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "sort_but_not_assemble.h"
#include "waveform_analyzer.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"
#include "TF1.h"
#include "TROOT.h"
#include "math.h"

TF1 *f;
TH1D *h;
TH2D *g,*g1;
TCanvas *c;  
TApplication *theApp;
ShapePar* par;
int  chnlow,chnhigh;
#endif
