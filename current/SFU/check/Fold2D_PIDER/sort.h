#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"

#include "TH2D.h"
#include "TCutG.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"

FILE* output;
int enb[BUFFSIZE];

TCutG *aGate[NCSI] = { new TCutG() };
TCutG *pGate[NCSI] = { new TCutG() };

int aGateFlag[NCSI] = { 0 };
int pGateFlag[NCSI] = { 0 };

int pos;

TH2D *h;
#endif
