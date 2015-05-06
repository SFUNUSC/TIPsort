#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  double eAddBack;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);

  printf("event has CsI fold %d TIGRESS fold %d\n",cev->csiarray.h.FE,cev->tg.h.FE);
  getc(stdin);

  if(cev->tg.h.FH>0)
    if(cev->csiarray.h.FH>0)
    for(posTig=1;posTig<NPOSTIGR;posTig++)
      if(((cev->tg.h.HHP&(1<<(posTig-1)))!=0) && (cev->tg.det[posTig].hge.FE>0))
        if((cev->tg.h.AHP&(1<<(posTig-1)))!=0)
	  {	
	    eAddBack = cev->tg.det[posTig].addback.E/cal_par->tg.contr_e;
	    col = cev->tg.det[posTig].addbackC;
	    ring=cev->tg.det[posTig].addbackR;
	    //printf("Position %d, Core %d, and Ring %d....\n",posTig,col,ring);
	    
	    if((eAddBack>=0) && (eAddBack<S32K))
	      {
		if((ring>0) && (ring<NRING)) hist[ring][(int)rint(eAddBack)]++;
	      }
	    else hist[ring][S32K-1000]++;
	  }
  free(cev);
  return SEPARATOR_DISCARD;
}
/*=============================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
 
  if(argc!=4)
    {
      printf("Tigress_ECalABRing_CsIEGate master_file_name Elow Ehigh\n");
      exit(-1);
    }

  //CsI energies for gating
  eLow=atof(argv[2]);
  eHigh=atof(argv[3]);
  
  printf("Program sorts ECalABRing_CsIEGate histograms for TIGRESS.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  memset(hist,0,sizeof(hist));

  read_master(argv[1],name);
  
  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calibration read from %s.\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  if(name->flag.CSIARRAY_cal_par==1)
    {
      printf("CsIArray calpar read from: %s\n",name->fname.CSIARRAY_cal_par);
      initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
    }
  else
    {
      printf("ERROR!!! TIGRESS calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  sort(name);
  
  if((output=fopen("Ring_ECalAB_CsIEGate.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  
  fwrite(hist,NRING*S32K*sizeof(int),1,output);

  fclose(output);

  return 0;
}
