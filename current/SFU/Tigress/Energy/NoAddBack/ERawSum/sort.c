#include "sort.h"

int analyze_data(raw_event *data)
{
  int pos,col,e;
  if(data->tg.h.Gefold>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((data->tg.h.GeHP&(1<<(pos-1)))!=0)
	if(data->tg.det[pos].h.Gefold>0)
	  for(col=0;col<NCOL;col++)
	    if((data->tg.det[pos].h.GeHP&(1<<col))!=0)
	      if(data->tg.det[pos].ge[col].h.Efold>0)
		if((data->tg.det[pos].ge[col].h.EHP&1)!=0)
		  {
		    e=data->tg.det[pos].ge[col].seg[0].charge;
		    if(e>=0 && e<S32K)
		      hist[pos][col][e]++;
		    else hist[pos][col][S32K-10]++;
		  }
  return SEPARATOR_DISCARD;
}
/*=====================================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *ClusterFile, *DataFile;
  input_names_type *name;
  char n[132], DataFileEntry[256], ClusterFileName[256];
  int pos,col;

  if(argc!=2)
    {
      printf("Tigress_ERaw cluster_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts summed ERaw histograms for Tigress crystal cores from a cluster file.\n");
  
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
      
      printf("Sorting gamma-ray energy data from file %s\n", DataFileEntry);
      sort(name);
    }
  
  for(pos=1;pos<NPOSTIGR;pos++)
    {
      sprintf(n,"pos%1d%1d_ERawSum.mca",pos/10,pos-(pos/10)*10);
      if((output=fopen(n,"w"))==NULL)
	{
	  printf("\nI can't open file %s\n",n);
	  exit(EXIT_FAILURE);
	}
      
      for(col=0;col<NCOL;col++)
	fwrite(hist[pos][col],S32K*sizeof(int),1,output);
      
      fclose(output);
    }}
