#ifndef MATRIXBSUB_H
#define MATRIXBSUB_H

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

//matrix histogram from input file
short int mat[S4K][S4K];

//background subtracted matrix
short int matBS[S4K][S4K];

//background matrix
float matBG[S4K][S4K];

//float histograms for reading in the projection and background from .spe
float proj[S4K],bg[S4K];

//name buffer for reading the spectra headers
char buf[32];

//root histogram for the background subtracted matrix and background matrix
TH2D *hMatBS;
TH2D *hBG;

//output projection histogram for the background subtracted matrix
int projOut[S4K];

#endif
