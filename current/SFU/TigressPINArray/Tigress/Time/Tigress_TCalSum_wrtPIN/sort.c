#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  long int t;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_PINARRAY(data,&cal_par->pinarray,&cev->pinarray);
  
  if((cev->tg.h.FT>0) && (cev->pinarray.h.FT>0))
    for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.THP&(one<<(pos-1)))!=0)
	if(cev->tg.det[pos].hge.FT>0)
	  for(col=0;col<NCOL;col++)
	    if((cev->tg.det[pos].hge.THP&(one<<col))!=0)
	      if(cev->tg.det[pos].ge[col].h.FT>0)
		if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
		  {
		    t=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;
		    if((cev->pinarray.h.THP&(one<<pin))!=0) 
		      {
			if((t>0) && (t<S32K)) hist[pos][col][t]++;
			else hist[pos][col][S32K-1000]++;
		      }}
free(cev);
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
      printf("Tigress_TCalSum_wrtPIN master_file_name pin_number\n");
      exit(-1);
    }
  
  printf("Program sorts TCalSum spectra for Tigress for a given PIN diooe.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));

  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

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
  
  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calibration read from the file: %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);  
    }
  else
    {
      printf("ERROR! TIGRESS calibration parameters not defined\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.PINARRAY_cal_par==1)
    {
      printf("PINARRAY calibration read from the file: %s\n",name->fname.PINARRAY_cal_par);
      initialize_PINARRAY_calibration(&cal_par->pinarray,name->fname.PINARRAY_cal_par);
    }
  else
    {
      printf("ERROR! PINARRAY calibration parameters not defined\n");
      exit(EXIT_FAILURE);
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
