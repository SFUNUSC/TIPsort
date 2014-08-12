#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-encoder.h"

#include "TCutG.h"
#include "TFile.h"
#include "TROOT.h"

TCutG *g;

int  idmin,idmax;
double chimin,chimax;

FILE* output;
int enb[BUFFSIZE];


#endif
