#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

#include "TCutG.h"
#include "TFile.h"
#include "TROOT.h"

#define MAXNPART 10
#define amuMeV 931.494061

int pos,col,colAddBack;

int hist[NRING*2][S32K];
int gatehist[NRING*2][S32K];
int projhist[NRING*2][S32K];
calibration_parameters* cal_par;
double supLow,supHigh;
double gateELow, gateEHigh;
double fudgeFactor;

//for PID
TCutG *aGate[NCSI] = { new TCutG() };
TCutG *pGate[NCSI] = { new TCutG() };
int aGateFlag[NCSI] = { 0 };
int pGateFlag[NCSI] = { 0 };
int useCharge;

//quantities for calculation of doppler shift
double ds;//calculated doppler shift based on detector positions
double beta;// v/c of the incoming beam
double beam_p[3];//vector containing initial momentum of the beam (x,y,z)
double part_p[NCSI][3]; //momenta of evaporated particles detected by CsI
double part_dir[NCSI][3]; //direction of evaporated particles detected by CsI
double res_p[3]; //momentum of residual particle (which emits gamma ray)
double res_dir[3]; //direction of residual particle (which emits gamma ray)
double gamma_dir[3]; //direction of gamma ray emitted (unit vector in direction of detector)
int part_type[NCSI]; //type of evaporated particles detected by CsI (0=unknown,1=proton,2=alpha)
double vecMag; //placeholder for vector magnitudes
char str[256];

TH1D *h;
#endif
