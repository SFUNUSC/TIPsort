#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-encoder.h"
#include "SFU-cal.h"


#include "TCutG.h"
#include "TFile.h"
#include "TROOT.h"

FILE* output;
int enb[BUFFSIZE];

TCutG *aGate[NCSI] = { new TCutG() };
TCutG *pGate[NCSI] = { new TCutG() };

int aGateFlag[NCSI] = { 0 };
int pGateFlag[NCSI] = { 0 };

int pos,gate_np,gate_na;
int useCharge;

#endif
