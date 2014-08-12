#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

#include "TCutG.h"
#include "TFile.h"
#include "TROOT.h"

TCutG *aGate[NCSI] = { new TCutG() };
TCutG *pGate[NCSI] = { new TCutG() };

double chimin,chimax;
int    idmin,idmax,pos,col;

int  a_hist[NPOSGRIF][NCOL][S32K];
int  p_hist[NPOSGRIF][NCOL][S32K];
int  ap_hist[NPOSGRIF][NCOL][S32K];

calibration_parameters* cal_par;

#endif
