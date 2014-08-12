#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "sort_and_assemble.h"
#include "TH1D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"

#define NHP  NSEGTIGR+NSUP
TH1D *h[NPOSTIGR][NCOL];
char cs[4][8]={"blue","green","red","white"};
Color_t rc[4]={kBlue,kGreen,kRed,kWhite};
int  pos,col;

#endif
