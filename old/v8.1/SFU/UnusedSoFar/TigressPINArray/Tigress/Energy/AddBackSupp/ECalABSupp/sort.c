#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  cal_event* cev;
  double eAddBack=0,ePreAddBack=0,ePIN=0;
  int suppFlag=0,take=0;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_PINARRAY(data,&cal_par->pinarray,&cev->pinarray);
  
  if((cev->tg.h.FH>0) && (cev->pinarray.h.FH>0))
    for(pos=1;pos<NPOSTIGR;pos++)
      if(((cev->tg.h.HHP&(one<<(pos-1)))!=0) && (cev->tg.det[pos].hge.FE>0))
	if((cev->tg.h.AHP&(one<<(pos-1)))!=0)
	  {
	    /*
	    //Let's output some samples of add back at work...
	    for(col=0;col<NCOL;col++)
	      if(((cev->tg.det[pos].hge.HHP&(one<<col))!=0) && (cev->tg.det[pos].ge[col].h.FH>0))
		{
		  ePreAddBack = cev->tg.det[pos].ge[col].seg[0].E/cal_par->tg.contr_e;
		  if(cev->tg.det[pos].hge.FE > 2) printf("For Position %d and Core %d the energy is %f.\n",pos,col,ePreAddBack);
		}
	    */
	    take=0;
	    eAddBack = cev->tg.det[pos].addback.E/cal_par->tg.contr_e;  
	    suppFlag = cev->tg.det[pos].suppress;
	    col = cev->tg.det[pos].addbackC;
	    col += NCOL*suppFlag;
	    
	    for(pin=1;pin<NPIN;pin++)
	      if(((cev->pinarray.h.HHP&(one<<pin))!=0) && (take==0))
		{
		  ePIN=cev->pinarray.pin[pin].E/cal_par->pinarray.contr_e;
		  if((ePIN>cal_par->pinarray.elow[pin]) && (ePIN<cal_par->pinarray.ehigh[pin]))
		    {
		      take=1;
		      if(eAddBack>=0 && eAddBack<S32K) hist[pos][col][(int)rint(eAddBack)]++;
		      else hist[pos][col][S32K-1000]++;
		      /*
			if(cev->tg.det[pos].hge.FE > 2) printf("The add back energy is ---> %f.\n",eAddBack);
		      */
		    }}}
  
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
      printf("Tigress_PINArrayECalLimABSupp master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts ECalABSupp histograms for TIGRESS gated on PINArrayECal.\n");
  
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
  
  for(pos=1;pos<NPOSTIGR;pos++)
    {
      sprintf(mcaFile,"pos%1d%1d_PINArrayECalLimABSupp.mca",pos/10,pos-(pos/10)*10);
      if((output=fopen(mcaFile,"w"))==NULL)
	{
	  printf("ERROR!!! I cannot open the mca file!\n");
	  exit(EXIT_FAILURE);
	}
      
      for(col=0;col<NCOL*2;col++) fwrite(hist[pos][col],S32K*sizeof(int),1,output);
      fclose(output);
    }}
