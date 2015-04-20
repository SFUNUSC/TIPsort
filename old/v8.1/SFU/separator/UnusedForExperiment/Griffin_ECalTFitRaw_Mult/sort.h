#ifndef SORT_H
#define SORT_H

//MAXGATE is an arbitrary upper limit
//on how many gates can generate different
//output data file formats
#define  MAXGATE 20
#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-encoder.h"
#include "SFU-cal.h"

#include "TCutG.h"
#include "TFile.h"
#include "TROOT.h"

TCutG *Gate[MAXGATE] = { new TCutG() };
calibration_parameters* cal_par;

FILE *out, *output[MAXGATE];
int numMax=0,i=0;
int enb[BUFFSIZE];

#endif
