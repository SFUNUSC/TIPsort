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
calibration_parameters* cal_par;
int ring;
double s0[NS3SEC],s1[NS3SEC],s2[NS3SEC];
double v0[NS3SEC],v1[NS3SEC],v2[NS3SEC];
#endif
