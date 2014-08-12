#include "sort.h"

int analyze_data(raw_event *data)
{


  
  if(data->csiarray.h.TSfold==fold)
     return SEPARATOR_KEEP;



  return SEPARATOR_DISCARD;
}
/*====================================================================================*/

int main(int argc, char *argv[])
{

  input_names_type* name;


  if(argc!=4)
    {
      printf("\n ./separate_CSI_Fold sfu_input_file_name sfu_output_file_name fold\n");
      exit(-1);
    }

  printf("Program separates events based on CsI fit timing in fold 2\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
 
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  strcpy(name->fname.out_data,argv[2]);

 

  fold=atof(argv[3]);



  name->flag.separate=1;

  sort(name); 
   
}
