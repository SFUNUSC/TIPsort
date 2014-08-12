#ifndef MIDAS_FORMAT_H
#define MIDAS_FORMAT_H

#include <stdio.h>
#include <time.h>
#include <string.h>

void  swapInt (char*, int);
void  swapShort(char*, int);
void  swapWords (char*, int);
int   next_record(FILE*);
int   next_event();
int   next_bank(char **, int **);
int   unpack_bank(unsigned short*, int,int);


#endif
