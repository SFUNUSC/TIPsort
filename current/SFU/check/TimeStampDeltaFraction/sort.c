#include "sort.h"

int analyze_data(raw_event *data)
{
	unsigned long long int min,max;
  if(verify_ts(data,&min,&max)>delta)
    {
      numBadEvents++;
    }
  numEvents++;
  return SEPARATOR_DISCARD;
}
/*========================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  
  if(argc!=3)
    {
      printf("check_TimeStampDeltaFraction sfu_input_data delta\n");
      printf("Program checks the number of events with time stamp inconsistency larger than delta.\n");
      exit(-1);
    }
  
  delta=atoi(argv[2]);
  printf("Program checks the number of events with time stamp inconsistency larger than delta.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);

  sort(name); 
  
  printf("Number of events in input data: %Li\n",numEvents);
  printf("Number of events in input data with inconsistency > %i: %Li\n",delta,numBadEvents);
  printf("Fraction of inconsistent events: %f\n",numBadEvents/(numEvents*1.0));
}
