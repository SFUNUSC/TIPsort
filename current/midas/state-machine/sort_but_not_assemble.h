#ifndef STATE_MACHINE_NOT_ASSEMBLE_H
#define STATE_MACHINE_NOT_ASSEMBLE_H

#define NEXT_EVENT        0
#define GET_FRAGMENTS     1
#define END_OF_RECORD     2
#define END_OF_FILE       3
#define ANALYZE_DATA      4
#define END_OF_SORT       5

#include "tig-format.h"
#include "midas-format.h"

extern int analyze_fragment(Tig10_event*,short*);
void sort_but_not_assemble(char*);
void sort_but_not_assemble_list(char*);
int  get_fragments(long*);

#endif
