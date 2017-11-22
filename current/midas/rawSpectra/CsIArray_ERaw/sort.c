#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
	uint64_t one=1;
	//int ehp;
  int e;
  for(pos=1;pos<NCSI;pos++)
    //if((ehp=data->csiarray.h.EHP)!=0)
      //if(((ehp>>pos)&1)!=0)
      if((data->csiarray.h.EHP[pos/64]&(one<<pos%64))!=0)
				{
					e=data->csiarray.csi[pos].charge;
					if((e>=0) && (e<S32K)) hist[pos][e]++;
					else hist[pos][S32K-100]++;
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
      printf("\n ./rawSpectra_CsIArrayE midas_input_file map_file_name\n");
      exit(-1);
    }
  /* initialize histograms */
  memset(&hist,0,sizeof(hist));

/* do sorting */
  sort_and_assemble(argv[1],argv[2]);
 /* display results */
  printf("\n"); 

  sprintf(name,"CsIArray_ERaw.mca");
  if((output=fopen(name,"w"))==NULL)
    {
      printf("\nI can't open file %s\n",name);
      exit(EXIT_FAILURE);
    }
  for(pos=0;pos<NCSI;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);

  fclose(output);


}
