#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"

#include "TH2D.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TCutG.h"
#include "TFile.h"
#include "TApplication.h"

TCutG *aGate[NCSI] = { new TCutG() };
TCutG *pGate[NCSI] = { new TCutG() };

TH2D *ap_h[NCSI];
TH2D *a_h[NCSI];
TH2D *p_h[NCSI];
TH2D *notInGate_h[NCSI];

int aGateFlag[NCSI] = { 0 };
int pGateFlag[NCSI] = { 0 };
int  pos,idmin,idmax;
double chimin,chimax;

#endif
