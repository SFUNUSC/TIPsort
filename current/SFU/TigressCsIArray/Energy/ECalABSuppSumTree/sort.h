#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"
#include "SFU-cal.h"

#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"

#define MAXNPART 10

#define NGATES 20

typedef struct{
  Int_t    CsIfold;
  Int_t    CsIId[NCSI];
  Double_t    CsIE[NCSI];
  Double_t ds;
  Int_t    suppFlag;
}TreeData;

TTree *tree;

TreeData TData;

int pos,col,ring,colAddBack;

calibration_parameters* cal_par;
double supLow,supHigh;

//quantities for calculation of doppler shift
double ds;//calculated doppler shift based on detector positions
double beta;// v/c of the incoming beam
double beam_p[3];//vector containing initial momentum of the beam (x,y,z)
double part_p[NCSI][3]; //momenta of evaporated particles detected by CsI
double res_p[3]; //momentum of residual particle (which emits gamma ray)
double res_dir[3]; //direction of residual particle (which emits gamma ray)
double gamma_dir[3]; //direction of gamma ray emitted (unit vector in direction of detector)
double vecMag; //placeholder for vector magnitudes
double gates[NGATES+1];
char str[256];
int num_gates;
FILE *gateFile;

#endif
