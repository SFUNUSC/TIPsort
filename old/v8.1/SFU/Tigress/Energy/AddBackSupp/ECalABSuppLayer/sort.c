#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  double eAddBack=0;
  int suppFlag=0;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  if(cev->tg.h.FH>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if(((cev->tg.h.HHP&(1<<(pos-1)))!=0) && (cev->tg.det[pos].hge.FE>0))
	if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
	  {
	    eAddBack = cev->tg.det[pos].addback.E/cal_par->tg.contr_e;  
	    suppFlag = cev->tg.det[pos].suppress;
	    col = cev->tg.det[pos].addbackC;
	    ring = cev->tg.det[pos].ge[col].ring;
	    layer = (ring-1)/2; 
	    layer += NLAYER*suppFlag;

	    if(eAddBack>=0 && eAddBack<S32K) 
	      {
		if((ring>0) && (ring<NRING)) hist[layer][(int)rint(eAddBack)]++;
	      }
	    else hist[layer][S32K-1000]++;
	  }

  free(cev);
  return SEPARATOR_DISCARD;
}
/*=========================================================================*/
int main(int argc, char *argv[])
{
  FILE *output;
  input_names_type* name;
  
  if(argc!=2)
    {
      printf("Tigress_ECalABSuppLayer master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts ECalABSuppLayer histograms for TIGRESS.\n");
  
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
      printf("TIGRESS calpar read from: %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR!!! TIGRESS calibration parameters not defined\n");
      exit(EXIT_FAILURE);
    }
  
  sort(name);
  
  if((output=fopen("Layer_ECalABSuppSum.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(hist,2*NLAYER*S32K*sizeof(int),1,output);
  fclose(output);
}
