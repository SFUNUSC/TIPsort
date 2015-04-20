#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
  fprintf(output,"trigger number %12d fold %12d\n",data->h.trig_num,data->h.frag_fold);
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  if(argc!=4)
    {
      printf("\n ./trig_list midas_input_data_file_name map_file_name list_file_name\n");
      exit(-1);
    }

 /* open the output file*/
  if((output=fopen(argv[3],"w"))==NULL)
    {
      printf("\nI can't open input file %s\n",argv[3]);
      exit(-2);
    }
 
/* do sorting */
  sort_and_assemble(argv[1],argv[2]);
 /* display results */
  fclose(output);
  printf("\n"); 
  printf("Program outputs the ordered list of triggers\n");
}
