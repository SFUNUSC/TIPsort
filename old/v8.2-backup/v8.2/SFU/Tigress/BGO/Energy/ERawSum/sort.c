#include "sort.h"

int analyze_data(raw_event *data)
{
  int eBGO,take;
  
  if(data->tg.h.BGOfold>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      {
	take=0;
	if(((data->tg.h.BGOHP&(1<<(pos-1)))!=0) && (data->tg.det[pos].h.BGOfold>0))
	  for(col=0;col<NCOL;col++)
	    if(((data->tg.det[pos].h.BGOHP&(1<<col))!=0) && (data->tg.det[pos].bgo[col].h.Efold>0))
	      for(sup=0;sup<NSUP;sup++)
		if(((data->tg.det[pos].bgo[col].h.EHP&(1<<sup))!=0) && (take==0))
		  {
		    eBGO=(int)rint(data->tg.det[pos].bgo[col].sup[sup].charge);
		    if((eBGO>0) && (eBGO<S32K)) 
		      {
			hist[pos][col][sup][eBGO]++;
			take=1;
		      }}
      }
  return SEPARATOR_DISCARD;
}
/*================================================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *cluster;
  input_names_type* name;
  char mcaFile[132], DataFile[132];
  
  if(argc!=2)
    {
      printf("Tigress_BGOERaw master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts BGOERawSum histograms for Tigress suppressors.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);

  if(name->flag.cluster_file==1)
    {
      printf("Sorting ECalABSum histograms for TIGRESS clovers and cores based upon the cluster file: %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
        {
          printf("ERROR!!! I can't open input file %s\n",name->fname.cluster_file);
          exit(-2);
        }}
  else
    {
      printf("ERROR!!! Cluster file not defined\n");
      exit(-1);
    }

  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting data from file %s\n", name);
      sort(name);
    }
  
  for(pos=1;pos<NPOSTIGR;pos++)
    for(col=0;col<NCOL;col++)
      {
	sprintf(mcaFile,"pos%1d%1d_col%d_BGOERawSum.mca",pos/10,pos-(pos/10)*10,col);
	if((output=fopen(mcaFile,"w"))==NULL)
	  {
	    printf("ERROR!!! I cannot open the mca file!\n");
	    exit(EXIT_FAILURE);
	  }
	for(sup=0;sup<NSUP;sup++) fwrite(hist[pos][col][sup],S32K*sizeof(int),1,output);
	fclose(output);
      }}

