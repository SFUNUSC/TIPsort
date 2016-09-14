#include "sort.h"
int analyze_assembled_event(raw_event* data)
{
  return encode(data,encoded_output,enb);
}
/*================================================================*/
int main(int argc, char *argv[])
{
  char *ext;

  if(argc!=4)
    {
      printf("midas2sfu input SFU_output_file map\n");
      printf("Input is list (ext .list) or single file (ext .mid)\n");
      exit(-1);
    }

  if((encoded_output=fopen(argv[2],"w"))==NULL)
    {
      printf("ERROR!!! I can't open %s for writing!\n",argv[2]);
      exit(-2);
    }

  ext=strrchr(argv[1],'.'); /* returns a pointer to the last . to grab extention*/

  /* proof that it works */
  /* if(ext != NULL) */
  /*   printf ("found a '.' at %s\n", ext); */
  /* getc(stdin); */

  memset(enb,0,sizeof(enb));
  enb[0]=BUFFER_TAG;
  enb[1]++;
  enb[1]++;

  /* sort from a list of midas files */
  if(strcmp(ext,".list")==0)
    {
      printf ("Sorting from a list: found extension %s\n", ext);
      sort_and_assemble_list(argv[1],argv[3]);
      printf("SUCCESS: assembled events from list %s into sfu file %s\n",argv[1],argv[2]);
      return 0;
    }
  /* sort from a single midas file */
  else if(strcmp(ext,".mid")==0)
    {
      printf ("Sorting from a midas file: found extension %s\n", ext);
      sort_and_assemble(argv[1],argv[3]);
      printf("SUCCESS: assembled events from midas file %s into sfu file %s\n",argv[1],argv[2]);
      return 0;
    }
  else
    {
      printf ("ERROR!!! Imporoper midas file source extension.\n");
      printf("Input is list (ext .list) or single file (ext .mid)\n");
      exit(-2);
    }
}
