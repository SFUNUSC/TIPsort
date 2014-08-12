#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
  int e;
  unsigned long long int ehp;
  for(pos=1;pos<NTIFF;pos++)
    if((ehp=data->tiffin.h.EHP)!=0)
      if(((ehp>>pos)&1)!=0)
	{
	  e=data->tiffin.chan[pos].charge/2;
	  if((e>=0) && (e<S32K)) hist[pos][e]++;
	  else hist[0][1000]++;
	}

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  FILE* output;
  char name[132];

  if(argc!=3)
    {
      printf("rawSpectra_TIFFINE midas_input_data map_file_name\n");
      exit(-1);
    }
  /* initialize histograms */
  memset(&hist,0,sizeof(hist));

  /* do sorting */
  sort_and_assemble(argv[1],argv[2]);
  /* display results */
  
  sprintf(name,"Tiffin_ERaw.mca");
  if((output=fopen(name,"w"))==NULL)
    {
      printf("ERROR! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  for(pos=0;pos<NTIFF;pos++) fwrite(hist[pos],S32K*sizeof(int),1,output);
  fclose(output);
}
