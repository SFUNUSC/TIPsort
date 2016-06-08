#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

#include "TH2D.h"
#include "TFile.h"

#define MAXNPART 10
#define PI 3.141592653589793238462643383279502884197

#define NGATES 20

int pos,col,ring,colAddBack;

int  hist[2*NGATES][S32K];
calibration_parameters* cal_par;
double supLow,supHigh,ELow,EHigh;

//quantities for calculation of doppler shift
double theta;//calculated angle
double beta;// v/c of the incoming beam
double beam_p[3];//vector containing initial momentum of the beam (x,y,z)
double part_p[NCSI][3]; //momenta of evaporated particles detected by CsI
double res_p[3]; //momentum of residual particle (which emits gamma ray)
double res_dir[3]; //direction of residual particle (which emits gamma ray)
double gamma_dir[3]; //direction of gamma ray emitted (unit vector in direction of detector)
double vecMag; //placeholder for vector magnitudes
double gates[NGATES+1];
char str[256],outname[256];
int num_gates;
FILE *gateFile;

TH2D *h;
#endif
