#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-encoder.h"
#include "SFU-cal.h"

FILE* output;
int enb[BUFFSIZE];
calibration_parameters* cal_par;

double minAngle,maxAngle;

double res_dir[2][3]; //direction of detected particles in CsI Array
double res_dir_mag[2]; //magnitude of the above vectors


#endif
