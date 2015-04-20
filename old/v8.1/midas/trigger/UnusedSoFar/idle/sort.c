#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  if(argc!=3)
    {
      printf("\n ./trig_list midas_input_data_file_name map_file_name\n");
      exit(-1);
    }

/* do sorting */
  sort_and_assemble(argv[1],argv[2]);
 /* display results */
  printf("\n"); 
  printf("Program assembles and does nothing\n");
}
