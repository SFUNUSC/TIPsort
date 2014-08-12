#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  cal_event* cev;
  double t,e;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_PINARRAY(data,&cal_par->pinarray,&cev->pinarray);
  
  if(cev->pinarray.h.FE>0)
    for(pos=1;pos<NPIN;pos++)
      if(((cev->pinarray.h.THP&(one<<pos))!=0) && ((cev->pinarray.h.EHP&(one<<pos))!=0))
	{
	  t=cev->pinarray.pin[pos].T;
	  e=cev->pinarray.pin[pos].E/cal_par->pinarray.contr_e;
	  
	  if((t>cal_par->pinarray.tlow[pos]) && (t<cal_par->pinarray.thigh[pos]))
	    {
	      t/=cal_par->pinarray.contr_t;
	      if((t<0) || (t>S32K)) t=S32K-4000;
	      if((e<0) || (e>S32K)) e=S32K-4000;
	      hist_e[pos][(int)rint(e)]++;
	      hist_t[pos][(int)rint(t)]++;
	    }}

  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE *output_e, *output_t, *cluster;
  input_names_type* name;
  char DataFile[132];
  
  if(argc!=2)
    {
      printf("PINArray_ECalSum_TLim master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts ECalSum_TLim histograms for the PINArray.\n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  read_master(argv[1],name);

  if(name->flag.PINARRAY_cal_par==1)
    {
      printf("PINArray calibration read from %s\n",name->fname.PINARRAY_cal_par);
      initialize_PINARRAY_calibration(&cal_par->pinarray,name->fname.PINARRAY_cal_par);
    }
  else
    {
      printf("ERROR!!! PINArray calibration parameters not defined!\n");
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
  
  if((output_e=fopen("PINArray_ECalSum_TLim.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  if((output_t=fopen("PINArray_TCalSum_TLim.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  for(int pos=0;pos<NPIN;pos++)
    {
      fwrite(hist_e[pos],S32K*sizeof(int),1,output_e);
      fwrite(hist_t[pos],S32K*sizeof(int),1,output_t);
    }
  fclose(output_e);
  fclose(output_t);
 }

