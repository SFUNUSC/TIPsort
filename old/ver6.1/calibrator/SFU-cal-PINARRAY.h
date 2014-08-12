#ifndef __SFU_CAL_PINARRAY_H
#define __SFU_CAL_PINARRAY_H


#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-cal-common.h"

typedef struct cPINARRAY
{
  lheader h;
  hit pin[NPIN];
}cPINARRAY;


typedef struct
{
  int    ceflag[NPIN];
  int    ctflag[NPIN];
  float  ce[NPIN][2];
  float  ct[NPIN][2];
  float  contr_e;
  float  contr_t;
  float  elow[NPIN];
  float  ehigh[NPIN];
  float  tlow[NPIN];
  float  thigh[NPIN];
} PINARRAY_calibration_parameters;




void initialize_PINARRAY_calibration(PINARRAY_calibration_parameters*, char *);
void read_PINARRAY_e_cal_par(PINARRAY_calibration_parameters *, char *);
void read_PINARRAY_t_cal_par(PINARRAY_calibration_parameters *, char *);
void read_PINARRAY_energy_limits(PINARRAY_calibration_parameters *, char *);
void read_PINARRAY_time_limits(PINARRAY_calibration_parameters *, char *);
void summarize_PINARRAY_calibration(PINARRAY_calibration_parameters *, char *);
void calibrate_PINARRAY(raw_event*, PINARRAY_calibration_parameters *, cPINARRAY*);

#endif
