#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int min,max;
  if(verify_ts(data,&min,&max)>delta)
    {
      printf("Inconsistent time stamps\n");
      printf("max %12.12x min %12.12x diff %8.8x\n",max,min,max-min);
      display_raw_event(data);
      getc(stdin);
    }
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/

int main(int argc, char *argv[])
{

  input_names_type* name;
  

  if(argc!=3)
    {
      printf("\n ./check_TimeStamps sfu_input_data_file_name delta\n");
      exit(-1);
    }
  
  delta=atoi(argv[2]);
  printf("Program check time stamp consistency \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name); 


   
}
