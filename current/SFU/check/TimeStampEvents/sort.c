#include "sort.h"

int analyze_data(raw_event *data)
{
	unsigned long long int min,max;
  verify_ts(data,&min,&max);
  min=min&0x00ffffff;
  max=max&0x00ffffff;
  //printf("Min: %Li, Max: %Li, ts: %Li\n",min,max,ts);
  if((min==ts)||(max==ts))
  	{
  		printf("\n\n\n");
  		display_raw_event(data);
  		numEvents++;
    }
  
  return SEPARATOR_DISCARD;
}
/*========================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  numEvents=0;
  
  if(argc!=3)
    {
      printf("check_TimeStampEvents sfu_input_data timestamp\n");
      printf("Program displays all events containing the specified (integer) time stamp.\n");
      printf("In a perfect world, timestamps should be unique to a single event.\n");
      exit(-1);
    }
  
  ts=(unsigned long long)atoi(argv[2]);
  ts=ts&0x00ffffff;
  printf("Program displays all events containing the specified time stamp.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);

  sort(name); 
  
  printf("\n\n\nNumber of events matching timestamp: %i\n",numEvents);
}
