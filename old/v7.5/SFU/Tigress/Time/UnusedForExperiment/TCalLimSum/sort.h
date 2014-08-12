#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

int count_1=0,count_2=0,count_3=0;
int count_4=0,count_5=0,count_6=0;
int count_7=0;

int pos,col;

int hist[NPOSTIGR][NCOL][S4K];
calibration_parameters* cal_par;
#endif
