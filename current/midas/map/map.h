#ifndef __MAP_H
#define __MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tig-format.h"
#include "SFU-format.h"
#include "get_sin_par.h"
#include "waveform_analyzer.h"

typedef struct tmap
{
int tigr_css_min;
int tigr_css_max;
int tigr_css_kpar;
int tigr_css_disp;
int tigr_ge_fit;
int tigr_bgo_fit;
int grif_css_min;
int grif_css_max;
int grif_css_kpar;
int grif_css_disp;
int grif_fit;
int pinarray_min;
int pinarray_max;
int pinarray_css_kpar;
int pinarray_css_disp;
int pinarray_fit;
double pinarray_trc;
int csiarray_min;
int csiarray_max;
int csiarray_css_kpar;
int csiarray_css_disp;
int csiarray_fit;
double csiarray_trc;
double csiarray_tf;
double csiarray_ts;
double csiarray_tr;
//pileup rejection algorithm parameters
int csiarray_pileup_rej; //flag as to whether pileup is rejected or not
int csiarray_filter_dist;
int csiarray_averaging_samples;
int csiarray_fall_amount;
int csiarray_rise_amount;
//end pileup rejection algorithm parameters
int s3_min;
int s3_max;
int s3_css_kpar;
int s3_css_disp;
int pinbd;
int rf;
double rf_period;
int  tigr_ge_map[NPOSTIGR];
int  grif_ge_map[NPOSGRIF];
int  pinarray_map[60];
int  csiarray_map[2*NCSI]; //allow more space in case detector ports are skipped over on the DAQ
int  s3_sec[5][10];
int  s3_ring[4][10];
int  s3_gid[10];
int  s3_ord[10];
int ts_order;//whether or not to order events by timestamp (rather than trigger number)
} tmap;

const int tigr_css_group=60;
const int grif_css_group=10;
const int s3_group=10;


void map_event(Tig10_event *, short*, raw_event*,tmap*,int);
void read_map(char*,tmap*);
void print_map();
#endif
