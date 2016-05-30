#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-encoder.h"
#include "SFU-cal.h"

#include "TH2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"

TH1D *h;
TH1D *g;

FILE* output;
int enb[BUFFSIZE];

calibration_parameters* cal_par;
double low,high;

//quantities for calculation of doppler shift
double ds;//calculated doppler shift based on detector positions
double beta;// v/c of the incoming beam
double beam_p[3];//vector containing initial momentum of the beam (x,y,z)
double part_p[NCSI][3]; //momenta of evaporated particles detected by CsI
double res_p[3]; //momentum of residual particle (which emits gamma ray)
double res_e; //energy of residual particle
double res_dir[3]; //direction of residual particle (which emits gamma ray)
double gamma_dir[3]; //direction of gamma ray emitted (unit vector in direction of detector)
double vecMag; //placeholder for vector magnitudes

#endif
