#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

int pos,col,ring;
double antiGate[4]={889.5,1021.8,1120.8,1241.3};

int  hist[NRING*2][S32K];
calibration_parameters* cal_par;
#endif
