#ifndef WAVEFORM_ANALYZER_H
#define WAVEFORM_ANALYZER_H

#define NSHAPE     5  //number of trial functions for waveform fit

#define CSI_BASELINE_RANGE 50 //baseline range in channels
#define FILTER 8 //integration region for noise reduction (in samples)
#define NOISE_LEVEL_CSI 100 //noise level for CsI
#define SIGN2NOISE 0 //signal to noise ratio for get_sig2noise

//bad chi squares for debugging
#define BADCHISQ_NEG         -1024-1 //chisq is negative
#define BADCHISQ_SMOOTH_T0   -1024-2 //smooth_t0 gives bad result
#define BADCHISQ_PAR_T0      -1024-3 //parabolic_t0 gives bad result
#define BADCHISQ_LIN_T0      -1024-4 //linear_t0 gives bad result
#define BADCHISQ_MAT         -1024-5 //matrix for fit is not invertable
#define BADCHISQ_AMPL        -1024-6 //component amplitude < 0
#define BADCHISQ_T0          -1024-7 //finding t0 fails for t0 < 0
#define BADCHISQ_FAIL_DIRECT -1024-8 //fails direct hit
#define BAD_EXCLUSION_ZONE   -1024-9 //fails finding exclusion zone

#define EPS        0.001
#define MAX_VALUE 16380 //max waveform value from ADC

//new definitions for Kris' changes to the waveform analyzer
#define PIN_BASELINE_RANGE 16
#define BAD_MAX            -1024-10
#define BAD_BASELINE_RANGE -1024-11
#define LARGECHISQ         1E111
#define T10LOW             50  //low limit for T10
#define T10HIGH            300 //high linit for T10
#define T50LOW             100
#define T50HIGH            300
#define T90LOW             200
#define T90HIGH            800
#define T0RANGE            8

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "lin_eq_solver.h"
#include "tig-format.h"

//For ROOT
#include "TH1D.h"
#include "TF1.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TROOT.h"
#include "math.h"

typedef struct
{
  double chisq;
  int    ndf;
  int    type;
  long double t[NSHAPE]; //decay constants for the fits
  long double am[NSHAPE]; //associated aplitudes for the decay constants
  double rf[5];

  //new stuff necessary for compiliation of Kris' waveform analyzer changes
  long double chisq_ex;
  long double chisq_f;
  int    ndf_ex;
  int    ndf_f;
  
}ShapePar;

typedef struct
{
  //parameters for baseline
  int    baseline_range;
  double baseline; //baseline
  double baselineStDev; //baseline variance
  int    bflag; //flag for baseline determiniation

  //paremeters for exclusion zone
  double max; //max value of the filtered waveform 
  double tmax; //x position of the max value of the filtered waveform
  double baselineMin; //max crossing point for exclusion zone
  double baselineMax; //max crossing point for exclusion zone
  int    temax; //x position, exclusion zone upper limit
  int    temin; //x position, exclusion zone lower limit
  double afit,bfit; //parameters for the line which fits risetime above temax
  int    mflag; //flag for tmax found
  int    teflag; //flag for exclusion zone determined
  
  double t0; //required for compilation of map.c - check that it works
  double t0_local;

  //new stuff necessary for compiliation of Kris' waveform analyzer changes
  double b0;
  double b1;
  long double s0;
  long double s1;
  long double s2;
  double t90;
  double t50;
  double t30;
  double t10;
  double t10t90;
  int t10_flag;
  int t30_flag;
  int t50_flag;
  int t90_flag;
  int t10t90_flag;
  int    thigh;
  double sig2noise;

}WaveFormPar;

void      print_WavePar(WaveFormPar*);
void      print_ShapePar(ShapePar*);
void      get_baseline(int, short*, WaveFormPar*);
void      get_tmax(int, short*, WaveFormPar*);
void      get_exclusion_zone_for_CsI(int,short*, WaveFormPar*);
void      show_CsI_exclusion_zone(int,short*,WaveFormPar*, TApplication*);
int       get_shape(int,int, short*,ShapePar*,WaveFormPar*);
double    get_t0(int,ShapePar*,WaveFormPar*,lin_eq_type);
double    get_CsI_tau(int,ShapePar*);
void      display_CsI_Fit(int, short*,ShapePar*,TApplication*);
Double_t  CsI_FitFunction(Int_t,ShapePar*);
Double_t  CsI_FitFunction_disp(Double_t*,Double_t*);
Double_t  CsI_FitFunction_localt0(Int_t,ShapePar*,WaveFormPar*);
void      show_CsI_Fit(int, short*,ShapePar*,WaveFormPar*,TApplication*);
int       get_shape_with_RF(int,int, short*,ShapePar*,WaveFormPar*);
void      display_CsI_Fit_with_RF(int, short*,ShapePar*,TApplication*);
Double_t  CsI_FitFunction_with_RF(Int_t,ShapePar*);
void      show_CsI_Fit_with_RF(int, short*,ShapePar*,WaveFormPar*,TApplication*);
double    fit_CsI_waveform(int, short*, ShapePar*, WaveFormPar*);
void      display_CsI_exclusion_zone_and_fit(int, short*,ShapePar*,WaveFormPar*,TApplication*);
void      show_CsI_exclusion_zone_and_fit(int, short*,ShapePar*,WaveFormPar*,TApplication*);
void      get_CsI_t0_local(int, short *,ShapePar*,WaveFormPar*);
void      get_t10t90(int, short*, WaveFormPar*);
void      get_sig2noise(int, short*, WaveFormPar*);


//placeholder functions required for compilation of the sort code
void      get_Griffin_t0(int, short *,WaveFormPar*);

//New stuff for Kris' PIN diode PID algorithm

typedef struct
{
  double slope;
  double intercept;
  double chisq;
  double ndf;
}LinePar;
 
typedef struct
{
  double constant;
  double linear;
  double quadratic;
  double chisq;
  double ndf;
}ParPar;

Double_t OneComponentFitFunction(Double_t, Double_t);
Double_t OneComponentPlotFunction(Double_t, ShapePar*);
Double_t OneComponentChiSq(int, short*,ShapePar*);
Double_t OneComponentFitWaveform(int, short*,ShapePar*,WaveFormPar*);
int      fit_parabola(short*,int,int,ParPar*);
int      fit_smooth_parabola(short*,int,int,double,ParPar*);
int      fit_line(short*,int,int,LinePar*);
double   get_linear_T0(int,short*,WaveFormPar*);
double   get_parabolic_T0(int,short*,WaveFormPar*);
double   get_smooth_T0(int,short*,WaveFormPar*);
double   fit_newT0(int, short*, WaveFormPar*);
void     display_newT0_fit(int, short*, WaveFormPar* ,TApplication*);
double   get_tfrac(int, short*, WaveFormPar*,double,double,double);
void     get_t10(int, short*, WaveFormPar*);
void     get_t30(int, short*, WaveFormPar*);
void     get_t50(int, short*, WaveFormPar*);
void     get_t90(int, short*, WaveFormPar*);

#endif
