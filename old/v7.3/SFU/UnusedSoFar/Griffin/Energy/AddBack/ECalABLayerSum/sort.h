#ifndef SORT_H
#define SORT_H

#define NLAYER 3
#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

int hist[NLAYER][S32K];
int pos,col,ring,layer;
calibration_parameters* cal_par;
#endif
