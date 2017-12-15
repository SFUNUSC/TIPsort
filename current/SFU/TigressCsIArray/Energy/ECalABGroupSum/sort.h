#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"
#include "TCutG.h"
#include "TFile.h"
#include "TROOT.h"
#include "TRandom3.h"

int hist[NGROUP][S32K];
calibration_parameters* cal_par;
TCutG *rGate[NCSI] = { new TCutG() };
int rGateFlag[NCSI] = { 0 };
int use_gates;
TRandom3 *randGen;

#endif
