#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

calibration_parameters* cal_par;

int pos,col,csi,ring;
unsigned long long int numEvents[NPOSTIGR][NCOL],ringNumEvents[NRING],totalNumEvents;
unsigned long long int numCsIEvents[NCSI],ringNumCsIEvents[NCSIRING],totalNumCsIEvents;

#endif
