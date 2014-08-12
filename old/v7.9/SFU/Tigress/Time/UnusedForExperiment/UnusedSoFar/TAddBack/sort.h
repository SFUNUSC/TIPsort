#ifndef SORT_H
#define SORT_H

#define NPAIR 6

#include "SFU-common.h"
#include "SFU-format.h"
#include "SFU-decoder.h"

int pos;
int  hist[NPAIR][S4K];
int  ind[NCOL][NCOL]={ {-1, 0, 1, 2},{-1,-1, 3, 4},{-1,-1,-1, 5},{-1,-1,-1,-1}};
#endif
