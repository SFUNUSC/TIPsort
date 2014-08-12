#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{
  trig         = ptr->trigger_num&0x7fffffff;
  timestamp    = ptr->timestamp;
  timestamp_up = ptr->timestamp_up;
  channel      = ptr->channel;

  if(input_trig_num==trig)
    {
      fragment++;
      printf("\nFragment %d in channel %d of trigger %d\n",fragment,channel,trig);
      printf("---has timestamp %8.8x and timestamp_up %8.8x\n",timestamp,timestamp_up);
      printf("---has charge    %8.8x and cfd          %8.8x\n\n",ptr->charge,ptr->cfd);
    }

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  if(argc!=3)
    {
      printf("\n ./trig_inspect midas_input_data_file_name trigger_number\n");
      exit(-1);
    }
  input_trig_num=atoi(argv[2]);
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("\n"); 
  printf("Program print fragment information for a given trigger\n");
}
