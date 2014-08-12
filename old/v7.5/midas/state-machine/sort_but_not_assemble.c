#include "sort_but_not_assemble.h"

/*================================================================*/
int get_fragments(long* proc)
{
   int *bank_data, items;
   char *bank_name;
   Tig10_event tig10_event;
   short waveform[MAX_SAMPLES];
   int process_waveforms=1;

  /* loop over banks, looking for WFDN ones (or others ...)  */
   while(1){
      if( (items = next_bank( &bank_name, &bank_data )) < 0 ){ return(-1); }
      //      fprintf(stdout,"bank:%s length:%d\n", bank_name, items);
      if( strcmp(bank_name,"WFDN") ){ continue; } /* ignore other banks */
      swapInt( (char *)bank_data, items*sizeof(int) );
      unpack_tig10_bank( bank_data, items, &tig10_event, process_waveforms,waveform);
      if((*proc%10000)==0) 
	printf("Number of analyzed fragments is %8ld\r",*proc);     
      analyze_fragment(&tig10_event,waveform);
      (*proc)++;
   }
   
   return(-1); /* go to next event */
}

/*================================================================*/
void sort_but_not_assemble(char* inp_name)
{
  FILE* input;
  int state=END_OF_RECORD;
  int stop=0;
  long proc=0;

 
 /* open the input file*/
  if((input=fopen(inp_name,"r"))==NULL)
    {
      printf("\nI can't open input file %s\n",inp_name);
      exit(-2);
    }
 
  while(stop==0){
    switch(state){
    case END_OF_RECORD:
      if( next_record(input) <= 0 ){ state = END_OF_FILE;}
      else {state = NEXT_EVENT;}
      break;
    case NEXT_EVENT:
      if( next_event() < 0 ){ state = END_OF_RECORD; }
      else { state = GET_FRAGMENTS; }
      break;
    case GET_FRAGMENTS:
      if( get_fragments(&proc) < 0 ){ state = NEXT_EVENT;}
      else { state = GET_FRAGMENTS;  }
      break;
    case END_OF_FILE:
      fclose(input);
      printf("\n Number of analyzed fragments is %8ld\n",proc);
      printf("\n");
      stop=1;
      break;
    default:
      printf("Unknown case\n");
      exit(0);
      break;
    }
  }
 
}
