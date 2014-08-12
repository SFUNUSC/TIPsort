#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

#include "TCutG.h"
#include "TFile.h"
#include "TROOT.h"

#define pMax 6
#define aMax 4
#define NSP  7

TCutG *aGate[NCSI] = { new TCutG() };
TCutG *pGate[NCSI] = { new TCutG() };

double chimin,chimax;
int    idmin,idmax,pos,col;

int aGateFlag[NCSI] = { 0 };
int pGateFlag[NCSI] = { 0 };

int  hist[NSP][S32K];

calibration_parameters* cal_par;

int ch[pMax][aMax],ev=0;
int id[pMax][aMax];
#endif
