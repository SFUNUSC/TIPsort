#include "sort.h"

int analyze_data(raw_event *data)
{
  if((data->h.setupHP&PINArray_BIT)!=0)
    {
      display_pinarray(&data->pinarray);
      getc(stdin);
    }
  return SEPARATOR_DISCARD;
}
/*==========================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  
  if(argc!=2)
    {
      printf("display_PINArray sfu_input_data\n");
      exit(-1);
    }
 
  printf("Program displays PINArray events.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name); 
}
