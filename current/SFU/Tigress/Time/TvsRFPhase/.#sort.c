#ifndef __SFU_CAL_TIGRESS_H
#define __SFU_CAL_TIGRESS_H

#define  NADDBACK 16

//Prior to putting in 1ns units!!!                                                                                                                                                                                                          
#define RFphase 135.74544

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-cal-common.h"

typedef struct cSup
{
  bheader  h;
  hit      sup[NSUP];
}cSup;

typedef struct cSeg
{
  sheader  h;
  int     ring;
  hit     seg[NSEGTIGR];
  double suppress;
}cSeg;

typedef struct cCss
{
  sheader hge;
  sheader hbgo;
  cSeg ge[NCOL];
  cSup bgo[NCOL];
  int  addbackHP;
  int  addbackF;
  int  addbackC;
  int  addbackR;
  hit  addback;
  int  suppress;
}cCss;

typedef struct cTIGRESS
{
 lheader g;
 sheader h;
 cCss det[NPOSTIGR];
}cTIGRESS;


typedef struct
{
  int    ceflag[NPOSTIGR][NCOL];
  int    ctflag[NPOSTIGR][NCOL];
  int    ringflag[NPOSTIGR][NCOL];
  float  ce[NPOSTIGR][NCOL][3];
  float  ct[NPOSTIGR][NCOL][2];
  float  contr_e;
  float  contr_t;
  int    offset;
  int    shift;
  int    ring_map[NPOSTIGR][NCOL];
  float  celow[NPOSTIGR][NCOL];
  float  cehigh[NPOSTIGR][NCOL];
  float  ctlow[NPOSTIGR][NCOL];
  float  cthigh[NPOSTIGR][NCOL];
  float  relow[NRING];
  float  rehigh[NRING];
  int    ttg_use_time_fit;
  int    bgo_use_time_fit;
  int    DoRFUnwrapping;
  int    sup_map[NPOSTIGR][NCOL][NPOSTIGR][NCOL][NSUP];
} TIGRESS_calibration_parameters;



void initialize_TIGRESS_calibration(TIGRESS_calibration_parameters*, char *);
void read_TIGRESS_core_e_cal_par(TIGRESS_calibration_parameters *, char *);
void read_TIGRESS_core_t_cal_par(TIGRESS_calibration_parameters *, char *);
void read_TIGRESS_core_energy_limits(TIGRESS_calibration_parameters *, char *);
void read_TIGRESS_core_time_limits(TIGRESS_calibration_parameters *, char *);
void read_TIGRESS_ring_map(TIGRESS_calibration_parameters *, char *);
void read_TIGRESS_suppression_map(TIGRESS_calibration_parameters *, char *);
void read_TIGRESS_ring_energy_gates(TIGRESS_calibration_parameters *, char *);
void summarize_TIGRESS_calibration(TIGRESS_calibration_parameters *, char *);
void calibrate_TIGRESS(raw_event *, TIGRESS_calibration_parameters *, cTIGRESS *);

#endif
