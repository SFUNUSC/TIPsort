#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int take=0;
  double ePIN=0,eAddBack=0;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_PINARRAY(data,&cal_par->pinarray,&cev->pinarray);
  
  if((cev->tg.h.FH>0) && (cev->pinarray.h.FH>0))
    for(pin=1;pin<NPIN;pin++)
      if((cev->pinarray.h.HHP&(one<<pin))!=0)
	{
	  ePIN=cev->pinarray.pin[pin].E/cal_par->pinarray.contr_e;
	  take=0;
	  
	  for(pos=1;pos<NPOSTIGR;pos++)
	    if(((cev->tg.h.HHP&(one<<(pos-1)))!=0) && (cev->tg.det[pos].hge.FE>0) && (take==0))
	      if((cev->tg.h.AHP&(one<<(pos-1)))!=0)
		{
		  eAddBack = cev->tg.det[pos].addback.E/cal_par->tg.contr_e;  
		  col = cev->tg.det[pos].addbackC;
		  
		  if((eAddBack>cal_par->tg.celow[pos][col]) && (eAddBack<cal_par->tg.cehigh[pos][col]))
		    {
		      take=1;
		      if((ePIN>0) && (ePIN<S32K)) hist[pin][(int)rint(ePIN)]++;
		      else hist[pin][S32K-1000]++;
		    }}}
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*===============================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  FILE *output;

  if(argc!=2)
    {
      printf("PINArray_TigECalLim master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts PINArray_ECal spectra gated on TigECalLim.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));

  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);
  
  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined.\n");
      exit(EXIT_FAILURE);
    }

  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calibration read from %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR!!! TIGRESS calibration parameters not defined!\n");
      exit(-1);
    }
  
  if(name->flag.PINARRAY_cal_par==1)
    {
      printf("PINARRAY calibration read from %s\n",name->fname.PINARRAY_cal_par);
      initialize_PINARRAY_calibration(&cal_par->pinarray,name->fname.PINARRAY_cal_par);
    }
  else
    {
      printf("ERROR!!! PINARRAY calibration parameters not defined!\n");
      exit(-1);
    }

  sort(name);
  
  if((output=fopen("PINArray_TigECalLim.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(hist,NPIN*S32K*sizeof(int),1,output);
  fclose(output);
  }
