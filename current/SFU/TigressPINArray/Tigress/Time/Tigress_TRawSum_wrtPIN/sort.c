#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  long int t;
  
  if((data->tg.h.Gefold>0) && (data->pinarray.h.Tfold>0))
    for(pos=1;pos<NPOSTIGR;pos++)
      if((data->tg.h.GeHP&(one<<(pos-1)))!=0)
	if(data->tg.det[pos].h.Gefold>0)
	  for(col=0;col<NCOL;col++)
	    if((data->tg.det[pos].h.GeHP&(one<<col))!=0)
	      if(data->tg.det[pos].ge[col].h.Tfold>0)
		if((data->tg.det[pos].ge[col].h.THP&1)!=0)
		  {
		    t=data->tg.det[pos].ge[col].seg[0].cfd&0x00ffffff;
		    t-=(data->tg.det[pos].ge[col].seg[0].timestamp*16)&0x00ffffff;
		    t+=S16K;
		    t/=4;

		    if((data->pinarray.h.THP&(one<<pin))!=0) 
		      {
			if((t>0) && (t<S32K)) hist[pos][col][t]++;
			else hist[pos][col][S32K-1000]++;
		      }}
  return SEPARATOR_DISCARD;
}
/*==========================================================================*/
int main(int argc, char *argv[])
{
  FILE *cluster, *output;
  input_names_type* name;
  char DataFile[256], n[132];
  
  if(argc!=3)
    {
      printf("Tigress_TRawSum_wrtPIN master_file_name pin_number\n");
      exit(-1);
    }
  
  printf("Program sorts TRawSum spectra for Tigress for a given PIN diooe.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);
  pin=atoi(argv[2]);

  printf("The PIN all TIGRESS clovers are aligned to is %d.\n",pin);
  
  if(name->flag.cluster_file==1)
    {
      printf("Sorting data from %s.\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("ERROR!!! I can't open input file %s!\n",name->fname.cluster_file);
	  exit(-2);
	}}
  else
    {
      printf("ERROR!!! Cluster file not defined!\n");
      exit(-1);
    }
  
  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting data from file %s.\n",DataFile);
      sort(name);
    }

  for(pos=1;pos<NPOSTIGR;pos++)
    {
      sprintf(n,"pos%1d%1d_TRawSum.mca",pos/10,pos-(pos/10)*10);
      if((output=fopen(n,"w"))==NULL)
	{
	  printf("ERROR!!! I cannot open the mca file!\n");
	  exit(EXIT_FAILURE);
	}
      
      for(col=0;col<NCOL;col++)
	fwrite(hist[pos][col],S32K*sizeof(int),1,output);
      
      fclose(output);
    }}
