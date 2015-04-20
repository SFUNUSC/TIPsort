#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  double t;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_PINARRAY(data,&cal_par->pinarray,&cev->pinarray);

  if((cev->pinarray.h.FT>0) && (cev->tg.h.FT>0))
    for(pin=1;pin<NPIN;pin++)
      if((cev->pinarray.h.THP&(one<<pin))!=0)
	{
	  t=cev->pinarray.pin[pin].T/cal_par->pinarray.contr_t;
	  if((cev->tg.h.THP&(one<<(pos-1)))!=0)
	    if(cev->tg.det[pos].hge.FT>0)
	      if((cev->tg.det[pos].hge.THP&(one<<col))!=0)
		if(cev->tg.det[pos].ge[col].h.FT>0)
		  if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
		    {
		      if((t>0) && (t<S32K)) hist[pin][(int)rint(t)]++;
		      else hist[pin][S32K-1000]++; 
		    }}
  free(cev);
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
      printf("PINArray_TCalSum_wrtTigress master_file_name TigPos TigCol\n");
      exit(-1);
    }
  
  printf("Program sorts TCalSum histograms for the PINArray without the RF.\n");
  pos=atoi(argv[2]);
  col=atoi(argv[3]);
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);
  
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
  
  if((output=fopen("PINArray_TCalSum.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  for(pin=0;pin<NPIN;pin++)
    fwrite(hist[pin],S32K*sizeof(int),1,output);
  
  fclose(output);
}
