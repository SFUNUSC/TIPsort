#ifndef SORT_H
#define SORT_H

#define NLAYER 3

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

int pos,col,pin,ring,layer;
int pinRing;

int  hist[NLAYER*NPINRING][S32K];
calibration_parameters* cal_par;
#endif
