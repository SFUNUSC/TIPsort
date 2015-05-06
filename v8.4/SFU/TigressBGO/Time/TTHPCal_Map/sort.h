#ifndef SORT_H
#define SORT_H


#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

#include "TH2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"

cal_event* cev;
calibration_parameters* cal_par;

TH2D *h,*h2;

int tthp[NPOSTIGR*NCOL][NPOSTIGR*NCOL*NSUP];

double tigE_low, tigE_high;

#endif
