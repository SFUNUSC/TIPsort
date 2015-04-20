#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  double t;
  
  if(data->pinarray.h.Tfold>0)
    for(pos=1;pos<NPIN;pos++)
      if((data->pinarray.h.THP&(one<<pos))!=0)
	{
	  t=data->pinarray.pin[pos].cfd&0x00ffffff;
	  t-=(data->pinarray.pin[pos].timestamp*16)&0x00ffffff;
	  if((data->h.setupHP&RF_BIT)!=0)
	    {
	      t-=(int)data->rf.sin.t0;
	      t+=S16K;
	      if(t<0)t=S32K-2000;
	      if(t>S32K) t=S32K-3000;
	    }
	  else
	    t=S32K-4000;
	  
	  hist[pos][(int)rint(t)]++;
	}
  return SEPARATOR_DISCARD;
}
/*===========================================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *cluster;
  input_names_type* name;
  char DataFile[132];
  
  if(argc!=2)
    {
      printf("PINArray_TRawSum master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts TRawSum histograms for the PINArray.\n");

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
  
  if((output=fopen("PINArray_TRawSum.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  for(int pos=0;pos<NPIN;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);
  
  fclose(output);
}
