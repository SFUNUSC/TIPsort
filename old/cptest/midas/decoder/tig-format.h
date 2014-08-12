#ifndef TIG_FORMAT_H
#define TIG_FORMAT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "midas-format.h"

#define MAX_SAMPLES  4096
#define CLEAR_SIZE (23*sizeof(int))

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


int unpack_tig10_bank(int *, int, Tig10_event *, int, short*);
void print_fragment_info(Tig10_event*,int);
#endif
