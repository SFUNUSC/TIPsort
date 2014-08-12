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

TH2D *h[NCSI];
TH1D *h_e[NCSI];
int    pos,idmin,idmax;
double chimin,chimax;

calibration_parameters* cal_par;

int  hist1[S32K];
int  hist2[S32K];
#endif
