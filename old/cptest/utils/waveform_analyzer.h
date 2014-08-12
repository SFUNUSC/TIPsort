#ifndef WAVEFORM_ANALYZER_H
#define WAVEFORM_ANALYZER_H

#define EPS        0.001
#define RANGE      64
#define GRIFFIN_BASELINE_RANGE 16
#define BADCHISQ   999999
#define SIGN2NOISE 0
#define NSHAPE     5
#define T10LOW     50
#define T10HIGH    200

#define T50LOW     100
#define T50HIGH    300

#define T90LOW     200
#define T90HIGH    800

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "lin_eq_solver.h"



typedef struct
{
  double chisq;
  int    ndf;
  int    type;
  double t[NSHAPE];
  double am[NSHAPE];
}ShapePar;

typedef struct
{
  int    baseline_range;
  double b;
  double bsq;
  double max;
  double tmax;
  double sig2noise;
  int    bflag;
  int    mflag;
  int t10_flag;
  int t30_flag;
  int t50_flag;
  int t90_flag;
  int t10t90_flag;
  int tlmin;
  int tlmax;
  double t90;
  double t50;
  double t30;
  double t10;
  double t10t90;
  double t00;
  double b0;
  double b1;
  double s0;
  double s1;
  double s2;
}WaveFormPar;



void get_baseline(int, short*, WaveFormPar*);
void get_tmax(int, short*, WaveFormPar*);
void get_sig2noise(int, short*, WaveFormPar*);
double  get_tfrac(int, short*, WaveFormPar*,double,double,double);
void    get_t10(int, short*, WaveFormPar*);
void    get_t30(int, short*, WaveFormPar*);
void    get_t50(int, short*, WaveFormPar*);
void    get_t90(int, short*, WaveFormPar*);
void    get_t10t90(int, short*, WaveFormPar*);
double  find_t0(int,ShapePar*,WaveFormPar*,lin_eq_type);
int     get_shape(int,int, short*,ShapePar*,WaveFormPar*);
double  fit_waveform(int, short*,ShapePar*,WaveFormPar*);
void    get_Griffin_t0(int, short *,WaveFormPar*);
void    get_CsI_t0(int, short *,ShapePar*,WaveFormPar*);
#endif
