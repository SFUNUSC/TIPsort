#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"

#define MAXNPART 10
#define PI 3.141592653589793238462643383279502884197
#define	DATASIZE 500000

#define NGATES 20

int pos,col,ring,colAddBack;

int  hist[1][S32K];
calibration_parameters* cal_par;
double supLow,supHigh,slope;
int numBins;

double ds_data[2][DATASIZE];
int ds_data_ind;

//quantities for calculation of doppler shift
double ctheta;//calculated angle
double beta;// v/c of the incoming beam
double beam_p[3];//vector containing initial momentum of the beam (x,y,z)
double part_p[NCSI][3]; //momenta of evaporated particles detected by CsI
double res_p[3]; //momentum of residual particle (which emits gamma ray)
double res_e; //energy of residual particle
double vecMag; //placeholder for vector magnitudes
double gates[NGATES+1];
char str[256],outname[256],tmpfilename[256];
int num_gates;
FILE *gateFile;

TH2D *h,*h2;
TH1D *h3;

#endif
