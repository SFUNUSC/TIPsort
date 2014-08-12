#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int e;
  
  if(data->pinarray.h.Efold>0)
    for(pos=1;pos<NPIN;pos++)
      if((data->pinarray.h.EHP&(one<<pos))!=0)
	{
	  e=(int)rint(data->pinarray.pin[pos].charge);
	  if((e>=0) && (e<S32K)) hist[pos][e]++;
	  else hist[pos][S32K-1000]++;
	}
  return SEPARATOR_DISCARD;
}
/*==========================================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *cluster;
  input_names_type* name;
  char DataFile[132];

  if(argc!=2)
    {
      printf("PINArray_ERawSum master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts ERawSum energy histogram for the PINArray.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);

  if(name->flag.cluster_file==1)
    {
      printf("Sorting data from %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("ERROR!!! I cannot open the cluster file!\n");
	  exit(-2);
	}}
  else
    {
      printf("ERROR!!! Cluster file not defined!\n");
      exit(-1);
    }
  
  while(fscanf(cluster,"%s",DataFile)!=EOF)
    {
      strcpy(name->fname.inp_data,DataFile);
      sort(name);
    }
  
  if((output=fopen("PINArray_ERaw.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  for(pos=0;pos<NPIN;pos++) fwrite(hist[pos],S32K*sizeof(int),1,output);
  fclose(output);
}
