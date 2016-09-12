#ifndef SORT_H
#define SORT_H

#define NRING 7
#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

int hist[NRING][S32K];
int pos,col,ring;

double antiGate[4]={889.5,1021.8,8000,5000};
//double antiGate[4]={889.5,1021.8,1120.8,5000};

calibration_parameters* cal_par;
#endif
