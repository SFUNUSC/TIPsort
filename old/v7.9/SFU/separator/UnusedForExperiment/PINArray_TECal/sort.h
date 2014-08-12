#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-encoder.h"
#include "SFU-cal.h"

#include "TCutG.h"
#include "TFile.h"
#include "TROOT.h"

TCutG *Gate = new TCutG();

calibration_parameters* cal_par;

FILE *output;
int enb[BUFFSIZE];

#endif
