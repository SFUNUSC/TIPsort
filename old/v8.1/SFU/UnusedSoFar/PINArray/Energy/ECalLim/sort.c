#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  double ePIN;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_PINARRAY(data,&cal_par->pinarray,&cev->pinarray);
  
  if(cev->pinarray.h.FE>0)
    for(pin=1;pin<NPIN;pin++)
      if((cev->pinarray.h.EHP&(one<<pin))!=0)
	{
	  ePIN=cev->pinarray.pin[pin].E/cal_par->pinarray.contr_e;
	  if((ePIN>cal_par->pinarray.elow[pin]) && (ePIN<cal_par->pinarray.ehigh[pin]))
	    {
	      if((ePIN>=0) && (ePIN<S32K)) hist[pin][(int)rint(ePIN)]++;
	      else hist[pin][S32K-1000]++;
	    }}
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*================================================================================*/
int main(int argc, char *argv[])
{
  FILE *output;
  input_names_type* name;
  
  if(argc!=2)
    {
      printf("PINArray_ECalLim master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts ECalLim for the PINArray.\n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));

  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  memset(hist,0,sizeof(hist)); 
  read_master(argv[1],name);
  
  if(name->flag.inp_data!=1)
    {
      printf("\nInput data file not defined\n");
      exit(EXIT_FAILURE);
    }
  
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
  
  sort(name);
    
  if((output=fopen("PINArray_ECalLim.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  for(pin=0;pin<NPIN;pin++) fwrite(hist[pin],S32K*sizeof(int),1,output);
  fclose(output);
}

