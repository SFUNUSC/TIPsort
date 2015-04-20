#ifndef SORT_H
#define SORT_H

#define NLAYER 3

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

int pos,col,pin,ring,layer;

int  hist[NLAYER*4][S32K];
calibration_parameters* cal_par;
#endif
