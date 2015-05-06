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
#include "TCutG.h"
#include "TROOT.h"

#define pMax 6
#define aMax 4
#define NSP  7

double chimin,chimax;
int    idmin,idmax,pos,pos_i,pos_j,col;
int    alphaLowerLimit,alphaUpperLimit, protonUpperLimit;

int aGateLowFlag[NCSI] = { 0 };
int aGateHighFlag[NCSI] = { 0 };
int pGateHighFlag[NCSI] = { 0 };

int aGateLow[NCSI] = { 0 };
int aGateHigh[NCSI] = { 0 };
int pGateHigh[NCSI] = { 0 };

int  hist[NSP][S32K];

calibration_parameters* cal_par;

int na,np,sp;  
int ch[pMax][aMax],ev=0;
int id[pMax][aMax];


#endif
