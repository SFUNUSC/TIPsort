#include "sort.h"

int analyze_fragment(Tig10_event* ptr, short* waveform)
{
  int tr;
  unsigned long long ts; //64 bit integer with 0 min

  //printf("tr %8d\n",ptr->trigger_num&0x0fffffff);
  tr=ptr->trigger_num&0x0fffffff;
  if(tr==trigger)
    {
      /* convert to single integer; bitwise inclusive OR is equivalent */
      /* to addition since no bits are shared */
      printf("ts %8.8x ts_up %8.8x\n",ptr->timestamp&0x00ffffff,ptr->timestamp_up&0x00ffffff);
      printf("ts %d ts_up %d\n",ptr->timestamp&0x00ffffff,ptr->timestamp_up&0x00ffffff);

      ts=(ptr->timestamp_up&0x00ffffff)<<20;
      printf("ts_up shifted %12.12llx\n",ts);
      printf("ts_up shifted %lld\n",ts);
      ts|=(ptr->timestamp&0x00ffffff);
      printf("timestamp %12.12llx\n",ts);
      printf("timestamp %lld\n",ts);


      if(ts<ts_min)
	ts_min=ts;
      if(ts>ts_max)
	ts_max=ts;

      getc(stdin);
    }
  
  return 0;
}
/*========================================================================*/
int main(int argc, char *argv[])
{

  one=1;
  ts_min=one<<63;
  ts_max=0;
 
  if(argc!=3)
    {
      printf("trig_timeStamp midas_input_data trigger_number\n");
      exit(-1);
    }
  
  trigger=atoi(argv[2]);
  printf("Program prints timestamps for a given channel\n");
  sort_but_not_assemble(argv[1]);
  printf("\nResults:\nts_min %lx ts_max %lx delta %lx\n",ts_min,ts_max,ts_max-ts_min);
  printf("ts_min %ld ts_max %ld delta %ld\n",ts_min,ts_max,ts_max-ts_min);

}
