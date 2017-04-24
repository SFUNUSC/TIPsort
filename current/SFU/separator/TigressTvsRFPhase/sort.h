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

int pos,col;
double min_t,max_t;


#endif
