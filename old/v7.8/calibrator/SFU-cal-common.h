#ifndef __SFU_CAL_COMMON_H
#define __SFU_CAL_COMMON_H

#include "SFU-common.h"
#include "SFU-format.h"

typedef struct hit
{
  double E;
  double T;
}hit;

typedef struct cheader
{
  int trig_num;
  unsigned long long int ts;
  int setupHP;
}cheader;


typedef struct lheader
{
  int FE;
  int FT;
  int FH;
  unsigned long long int EHP;
  unsigned long long int THP;
  unsigned long long int HHP; 
}lheader;

typedef struct sheader
{
  int FE;
  int FT;
  int FH;
  int FA;
  int EHP;
  int THP;
  int HHP;
  int AHP;
}sheader;

#endif
