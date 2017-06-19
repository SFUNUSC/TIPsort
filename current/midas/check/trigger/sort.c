#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{ 
	int trigNum=ptr->trigger_num&0x0fffffff;
 
  if(trigNum>maxTrigger)
  	maxTrigger=trigNum;
  if(trigNum<minTrigger)
  	minTrigger=trigNum;
  
  numFragments++;
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  
  char *ext;
	maxTrigger=0;
	minTrigger=9999999;//some big number
	numFragments=0;

  if(argc!=2)
    {
      printf("project_channel midas_input_data\n");
      printf("Program checks trigger numbers in a midas file or list of midas files.\n");
      printf("Input is list (ext .list) or single file (ext .mid)\n");
      exit(-1);
    }

	printf("Program checks trigger numbers in a midas file or list of midas files.\n");
	
	if(strrchr(argv[1],'.')!=NULL)
		ext=strrchr(argv[1],'.'); /* returns a pointer to the last . to grab extention*/
	else
		{
      printf ("ERROR!!! Imporoper midas file source extension.\n");
      printf("Input is list (ext .list) or single file (ext .mid)\n");
      exit(-2);
    }
    
	/* sort from a list of midas files */
  if(strcmp(ext,".list")==0)
    {
      printf ("Sorting from a list: found extension %s\n", ext);
      sort_but_not_assemble_list(argv[1]);
    }
  /* sort from a single midas file */
  else if(strcmp(ext,".mid")==0)
    {
      printf ("Sorting from a midas file: found extension %s\n", ext);
      sort_but_not_assemble(argv[1]);
    }
  else
    {
      printf ("ERROR!!! Imporoper midas file source extension.\n");
      printf("Input is list (ext .list) or single file (ext .mid)\n");
      exit(-2);
    }
	
  sort_but_not_assemble(argv[1]);

  printf("Finished sort.\n");
  printf("Min Trigger #: %i\n",minTrigger);
  printf("Max Trigger #: %i\n",maxTrigger);
  printf("# of fragments: %i\n",numFragments);
}
