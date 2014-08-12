#include "sort.h"

int analyze_data(raw_event *data)
{
  if(((data->h.setupHP&S3SEC_BIT)!=0)||((data->h.setupHP&S3RING_BIT)!=0))
    {
      display_S3(&data->s3);
      getc(stdin);
    }
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/

int main(int argc, char *argv[])
{

  input_names_type* name;

  if(argc!=2)
    {
      printf("\n ./display_S3 sfu_input_data_file_name\n");
      exit(-1);
    }
  
 
  printf("Program displays S3 events \n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name); 
   
}
