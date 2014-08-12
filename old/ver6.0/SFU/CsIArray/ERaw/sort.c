#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,e;
  if(data->csiarray.h.Efold>0)
    for(pos=1;pos<NCSI;pos++)
      if((data->csiarray.h.EHP&(one<<pos))!=0)
	{
	  e=data->csiarray.csi[pos].charge;
	  if(e>=0)
	    if(e<S32K)
	      hist[pos][e]++;
	  }
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;

  if(argc!=2)
    {
      printf("\n ./CSIARRAY_ERaw sfu_input_data_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts raw energy histogram for the CSIARRAY \n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);

  if((output=fopen("CSIArray_raw_energy.mca","w"))==NULL)
    {
      printf("\nI can't open file %s\n","CSIArray_raw_energy.mca");
      exit(EXIT_FAILURE);
    }
  for(int pos=0;pos<NCSI;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);

  fclose(output);
 
}

