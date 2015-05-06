#ifndef SPECBS_H
#define SPECBS_H

//for access to SFU data structure elements
#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

//standard includes for I/O, strings, etc.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

//root includes for histograms, files, etc.
#include "TH2D.h"
#include "TH1D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"

//histogram from total projection spectra
int tproj[NRING*2][S32K];
double dtproj[NRING][S32K];

//histogram from single gated spectra
int gated[NRING*2][S32K];
double dgated[NRING][S32K];

//histograms for smooth background from total projection spectrum
double dbg[NRING][S32K];
double tmp_bg[S32K];
int sbg[NRING][S32K];

//histogram for total background
int tbg[NRING][S32K];

//limits for background
double bLow,bHigh;

//ring energy gate limits
double relow[NRING];
double rehigh[NRING];

//fwhm parameters for background subtraction
double swpars[3];

//background subtracted spectra
int final[NRING][S32K];

int autobkgnd(void);
int find_bg2(int loch, int hich, double *x1, double *y1, double *x2, double *y2);
void read_TIGRESS_ring_energy_gates(char *);


#endif
