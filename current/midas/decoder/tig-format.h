#ifndef TIG_FORMAT_H
#define TIG_FORMAT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "midas-format.h"

#define MAX_SAMPLES 4096
#define CLEAR_SIZE (23*sizeof(int))
//PJV: 10/3/13
//This NEG_CHARGE_VALUE will end up
//With a raw energy around 30000 for
//standard Ge K and dispersion values.
#define NEG_CHARGE_VALUE    3750000

typedef struct tig10_event_struct {
   int             trigger_num;
   int                    port;
   int                   tig10;
   int               collector;
   int                 channel;
   int             charge_flag;
   int                cfd_flag;
   int                led_flag;
   int          timestamp_flag;
   int       timestamp_up_flag;
   int           trig_req_flag;
   int           trig_acc_flag;
   int             pileup_flag;
   int           livetime_flag;
   int         waveform_length;
   int                  charge;
   int                     cfd;
   int                     led;
   int               timestamp;
   int            timestamp_up;
   int                trig_req;
   int                trig_acc;
   int                livetime;
} Tig10_event;

typedef struct ts_table
{
	int tableSize; //number of entries in the table
	int allocatedSize; //number of entries allocated in memory
  int*      table; //lookup table mapping trigger numbers to timestamps
}ts_table;

int unpack_tig10_bank(int *, int, Tig10_event *, int, short*,int,ts_table*);
int unpack_tig10_bank(int *, int, Tig10_event *, int, short*);
void print_fragment_info(Tig10_event*,int);
#endif
