#ifndef __SFU_CAL_CSIARRAY_H
#define __SFU_CAL_CSIARRAY_H

//Prior to putting in 1ns units!!!                                                                                                                                                                                                          
#define RFphase 135.74544

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-cal-common.h"

typedef struct cCSIARRAY
{
  lheader h;
  hit csi[NCSI];
}cCSIARRAY;


typedef struct
{
  int    ceflag[NCSI];
  int    ctflag[NCSI];
  float  ce[NCSI][2];
  float  ct[NCSI][2];
  float  contr_e;
  float  contr_t;
  int    offset;
  int    shift;
  float  elow[NCSI];
  float  ehigh[NCSI];
  float  tlow[NCSI];
  float  thigh[NCSI];
  int    use_time_fit;
  int    DoRFUnwrapping;
} CSIARRAY_calibration_parameters;




void initialize_CSIARRAY_calibration(CSIARRAY_calibration_parameters*, char *);
void read_CSIARRAY_e_cal_par(CSIARRAY_calibration_parameters *, char *);
void read_CSIARRAY_t_cal_par(CSIARRAY_calibration_parameters *, char *);
void read_CSIARRAY_energy_limits(CSIARRAY_calibration_parameters *, char *);
void read_CSIARRAY_time_limits(CSIARRAY_calibration_parameters *, char *);
void summarize_CSIARRAY_calibration(CSIARRAY_calibration_parameters *, char *);
void calibrate_CSIARRAY(raw_event*, CSIARRAY_calibration_parameters *, cCSIARRAY*);

#endif
