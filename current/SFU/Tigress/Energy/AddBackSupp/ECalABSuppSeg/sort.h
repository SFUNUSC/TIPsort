#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

int pos,col,sgm;
double supHigh,supLow;

int  hist[NPOSTIGR][NCOL][NSEGTIGR][S32K];
calibration_parameters* cal_par;
#endif
