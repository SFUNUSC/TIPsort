#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  double u;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);

  if(cev->csiarray.h.FE==2)
    if(cev->csiarray.h.FT==2)
      {
	u=cev->csiarray.U;
	//printf("CsIArray ene = %f\n",u);
	//getc(stdin);
	if(u>=0 && u<S32K)
	  hist[(int)rint(u)]++;
	  
      }
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *cluster;
  input_names_type* name;
  char DataFile[132];


  if(argc!=2)
    {
      printf("CsIArray_ECal master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts ECal histograms for the CsIArray.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);

if(name->flag.cluster_file==1)
    {
      printf("Sorting calibrated energy histograms for TIGRESS clovers and cores based upon the cluster file: %s\n",name->fname.cluster_file);
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

  if(name->flag.CSIARRAY_cal_par==1)
    {
      printf("CsIArray calpar read from: %s\n",name->fname.CSIARRAY_cal_par);
      initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
    }
  else
    {
      printf("ERROR! CsIArray calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }

while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting CsI Delta U data from file %s\n", name);
      sort(name);
    }  
  
  if((output=fopen("CsIArray_deltaUSum.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!");
      exit(EXIT_FAILURE);
    }
 
  fwrite(hist,S32K*sizeof(int),1,output);
  
  fclose(output);
}
