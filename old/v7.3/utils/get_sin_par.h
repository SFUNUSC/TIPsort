#ifndef GET_SIN_PAR_H
#define GET_SIN_PAR_H

#define TWOPI 6.283185307

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "lin_eq_solver.h"

typedef struct
{
  double A;
  double t0;
  double C;
}StructSinPar;

int get_sin_par(double ,int,short*, StructSinPar*);
#endif
