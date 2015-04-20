#ifndef __SFU_CAL_H
#define __SFU_CAL_H


#include "SFU-common.h"
#include "SFU-format.h"
#include "read_master.h"
#include "SFU-cal-common.h"
#include "SFU-cal-TIGRESS.h"
#include "SFU-cal-GRIFFIN.h"
#include "SFU-cal-PINARRAY.h"
#include "SFU-cal-CSIARRAY.h"
#include "SFU-cal-S3.h"

typedef struct calibration_parameters
{
  S3sec_calibration_parameters s3sec;
  S3ring_calibration_parameters s3ring;
  PINARRAY_calibration_parameters pinarray;
  CSIARRAY_calibration_parameters csiarray;
  TIGRESS_calibration_parameters tg;
  GRIFFIN_calibration_parameters gr;
} calibration_parameters;

typedef struct cal_event
{
  cheader h;
  cPINARRAY pinarray;
  cCSIARRAY csiarray;
  cTIGRESS  tg;
  cGRIFFIN  gr;
  cS3       s3;
} cal_event;

void initialize_TIGRESS_PINARRAY_calibration(input_names_type*,calibration_parameters*);
#endif
