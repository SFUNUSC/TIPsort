#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-encoder.h"
#include "SFU-cal.h"

#include "TH2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"

TH1D *h;
TH1D *g;

FILE* output;
int enb[BUFFSIZE];

int corr;

double tigTVal;

calibration_parameters* cal_par;
double gate_length,half_gate_length;
#endif
