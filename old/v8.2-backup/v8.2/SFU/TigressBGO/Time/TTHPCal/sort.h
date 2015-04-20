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

TH2D *h;
cal_event* cev;
calibration_parameters* cal_par;
int tthp[NPOSTIGR*NCOL][NPOSTIGR*NCOL*NSUP];

#endif
