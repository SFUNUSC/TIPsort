#ifndef SORT_H
#define SORT_H

#include "SFU-common.h"
#include "sort_but_not_assemble.h"
#include "TH1D.h"
#include "TVirtualFFT.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TFile.h"
#include "TApplication.h"
#include "math.h"

TH1D *h=NULL;
TH1 *fourier_transform=NULL;
TH1D *FFTsum;
//TFile f("FFTfiltersum.root", "new"); //use if one wants to write histogram to a file
bool firstfragment=true;
TCanvas *c;  
TApplication *theApp;
int  chan,length,number,counter=0;

#endif
