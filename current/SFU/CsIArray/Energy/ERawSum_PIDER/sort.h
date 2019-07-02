#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

#include "TCutG.h"
#include "TFile.h"
#include "TROOT.h"

//for PID
TCutG *aGate[NCSI] = { new TCutG() };
TCutG *pGate[NCSI] = { new TCutG() };
int aGateFlag[NCSI] = { 0 };
int pGateFlag[NCSI] = { 0 };
int useCharge;

int  hist[NCSI*2][S32K];
int pos;

calibration_parameters* cal_par;
int part_type[NCSI]; //type of evaporated particles detected by CsI (0=unknown,1=proton,2=alpha)

#endif
