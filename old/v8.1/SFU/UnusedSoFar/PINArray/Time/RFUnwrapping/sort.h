#ifndef SORT_H
#define SORT_H

//Prior to putting in 1ns units!!!
#define RFphase 135.74544

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "math.h"

#include "TH2D.h"
#include "TH1D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"

TH2D *h;
TH1D *g;

int pin;
int count_1=0,count_2=0;
int count_3=0,count_4=0;  

int  hist[NPIN][S32K];
int  histRF[NPIN][S4K];
#endif
