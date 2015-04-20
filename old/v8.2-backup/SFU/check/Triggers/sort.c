#include "sort.h"

int analyze_data(raw_event *data)
{
  int t;
  
  t=data->h.trig_num&0x7fffffff;
  if(first==-1) first=t;
  last=t;
  if(t<=store)
    {
      wrong++;
      printf("Wrong trigger order: event #%10d before event #%10d.\n",store,t);
    }
  if((t-store)!=1)
    for(int i=store+1;i<t;i++)
      {
	printf("Event# %10d is missing.\n",i);
	miss++;
      }
  store=t;
  return SEPARATOR_DISCARD;
}
/*==============================================================================*/
int main(int argc, char *argv[])
{
  input_names_type *name;
  
  if(argc!=2)
    {
      printf("check_Triggers sfu_input_data\n");
      exit(-1);
    }
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  printf("Program verifies trigger numbering sequence \n");
  
  strcpy(name->fname.inp_data,argv[1]);
  sort(name); 
  printf("First trigger                          is %10d\n",first);
  printf("Last trigger                           is %10d\n",last);
  printf("Number of incorrectly ordered triggers is %10d\n",wrong);
  printf("Number of missing triggers             is %10d\n",miss);
}
