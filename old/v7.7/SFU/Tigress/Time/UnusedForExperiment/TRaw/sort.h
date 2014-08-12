#ifndef SORT_H
#define SORT_H

//Prior to putting in 1ns units!!!
#define RFphase 135.74544

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"

#include "TH2D.h"
#include "TH1D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"

TH2D *h;
TH1D *g;

int pos,col,offset=0,shift=0;

int hist[NPOSTIGR][NCOL][S32K];
int histRF[NPOSTIGR][NCOL][S4K];
#endif
