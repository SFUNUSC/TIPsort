#ifndef __SFU_DECODER_H
#define __SFU_DECODER_H

#include "SFU-common.h"
#include "SFU-format.h"
#include "read_master.h"

#define NEXT_EVENT        0
#define PROCESSING_EVENT  1
#define END_OF_RECORD     2
#define END_OF_FILE       3
#define ANALYZE_DATA      4

#define SEPARATOR_KEEP 0xffff
#define SEPARATOR_DISCARD 0x1111


int next_event(int **, int *, counters *, int*);
int process_event(int*, raw_event*);
void print_data(int*,int);
void sort(input_names_type*);
extern int analyze_data(raw_event*);
#endif
