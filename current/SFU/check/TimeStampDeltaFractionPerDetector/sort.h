#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"

int pos,col,csi;
int delta;
unsigned long long int numEvents[NPOSTIGR][NCOL],numBadEvents[NPOSTIGR][NCOL];
unsigned long long int numCsIEvents[NCSI],numBadCsIEvents[NCSI];

#endif
