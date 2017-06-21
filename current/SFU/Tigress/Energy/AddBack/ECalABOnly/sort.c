#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  double eAddBack=0,eCal=0;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  if(cev->tg.h.FH>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if(((cev->tg.h.HHP&(1<<(pos-1)))!=0) && (cev->tg.det[pos].hge.FE>0))
				if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
					{

						//get addback energy
						eAddBack = cev->tg.det[pos].addback.E/cal_par->tg.contr_e;  
						col = cev->tg.det[pos].addbackC;
						
						//get corresponding non-addback energy (in same position)
						eCal=cev->tg.det[pos].ge[col].seg[0].E/cal_par->tg.contr_e;
						
						//check whether they are equal
						//if so, addback was not used, discard event
						if(eAddBack==eCal)
							break;
					
						//record addback energy
						if(eAddBack>=0 && eAddBack<S32K) 
							hist[pos][col][(int)(eAddBack)]++;
						else 
							hist[pos][col][S32K-1000]++;
					
						//if(cev->tg.det[pos].hge.FE > 2) printf("The add back energy is ---> %f.\n",eAddBack);
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
      printf("Tigress_ECalABOnly master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts ECalABOnly histograms for TIGRESS.\n");
  
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
  
  for(pos=1;pos<NPOSTIGR;pos++)
    {
      sprintf(mcaFile,"pos%1d%1d_ECalABOnly.mca",pos/10,pos-(pos/10)*10);
      if((output=fopen(mcaFile,"w"))==NULL)
	{
	  printf("ERROR!!! I cannot open the mca file!\n");
	  exit(EXIT_FAILURE);
	}
      
      for(col=0;col<NCOL;col++) fwrite(hist[pos][col],S32K*sizeof(int),1,output);
      fclose(output);
    }}
