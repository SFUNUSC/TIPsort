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

TCanvas *c;
TApplication *theApp;
TH1D *h;
ShapePar* par;
int chi,type;
int chn,chimin,chimax;
int idmin,idmax,lowRatio,highRatio;

#endif
