#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

int pos,col,ring,colAddBack;
double supLow,supHigh;
int fold;

double dshift[NRING+1];
char str[256];
int num_rings;
FILE *shiftFile;

int  hist[NRING*2][S32K];
calibration_parameters* cal_par;
#endif
