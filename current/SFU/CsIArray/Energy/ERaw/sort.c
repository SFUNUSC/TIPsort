#include "sort.h"

int analyze_data(raw_event *data)
{
  uint64_t one=1;
  int pos,e;

  if(data->csiarray.h.Efold>0)
    for(pos=1;pos<NCSI;pos++)
      if((data->csiarray.h.THP[pos/64]&(one<<pos%64))!=0)
				{
					e=data->csiarray.csi[pos].charge;
					if((e>=0) && (e<S32K)) hist[pos][e]++;
				}
  return SEPARATOR_DISCARD;
}
/*==========================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;

  if(argc!=2)
    {
      printf("CsIArray_ERaw sfu_input_data\n");
      exit(-1);
    }
  
  printf("Program sorts ERaw histogram for the CsIArray.\n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);
  
  if((output=fopen("CsIArray_ERaw.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  for(int pos=0;pos<NCSI;pos++) fwrite(hist[pos],S32K*sizeof(int),1,output);
  fclose(output);
}
