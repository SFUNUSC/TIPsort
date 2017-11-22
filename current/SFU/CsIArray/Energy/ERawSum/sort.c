#include "sort.h"

int analyze_data(raw_event *data)
{
  uint64_t one=1;
  int pos,e;

  if(data->csiarray.h.Efold>0)
    for(pos=1;pos<NCSI;pos++)
      if((data->csiarray.h.EHP[pos/64]&(one<<pos%64))!=0)
	{
	  e=data->csiarray.csi[pos].charge;
	  if((e>=0) && (e<S32K)) hist[pos][e]++;
	}
  return SEPARATOR_DISCARD;
}
/*==========================================================*/
int main(int argc, char *argv[])
{
  FILE *output,  *ClusterFile, *DataFile;;
  input_names_type* name;
  char DataFileEntry[256], ClusterFileName[256];

  if(argc!=2)
    {
      printf("CsIArray_ERaw cluster_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts summed ERaw histograms for the CsIArray from a cluster file.\n");

  strcpy(ClusterFileName,argv[1]);
  memset(hist,0,sizeof(hist));
  
  name=(input_names_type*)malloc(sizeof(input_names_type));

  if((ClusterFile=fopen(ClusterFileName,"r")) == NULL)
    {
      printf("ERROR! I cannot find %s\n",ClusterFileName);
      exit(-2);
    }

  while(fscanf(ClusterFile,"%s",DataFileEntry) != EOF)
    {
      if((DataFile=fopen(DataFileEntry,"r")) == NULL)
	{
	  printf("ERROR! I cannot find %s\n",DataFileEntry);
	  exit(-2);
	}

      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFileEntry);
      
      printf("Sorting particle energy data from file %s\n", DataFileEntry);
      sort(name);
    }
  
  if((output=fopen("CsIArray_ERawSum.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  for(int pos=0;pos<NCSI;pos++) fwrite(hist[pos],S32K*sizeof(int),1,output);
  fclose(output);
}
