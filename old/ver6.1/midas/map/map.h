#ifndef __MAP_H
#define __MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tig-format.h"
#include "SFU-format.h"
#include "get_sin_par.h"
#include "waveform_analyzer.h"

#define NCSIMAP 30
typedef struct tmap
{
int tigr_css_min;
int tigr_css_max;
int grif_css_min;
int grif_css_max;
int grif_fit;
int pinarray_min;
int pinarray_max;
int csiarray_min;
int csiarray_max;
int csiarray_fit;
double csiarray_trc;
double csiarray_tf;
double csiarray_ts;
double csiarray_tr;
int s3_min;
int s3_max;
int pinbd;
int rf;
double rf_period;
int  tigr_ge_map[NPOSTIGR];
int  grif_ge_map[NPOSGRIF];
int  pinarray_map[60];
int  csiarray_map[NCSIMAP];
int  s3_sec[5][10];
int  s3_ring[4][10];
int  s3_gid[10];
int  s3_ord[10];
} tmap;

const int tigr_css_group=60;
const int grif_css_group=10;
const int s3_group=10;


void map_event(Tig10_event *, short*, raw_event*,tmap*,int);
void read_map(char*,tmap*);
void print_map();
#endif
