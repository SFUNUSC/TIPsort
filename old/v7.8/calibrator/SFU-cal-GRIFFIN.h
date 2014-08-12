//Comments by PJV. Code copied, then modified, from SFU-cal-TIGRESS.h
#ifndef __SFU_CAL_GRIFFIN_H
#define __SFU_CAL_GRIFFIN_H

//We can't do clover-to-clover addback,
//but I'll leave the definition here.
#define  NADDBACK 16

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-cal-common.h"

typedef struct cGSeg
{
  sheader  h;
  int     ring;
  hit     seg[NSEGGRIF];
}cGSeg;

typedef struct cGCss
{
  sheader hge;
  cGSeg ge[NCOL];
  int  addbackHP;
  int  addbackF;
  int  addbackC;
  hit  addback;
}cGCss;

typedef struct cGRIFFIN
{
 lheader g;
 sheader h;
 cGCss det[NPOSGRIF];
}cGRIFFIN;

typedef struct
{
  int    ceflag[NPOSGRIF][NCOL];
  int    ctflag[NPOSGRIF][NCOL];
  int    ringflag[NPOSGRIF][NCOL];
  float  ce[NPOSGRIF][NCOL][3];
  float  ct[NPOSGRIF][NCOL][2];
  float  contr_e;
  float  contr_t;
  int    offset;
  int    shift;
  int    ring_map[NPOSGRIF][NCOL];
  float  celow[NPOSGRIF][NCOL];
  float  cehigh[NPOSGRIF][NCOL];
  float  ctlow[NPOSGRIF][NCOL];
  float  cthigh[NPOSGRIF][NCOL];
} GRIFFIN_calibration_parameters;

void initialize_GRIFFIN_calibration(GRIFFIN_calibration_parameters*, char *);
void read_GRIFFIN_core_e_cal_par(GRIFFIN_calibration_parameters *, char *);
void read_GRIFFIN_core_t_cal_par(GRIFFIN_calibration_parameters *, char *);
void read_GRIFFIN_core_energy_limits(GRIFFIN_calibration_parameters *, char *);
void read_GRIFFIN_core_time_limits(GRIFFIN_calibration_parameters *, char *);
void read_GRIFFIN_ring_map(GRIFFIN_calibration_parameters *, char *);
void summarize_GRIFFIN_calibration(GRIFFIN_calibration_parameters *, char *);
void calibrate_GRIFFIN(raw_event *, GRIFFIN_calibration_parameters *, cGRIFFIN *);

#endif
