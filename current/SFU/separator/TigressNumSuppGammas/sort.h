#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-encoder.h"
#include "SFU-cal.h"


FILE* output;
int enb[BUFFSIZE];

double supHigh,supLow;
int num_gammas;

calibration_parameters* cal_par;
#endif
