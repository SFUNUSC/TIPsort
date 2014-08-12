#ifndef STATE_MACHINE_ASSEMBLE_H
#define STATE_MACHINE_ASSEMBLE_H


#define DEPTH 8000000
#define SHIFT  100000
#define NEXT_EVENT        0
#define GET_FRAGMENTS     1
#define END_OF_RECORD     2
#define END_OF_FILE       3
#define ANALYZE_DATA      4

#include "tig-format.h"
#include "midas-format.h"
#include "SFU-format.h"
#include "map.h"

typedef struct data_pointers
{
  int first;
  int last;
  int shift;
  int trig;
  int turn;
  int turn_enable;
  int turn_in_progress;
  unsigned long proc;
  size_t son;
  size_t sot;
  size_t sos;
}data_pointers;

typedef struct node
{
  Tig10_event* eptr;
  short*       wptr;
  struct node *next;
}node;

extern int analyze_assembled_event(raw_event*);
void insert_node(node*, data_pointers*,Tig10_event*,short*);
void remove_event(int,node*);
void assemble_event(int,node*, data_pointers*,tmap*);
void add_tig10_event(Tig10_event*,short*,node*,data_pointers*,tmap*);
void sort_and_assemble(char*,char*);
int  get_and_assemble_fragments(node*,data_pointers*,tmap*);

#endif
