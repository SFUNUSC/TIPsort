#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  double t;
  
  if((data->pinarray.h.Tfold>0) && (data->tg.h.Gefold>0))
    for(pin=1;pin<NPIN;pin++)
      if((data->pinarray.h.THP&(one<<pin))!=0)
	{
	  t=data->pinarray.pin[pin].cfd&0x00ffffff;
	  t-=(data->pinarray.pin[pin].timestamp*16)&0x00ffffff;
	  t+=S16K;
	  t/=4;
	  
	  if((data->tg.h.GeHP&(one<<(pos-1)))!=0)
	    if(data->tg.det[pos].h.Gefold>0)
	      if((data->tg.det[pos].h.GeHP&(one<<col))!=0)
		if(data->tg.det[pos].ge[col].h.Tfold>0)
		  if((data->tg.det[pos].ge[col].h.THP&1)!=0)
		    {
		      if((t>0) && (t<S32K)) hist[pin][(int)rint(t)]++;
		      else hist[pin][S32K-1000]++; 
		    }}
  return SEPARATOR_DISCARD;
}
/*===========================================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *cluster;
  input_names_type* name;
  char DataFile[132];
  
  if(argc!=4)
    {
      printf("PINArray_TRawSum_wrtTigress master_file_name TigPos TigCol\n");
      exit(-1);
    }
  
  printf("Program sorts TRawSum histograms for the PINArray without the RF.\n");
  pos=atoi(argv[2]);
  col=atoi(argv[3]);

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
  for(pin=0;pin<NPIN;pin++)
    fwrite(hist[pin],S32K*sizeof(int),1,output);
  
  fclose(output);
}
