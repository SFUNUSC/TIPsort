#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

calibration_parameters* cal_par;

FILE * output[NPOSTIGR-1][NCOL][8];
double ecorelow,ecorehigh,eseglow,eseghigh;

#endif
