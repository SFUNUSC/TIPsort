/* Kris waveform analyzer */
#ifndef WAVEFORM_ANALYZER_H
#define WAVEFORM_ANALYZER_H

#define EPS        0.001
#define GRIFFIN_BASELINE_RANGE 16 
#define CSI_BASELINE_RANGE 32
#define TIFFIN_BASELINE_RANGE 128
#define TIFFIN_RISE 1000
#define TIFFIN_LL_SHIFT 100
#define STANDARD_EXCLUSION_WIDTH 10
#define BADCHISQ       -1
#define BADCHISQ_AMPL  -2
#define T0FAIL         -3

#define SIGN2NOISE 0
#define NOISE_LEVEL_CSI 100
#define NOISE_LEVEL_GRIFFIN 10
#define NOISE_LEVEL_TIFFIN 50
#define NSHAPE     5  //number of trial functions for waveform fit
#define FILTER     8 //integration region for noise reduction (in samples)
#define T10LOW     50  //low limit for T10
#define T10HIGH    300 //high linit for T10

#define T50LOW     100
#define T50HIGH    300

#define T90LOW     200
#define T90HIGH    800

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "lin_eq_solver.h"
#include "tig-format.h"

#include "TH1D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TROOT.h"
#include "math.h"

typedef struct
{
  long double chisq;
  long double chisq_ex;
  long double chisq_f;
  int    ndf;
  int    ndf_ex;
  int    ndf_f;
  int    type;
  long double t[NSHAPE];
  long double am[NSHAPE];
  long double rf[5];
}ShapePar;

typedef struct
{
  int    baseline_range;
  double b;
  double bsq;
  double max;
  double tmax;
  int    temax;
  int    temin;
  double sig2noise;
  int    bflag;
  int    mflag;
  int t10_flag;
  int t30_flag;
  int t50_flag;
  int t90_flag;
  int t10t90_flag;
  double t90;
  double t50;
  double t30;
  double t10;
  double t10t90;
  double t00;
  int    thigh;
  double b0;
  double b1;
  long double s0;
  long double s1;
  long double s2;
}WaveFormPar;



void      get_baseline(int, short*, WaveFormPar*);
void      get_tmax(int, short*, WaveFormPar*);
void      get_sig2noise(int, short*, WaveFormPar*);
double    get_tfrac(int, short*, WaveFormPar*,double,double,double);
void      get_t10(int, short*, WaveFormPar*);
void      get_t30(int, short*, WaveFormPar*);
void      get_t50(int, short*, WaveFormPar*);
void      get_t90(int, short*, WaveFormPar*);
void      get_t10t90(int, short*, WaveFormPar*);
double    find_t0(int,ShapePar*,WaveFormPar*,lin_eq_type);
int       get_shape(int,int, short*,ShapePar*,WaveFormPar*);
int       get_shape_with_RF(int,int, short*,ShapePar*,WaveFormPar*);
double    fit_waveform(int, short*,ShapePar*,WaveFormPar*,TApplication*);
double    fit_waveform_with_RF(int, short*,ShapePar*,WaveFormPar*);
void      get_Griffin_t0(int, short *,WaveFormPar*);
void      get_TIFFIN_t0(int, short *,WaveFormPar*);
void      get_exclusion_zone_for_Griffin(int,short*, WaveFormPar*);
void      get_exclusion_zone_for_TIFFIN(int,short*, WaveFormPar*);
int       get_TIFFIN_shape(int, short*,ShapePar*,WaveFormPar*);
double    fit_TIFFIN_waveform(int, short*,ShapePar*,WaveFormPar*);
void      print_WavePar(WaveFormPar*);
void      print_ShapePar(ShapePar*);
void      display_waveform(int, short*,TApplication*);
void      CsI_get_exclusion_zone(int,short*, WaveFormPar*);
Double_t  CsI_FitFunction(Int_t,ShapePar*);
Double_t  CsI_FitFunctionWithRF(Int_t,ShapePar*);
void      CsI_DisplayFitWithRF(int, short*,ShapePar*,TApplication*);
void      CsI_ShowFitWithRF(int, short*,ShapePar*,WaveFormPar*,TApplication*);
void      CsI_DisplayFit(int, short*,ShapePar*,TApplication*);
void      CsI_ShowFit(int, short*,ShapePar*,WaveFormPar*,TApplication*);
void      CsI_AddChiSqInTheExclusionZone(short*,ShapePar*,WaveFormPar*);
double    CsI_get_tau(int,ShapePar*);
void      get_CsI_t0(int, short *,ShapePar*,WaveFormPar*);
void CsI_show_exclusion_zone(Int_t,short*,WaveFormPar*, TApplication*);
#endif
