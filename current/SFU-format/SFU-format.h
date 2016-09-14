#ifndef __SFU_FORMAT_H
#define __SFU_FORMAT_H

#include "get_sin_par.h"
#include "waveform_analyzer.h"

#define NRING      7
#define NSFUdaq   60
#define NPOSTIGR  17
#define NPOSGRIF   4
#define NSEGTIGR  10
#define NSEGGRIF   2
#define NSUP       5
#define NCOL       4
#define NPIN      45
#define NS3SEC    33
#define NS3RING   25
#define NCSI      41
#define NCSIRING   6

#define MAX_DET_BITS  8
#define TIGRESS_BIT   1
#define PINArray_BIT  2
#define RF_BIT        4
#define BDPIN_BIT     8
#define S3SEC_BIT    16
#define S3RING_BIT   32
#define CsIArray_BIT 64
#define GRIFFIN_BIT  128

#define BUFFER_TAG        0xfafafafa
#define EVENT_TAG         0xabcdabcd
#define CHANNEL_TAG       0xabcd0000
#define HEADER_TAG        0xabcd0001
#define RF_TAG            0xabcd0002
#define TIGRESS_TAG       0xabcd0003
#define CSSTIGR_TAG       0xabcd0004
#define TIGRGe_TAG        0xabcd0005
#define BGO_TAG           0xabcd0006
#define PINArray_TAG      0xabcd0007
#define BDPIN_TAG         0xabcd0008
#define S3SEC_TAG         0xabcd0009
#define S3RING_TAG        0xabcd000a
#define CsIArray_TAG      0xabcd000b
#define CSIFIT_TAG        0xabcd000c
#define GRIFFIN_TAG       0xabcd000d
#define CSSGRIF_TAG       0xabcd000e
#define GRIFGe_TAG        0xabcd000f
#define PINFIT_TAG        0xabcd0011

typedef struct channel
{
  int timestamp;
  int timestamp_up;
  int charge;
  int cfd;
}channel;

typedef struct header
{
  unsigned short Efold;
  unsigned short Tfold;
  unsigned short TSfold;
  unsigned short EHP;
  unsigned short THP;
  unsigned short TSHP;
} header;

typedef struct pheader
{
  int   Efold;
  int   Tfold;
  int   TSfold;
  unsigned long long int EHP;
  unsigned long long int THP;
  unsigned long long int TSHP;
} pheader;

typedef struct rheader
{
  int   trig_num;
  int   timestamp_up_min;
  int   timestamp_up_max;
  int   timestamp_min;
  int   timestamp_max;
  int   setupHP;
  int   frag_fold;
} rheader;

typedef struct theader
{
 unsigned short Gefold;
 unsigned short BGOfold;
 unsigned short GeHP;
 unsigned short BGOHP;
} theader;

typedef struct gheader
{
 int    Posfold;
 int    Gefold;
 int    BGOfold;
 int    Tfold;
 int    PosHP;
 unsigned long long int THP;
 unsigned long long int GeHP;
 unsigned long long int BGOHP;
} gheader;

typedef struct Sup
{
  header  h;
  channel sup[NSUP];
  double  t0[NSUP];
}Sup;

typedef struct SegTIGR
{
  header  h;
  channel seg[NSEGTIGR];
  double  t0[NSEGTIGR];
}SegTIGR;

typedef struct SegGRIF
{
  header  h;
  channel seg[NSEGGRIF];
  double  t0[NSEGGRIF];
}SegGRIF;

typedef struct CssTIGR
{
  theader h;
  SegTIGR ge[NCOL];
  Sup bgo[NCOL];
}CssTIGR;

typedef struct CssGRIF
{
  theader h;
  SegGRIF ge[NCOL];
}CssGRIF;

typedef struct Tigress
{
 gheader g;
 theader h;
 CssTIGR det[NPOSTIGR];
}Tigress;

typedef struct Griffin
{
 gheader g;
 theader h;
 CssGRIF det[NPOSGRIF];
}Griffin;

typedef struct RF
{
  channel ch;
  StructSinPar sin;
}RF;

typedef struct PINArray
{
  pheader  h;
  channel pin[NPIN];
  ShapePar wfit[NPIN];
}PINArray;

typedef struct SFUDAQ
{
  pheader  h;
  channel  chan[NSFUdaq];
}SFUDAQ;

typedef struct CsIArray
{
  pheader  h;
  channel csi[NCSI];
  ShapePar wfit[NCSI];
  double  t0[NCSI];
}CsIArray;

typedef struct PINBD
{
  header  h;
  channel pin;
}PINBD;

typedef struct S3
{
  pheader rh;
  pheader sh;
  channel ring[NS3RING];
  channel sec[NS3SEC];
} S3;

typedef struct raw_event
{
  rheader  h;
  RF       rf;
  Tigress  tg;
  Griffin  gr;
  PINArray pinarray;
  PINBD    pinbd;
  SFUDAQ   SFUdaq;
  S3       s3;
  CsIArray csiarray;
}raw_event;

void display_raw_event(raw_event*);
void display_Tigress(Tigress*);
void display_Griffin(Griffin*);
void display_RF(RF*);
void display_pinarray(PINArray*);
void display_csiarray(CsIArray*);
void display_Css(CssTIGR*);
void display_GCss(CssGRIF*);
void display_S3(S3*);
void display_pinbd(PINBD*);
void display_header(header*);
void display_rheader(rheader*);
void display_pheader(pheader*);
void display_theader(theader*);
void display_gheader(gheader*);
void display_GRIF_theader(theader*);
void display_GRIF_gheader(gheader*);
void display_GRIF_cheader(theader*);
void display_cheader(theader*);
void display_bheader(header*);
void display_pbdheader(header*);
void display_channel(channel*);
void display_channel_legend();
void display_segment_legend();
void display_suppressor_legend();
int verify_ts_pinarray(PINArray*,unsigned long long int*,unsigned long long int*);
int verify_ts_csiarray(CsIArray*,unsigned long long int*,unsigned long long int*);
int verify_ts_Tigress(Tigress*,unsigned long long int*,unsigned long long int*);
int verify_ts_Griffin(Griffin*,unsigned long long int*,unsigned long long int*);
int verify_ts_RF(RF*,unsigned long long int*,unsigned long long int*);
int verify_ts_PINBD(PINBD*,unsigned long long int*,unsigned long long int*);
int verify_ts(raw_event*,unsigned long long int*,unsigned long long int*);
int verify_ts_S3(S3*,unsigned long long int*,unsigned long long int*);

#endif
