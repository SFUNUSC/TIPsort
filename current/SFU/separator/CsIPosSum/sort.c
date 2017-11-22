#include "sort.h"

int analyze_data(raw_event *data)
{

  uint64_t one=1;
  int pos;

	if((data->h.setupHP&CsIArray_BIT)==0) 
    return SEPARATOR_DISCARD;

	for(pos=1;pos<NCSI;pos++)
    if((data->csiarray.h.THP[pos/64]&(one<<pos%64))!=0)
    	{
    		if(pos<posLow || pos>posHigh)
					return SEPARATOR_DISCARD;
    	}
	encode(data,output,enb);
	return SEPARATOR_DISCARD;
}
/*====================================================================================*/

int main(int argc, char *argv[])
{
  input_names_type* name;
  FILE *cluster;
  char DataFile[132];
  
  if(argc!=5)
    {
      printf("separate_CsIPosSum cluster_file SFU_output_data posLow posHigh\n");
      printf("Program separates data with Csi hits only in the position range specified.\n");
      exit(-1);
    }
   
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.cluster_file,argv[1]);
  strcpy(name->fname.out_data,argv[2]);
  //name->flag.separate=1;

  posLow=atoi(argv[3]);
  posHigh=atoi(argv[4]);
  
  if((output=fopen(name->fname.out_data,"w"))==NULL)
    {
      printf("\nI can't open output file %s for writing\n",name->fname.out_data);
      exit(-2);
    }

  memset(enb,0,sizeof(enb));
  enb[0]=BUFFER_TAG;
  enb[1]++;
  enb[1]++;
  
  if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
    {
      printf("ERROR!!! I cannot open the cluster file!\n");
      exit(-2);
    }
  
  while(fscanf(cluster,"%s",DataFile)!=EOF)
    {
      printf("Reading data from file %s.\n",DataFile);
      strcpy(name->fname.inp_data,DataFile);
      sort(name);
      //save the last buffer which will be dropped otherwise
			//if enb[1]==2 then the buffer contains no data, only the header
				if(enb[1]>2)
					fwrite(enb,sizeof(int),BUFFSIZE,output);
    }
    
}
