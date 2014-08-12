#include "sort.h"
int analyze_assembled_event(raw_event* data)
{
  return encode(data,encoded_output,enb);
}
/*================================================================*/
int main(int argc, char *argv[])
{
  if(argc!=4)
    {
      printf("midas2sfu midas_input_file SFU_output_file map\n");
      exit(-1);
    }
  if((encoded_output=fopen(argv[2],"w"))==NULL)
    {
      printf("ERROR!!! I can't open %s for writing!\n",argv[2]);
      exit(-2);
    }

  memset(enb,0,sizeof(enb));
  enb[0]=BUFFER_TAG;
  enb[1]++;
  enb[1]++;

  sort_and_assemble(argv[1],argv[3]);
  printf("\n SUCCESS: Program assembled events from %s to %s\n",argv[1],argv[2]);
}
