#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  cal_event* cev;
  double eAddBack=0;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_GRIFFIN(data,&cal_par->gr,&cev->gr);

  if(cev->gr.h.FE>0)
    for(pos=1;pos<NPOSGRIF;pos++)
      if(((cev->gr.h.EHP&(one<<(pos-1)))!=0) && (cev->gr.det[pos].hge.FE>0))
	if((cev->gr.h.AHP&(one<<(pos-1)))!=0)
	  {
	    eAddBack = cev->gr.det[pos].addback.E/cal_par->gr.contr_e;  
	    col = cev->gr.det[pos].addbackC;
	    if(eAddBack>=0 && eAddBack<S32K) hist[pos][col][(int)rint(eAddBack)]++;
	    else hist[pos][col][S32K-1000]++; 
	  }
  free(cev);
  return SEPARATOR_DISCARD;
}
/*=========================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  char mcaFile[132];
  
  if(argc!=2)
    {
      printf("Griffin_ECalAB master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts ECalAB histograms for GRIFFIN.\n");
  
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
  
  if(name->flag.GRIFFIN_cal_par==1)
    {
      printf("GRIFFIN calpar read from: %s\n",name->fname.GRIFFIN_cal_par);
      initialize_GRIFFIN_calibration(&cal_par->gr,name->fname.GRIFFIN_cal_par);
    }
  else
    {
      printf("ERROR!!! GRIFFIN calibration parameters not defined\n");
      exit(EXIT_FAILURE);
    }
  
  sort(name);
  
  for(pos=1;pos<NPOSGRIF;pos++)
    {
      sprintf(mcaFile,"pos%1d%1d_ECalAB.mca",pos/10,pos-(pos/10)*10);
      if((output=fopen(mcaFile,"w"))==NULL)
	{
	  printf("ERROR!!! I cannot open the mca file!\n");
	  exit(EXIT_FAILURE);
	}
      
      for(col=0;col<NCOL;col++) fwrite(hist[pos][col],S32K*sizeof(int),1,output);
      fclose(output);
    }}
