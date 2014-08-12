#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-encoder.h"

#include "TCutG.h"
#include "TFile.h"
#include "TROOT.h"

TCutG *Gate[NCSI] = { new TCutG() };

int CsIGateFlag[NCSI] = { 0 };

double chimin,chimax;
int    idmin,idmax,csi;

FILE *output;
int enb[BUFFSIZE];
#endif
