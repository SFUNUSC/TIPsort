#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
  return encode(data,encoded_output,enb);
}
/*================================================================*/
int main(int argc, char *argv[])
{
 

  if(argc!=4)
    {
      printf("\n ./midas2sfu midas_input_data_file_name SFU_output_data_file_name map\n");
      exit(-1);
    }
  if((encoded_output=fopen(argv[2],"w"))==NULL)
    {
      printf("\nI can't open output file %s for writing\n",argv[2]);
      exit(-2);
    }
  memset(enb,0,sizeof(enb));
  enb[0]=BUFFER_TAG;
  enb[1]++;
  enb[1]++;
/* do sorting */
  sort_and_assemble(argv[1],argv[3]);
 /* display results */
  printf("\n"); 
  printf("Program assemble events from midas to the SFU format\n");
}
