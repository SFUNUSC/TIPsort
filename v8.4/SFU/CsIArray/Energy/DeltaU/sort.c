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
  FILE * output;
  input_names_type* name;

  if(argc!=2)
    {
      printf("CsIArray_DeltaU master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts ECal histograms for the CsIArray.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);

  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined!\n");
      exit(EXIT_FAILURE);
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
  
  sort(name);
  
  if((output=fopen("CsIArray_deltaU.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!");
      exit(EXIT_FAILURE);
    }
 
  fwrite(hist,S32K*sizeof(int),1,output);
  
  fclose(output);
}
