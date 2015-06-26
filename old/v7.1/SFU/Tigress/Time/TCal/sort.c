#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos,col;
  double t;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);

  if(cev->tg.h.FT>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.THP&(1<<(pos-1)))!=0)
	if(cev->tg.det[pos].hge.FT>0)
	  for(col=0;col<NCOL;col++)
	    if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
	      if(cev->tg.det[pos].ge[col].h.FT>0)
		if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
		  {
		    t=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;
		    if(t>0 && t<S4K)
		      hist[pos][col][(int)rint(t)]++;
		    
		    else hist[pos][col][S4K-10]++;
		  }
  free(cev);
  return SEPARATOR_DISCARD;
}
/*==========================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  int stop,pos,col;
  char n[132];
  
  if(argc!=2)
    {
      printf("TIGRESS_TCal master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts calibrated time histogram for TIGRESS cores.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);
  
  if(name->flag.inp_data!=1)
    {
      printf("ERROR! Input data file not defined.\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calibration read from: %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR! TIGRESS calibration parameters not defined.\n");
      exit(EXIT_FAILURE);
    }
  
  sort(name);
  
  for(pos=1;pos<NPOSTIGR;pos++)
    {
      stop=0;
      for(col=0;col<NCOL;col++)
	stop+=cal_par->tg.ceflag[pos][col];
      if(stop>0)
	{
	  sprintf(n,"pos%1d%1d_CalCoreTime.spn",pos/10,pos-(pos/10)*10);
	  if((output=fopen(n,"w"))==NULL)
	    {
	      printf("ERROR! I can't open: %s\n",n);
	      exit(EXIT_FAILURE);
	    }
	  
	  for(col=0;col<NCOL;col++)
	    fwrite(hist[pos][col],S4K*sizeof(int),1,output);
	  
	  fclose(output);
	  
	}}
}