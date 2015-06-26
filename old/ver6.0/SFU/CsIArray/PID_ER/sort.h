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

TH2D *h,*g;
int  det_id;
int  idmin,idmax;
double chimin,chimax;
int  hist[NCSI][S32K];
#endif