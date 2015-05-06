#ifndef __SFU_CAL_S3_H
#define __SFU_CAL_S3_H


#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-cal-common.h"

typedef struct cS3
{
  lheader sh;
  lheader rh;
  hit sec[NS3SEC];
  hit ring[NS3RING];
}cS3;


typedef struct
{
  int    ceflag[NS3SEC];
  int    ctflag[NS3SEC];
  float  ce[NS3SEC][2];
  float  ct[NS3SEC][2];
  float  contr_e;
  float  contr_t;
  float  elow[NS3SEC];
  float  ehigh[NS3SEC];
  float  tlow[NS3SEC];
  float  thigh[NS3SEC];
} S3sec_calibration_parameters;

typedef struct
{
  int    ceflag[NS3RING];
  int    ctflag[NS3RING];
  float  ce[NS3RING][2];
  float  ct[NS3RING][2];
  float  contr_e;
  float  contr_t;
  float  elow[NS3RING];
  float  ehigh[NS3RING];
  float  tlow[NS3RING];
  float  thigh[NS3RING];
} S3ring_calibration_parameters;

void initialize_S3sec_calibration(S3sec_calibration_parameters*, char*);
void read_S3sec_e_cal_par(S3sec_calibration_parameters *, char *);
void read_S3sec_t_cal_par(S3sec_calibration_parameters *, char *);
void read_S3sec_energy_limits(S3sec_calibration_parameters *, char *);
void read_S3sec_time_limits(S3sec_calibration_parameters *, char *);
void summarize_S3sec_calibration(S3sec_calibration_parameters *, char *);
void calibrate_S3sec(raw_event*, S3sec_calibration_parameters *, cS3*);

void initialize_S3ring_calibration(S3ring_calibration_parameters*, char*);
void read_S3ring_e_cal_par(S3ring_calibration_parameters *, char *);
void read_S3ring_t_cal_par(S3ring_calibration_parameters *, char *);
void read_S3ring_energy_limits(S3ring_calibration_parameters *, char *);
void read_S3ring_time_limits(S3ring_calibration_parameters *, char *);
void summarize_S3ring_calibration(S3ring_calibration_parameters *, char *);
void calibrate_S3ring(raw_event*, S3ring_calibration_parameters *, cS3*);
#endif
