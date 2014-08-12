#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
  int ehp,e;
  for(pos=1;pos<NPIN;pos++)
    if((ehp=data->pinarray.h.EHP)!=0)
      if(((ehp>>pos)&1)!=0)
	{
	  e=data->pinarray.pin[pos].charge;
	  if(e>=0)
	    if(e<S32K)
	      hist[pos][e]++;
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
      printf("\n ./rawSpectra_PINArrayE midas_input_data_file_name map_file_name\n");
      exit(-1);
    }
  /* initialize histograms */
  memset(&hist,0,sizeof(hist));

/* do sorting */
  sort_and_assemble(argv[1],argv[2]);
 /* display results */
  printf("\n"); 

  sprintf(name,"PINArray_raw_energy.mca");
  if((output=fopen(name,"w"))==NULL)
    {
      printf("\nI can't open file %s\n",name);
      exit(EXIT_FAILURE);
    }
  for(pos=0;pos<NPIN;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);

  fclose(output);


}
