#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"

TH2D *hFITlocal,*hFITfunction,*hCFD;
TH1D *h_ecsi,*h_tcsiFITlocal,*h_tcsiFITfunction,*h_tcsiCFD,*h_trf,*h_tdiffFITlocal,*h_tdiffFITfunction,*h_tdiffCFD;

int  idmin,idmax;
double chimin,chimax;

#endif
