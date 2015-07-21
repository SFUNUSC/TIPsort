#ifndef SORT_H
#define SORT_H

#define NRING 7
#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

int hist[NRING][S32K];
int pos,col,ring;
calibration_parameters* cal_par;
#endif
