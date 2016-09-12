#include "sort.h"

int analyze_data(raw_event *data)
{
  double tBGO;
  int take;
  
  if(data->tg.h.BGOfold>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      {
	take=0;
	if(((data->tg.h.BGOHP&(1<<(pos-1)))!=0) && (data->tg.det[pos].h.BGOfold>0))
	  for(col=0;col<NCOL;col++)
	    if(((data->tg.det[pos].h.BGOHP&(1<<col))!=0) && (data->tg.det[pos].bgo[col].h.Tfold>0))
	      for(sup=0;sup<NSUP;sup++)
		if(((data->tg.det[pos].bgo[col].h.THP&(1<<sup))!=0) && (take==0))
		  {
		    tBGO=data->tg.det[pos].bgo[col].sup[sup].cfd&0x00ffffff;
		    tBGO-=(data->tg.det[pos].bgo[col].sup[sup].timestamp*16)&0x00ffffff;
		    if((data->h.setupHP&RF_BIT)!=0)
		      {
			tBGO-=(int)data->rf.sin.t0;
			tBGO+=S16K;
			if((tBGO>=0) && (tBGO<S32K))
			  {
			    hist[pos][col][sup][(int)rint(tBGO)]++;
			    take=1;
			  }
			else 
			  hist[pos][col][sup][S32K-5000]++;
		      }
		  }
      }
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *cluster;
  input_names_type* name;
  char mcaFile[132],DataFile[256];
  
  if(argc!=2)
    {
      printf("Tigress_BGOTRaw master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts summed BGOTRaw histograms for Tigress suppressors.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
    
  //strcpy(name->fname.inp_data,argv[1]);
  memset(hist,0,sizeof(hist));

  //Here is where we read the master file
  read_master(argv[1],name);

  if(name->flag.cluster_file==1)
    {
      printf("Sorting raw time histograms for BGO based upon the cluster file: %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("ERROR! I can't open input file %s\n",name->fname.cluster_file);
	  exit(-2);
	}
    }
  else
    {
      printf("ERROR! Cluster file not defined\n");
      exit(-1);
    }
    
  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting timing data from file %s\n",DataFile);
      sort(name);
    }
  //end reading master file

  for(pos=1;pos<NPOSTIGR;pos++)
    for(col=0;col<NCOL;col++)
      {
	sprintf(mcaFile,"pos%1d%1d_col%d_BGOTRawSum.mca",pos/10,pos-(pos/10)*10,col);
	if((output=fopen(mcaFile,"w"))==NULL)
	  {
	    printf("ERROR!!! I cannot open the mca file!\n");
	    exit(EXIT_FAILURE);
	  }
	for(sup=0;sup<NSUP;sup++) fwrite(hist[pos][col][sup],S32K*sizeof(int),1,output);
	fclose(output);
      }
}
