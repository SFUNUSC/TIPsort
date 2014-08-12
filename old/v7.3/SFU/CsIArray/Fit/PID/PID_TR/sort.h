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

TH2D *h[NCSI];

int  pos,idmin,idmax;
double chimin,chimax;

int  hist1[S32K];
int  hist2[S32K];

#endif
