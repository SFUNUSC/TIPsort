#ifndef WAVEFORM_ANALYZER_H
#define WAVEFORM_ANALYZER_H

#define NSHAPE     5  //number of trial functions for waveform fit

#define CSI_BASELINE_RANGE 100 //baseline range in channels
#define FILTER 8 //integration region for noise reduction (in samples)
#define NOISE_LEVEL_CSI 100 //noise level for CsI with RF

//bad chi squares for debugging
#define BADCHISQ_NEG         -1 //chisq is negative
#define BADCHISQ_MAT         -2 //matrix for fit is not invertable
#define BADCHISQ_AMPL        -3 //component amplitude < 0
#define BADCHISQ_T0          -4 //finding t0 fails for t0 < 0
#define BADCHISQ_FAIL_DIRECT -5 //fails direct hit


#define EPS        0.001


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "lin_eq_solver.h"

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
  int    mflag; //flag for exclusion zone determination
  
  double t0; //required for compilation of map.c - check that it works
  
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
void      show_CsI_Fit(int, short*,ShapePar*,WaveFormPar*,TApplication*);
int       get_shape_with_RF(int,int, short*,ShapePar*,WaveFormPar*);
void      display_CsI_Fit_with_RF(int, short*,ShapePar*,TApplication*);
Double_t  CsI_FitFunction_with_RF(Int_t,ShapePar*);
void      show_CsI_Fit_with_RF(int, short*,ShapePar*,WaveFormPar*,TApplication*);
double    fit_CsI_waveform(int, short*, ShapePar*, WaveFormPar*);
void      display_CsI_exclusion_zone_and_fit(int, short*,ShapePar*,WaveFormPar*,TApplication*);
void      show_CsI_exclusion_zone_and_fit(int, short*,ShapePar*,WaveFormPar*,TApplication*);

//placeholder functions required for compilation of the sort code
void      get_Griffin_t0(int, short *,WaveFormPar*);



#endif
