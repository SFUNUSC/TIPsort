#ifndef LIN_EQ_SOLVER_H
#define LIN_EQ_SOLVER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_DIM 20

typedef struct
{
  long double matrix[MAX_DIM][MAX_DIM];
  long double vector[MAX_DIM];
  long double solution[MAX_DIM];
  int    dim;
}lin_eq_type;

int solve_lin_eq(lin_eq_type *lin_eq);
long double  det(int m, lin_eq_type *lin_eq);

#endif
