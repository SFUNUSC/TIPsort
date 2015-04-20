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
TH1D *talpha_proj,*tproton_proj,*x_proj;
double chimin,chimax;
double alpha,xbar;
int  hist[NCSI][S32K];

#endif
