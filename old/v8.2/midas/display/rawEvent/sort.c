#include "sort.h"

int analyze_assembled_event(raw_event* data)
{

  display_raw_event(data);
  getc(stdin);
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  if(argc!=3)
    {
      printf("display_rawEvent midas_input_data map\n");
      exit(-1);
    }
  
  sort_and_assemble(argv[1],argv[2]);

  printf("Program displays raw TIGRESS and auxillary detector events from midas file.\n");

 
 
 
}
