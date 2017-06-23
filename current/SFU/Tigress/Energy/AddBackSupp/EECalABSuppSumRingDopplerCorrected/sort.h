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

int  hist[S4K];
short int  mat[S4K][S4K];
calibration_parameters* cal_par;
double  tlow,thigh;
double supLow,supHigh;
int colAddBack;
double cntr;

double dshift[NRING+1];
char str[256];
int num_rings;
FILE *shiftFile;

TH2D *h;

#endif
