#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

#include "TCutG.h"
#include "TFile.h"
#include "TROOT.h"

TCutG *g = new TCutG();

int  idmin,idmax;
double chimin,chimax;
calibration_parameters* cal_par;

#endif
