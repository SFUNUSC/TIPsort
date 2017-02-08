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

TH1D *h, *positionHist;
//int  hist[NCSIRING][S32K];
calibration_parameters* cal_par;


double res_dir[2][3]; //direction of detected particles in CsI Array
double res_dir_mag[2]; //magnitude of the above vectors

#endif
