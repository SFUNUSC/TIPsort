#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

calibration_parameters* cal_par;

int hist[NPIN][S32K];
int pos,col,pin;

#endif
