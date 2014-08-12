#include "sort.h"

int analyze_data(raw_event *data)
{
  if(data->h.setupHP==TrigBit)
    return SEPARATOR_KEEP;
  
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/

int main(int argc, char *argv[])
{
  input_names_type* name;
  
  if(argc!=4)
    {
      printf("separate_TrigBits SFU_input_data_file SFU_output_data_file TrigBit\n");
      exit(-1);
    }
  
  printf("Program separates according to the given TrigBits \nTrigBits are: \nCsIArray--->64 \nGRIFFIN--->128 \nRF--->4\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  strcpy(name->fname.out_data,argv[2]);
  name->flag.separate=1;

  TrigBit=atoi(argv[3]);
  sort(name); 
}
