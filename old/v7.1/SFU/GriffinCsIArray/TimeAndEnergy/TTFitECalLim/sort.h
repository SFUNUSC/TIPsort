#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

#include "TH2D.h"
#include "TH1D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"

TH2D *h;
TH1D *h_e1,*h_t1,*h_e2,*h_t2,*h_tdiff;
calibration_parameters* cal_par;

int chimin=0, chimax=0;
int idmin=0, idmax=0;
int counter;

#endif