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

TH2D *h_csi1[NRING], *h_csi2[NRING];
int hist1[NRING][S32K],hist2[NRING][S32K];
calibration_parameters* cal_par;
double A;

#endif
