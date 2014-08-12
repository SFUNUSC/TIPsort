#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "sort_but_not_assemble.h"
#include "waveform_analyzer.h"
#include "TH1D.h"
#include "TVirtualFFT.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TFile.h"
#include "TApplication.h"
#include "math.h"

TH1D *h,*g;
TH1 *fourier_transform;
TH1D *FFTsum,*FFTsumfit;
bool firstfragment;
TCanvas *c;  
TApplication *theApp;
int  chn,len;
ShapePar* par;
#endif
