#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{
  int trig,diff=0;
  trig=ptr->trigger_num&0x7fffffff;
  if(store>0)
    diff=trig-store;
  if(diff>dmax)
    dmax=diff;
  if(diff<dmin)
    dmin=diff;
  store=trig;

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  if(argc!=2)
    {
      printf("\n ./trig_diff midas_input_data_file_name\n");
      exit(-1);
    }
  
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
  printf("\n"); 
 printf("Program calculates the difference between trigger numbers of consecutive fragments\n");
 printf("The minimum difference is: %8d\n",dmin);
 printf("The maximum difference is: %8d\n",dmax);


}
