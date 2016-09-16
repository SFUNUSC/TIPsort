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

int pos;

int thrtype;//threshold type: 0=take events below threshold, 1=take events above threshold
double threshold; //threshold PID value


#endif
