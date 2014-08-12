#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos,col;
  double e;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_GRIFFIN(data,&cal_par->gr,&cev->gr);
  
  if(cev->gr.h.FE>0)
    for(pos=1;pos<NPOSGRIF;pos++)
      if((cev->gr.h.EHP&(1<<(pos-1)))!=0)
  	if(cev->gr.det[pos].hge.FE>0)
  	  for(col=0;col<NCOL;col++)
  	    if((cev->gr.det[pos].hge.EHP&(1<<col))!=0)
  	      if(cev->gr.det[pos].ge[col].h.FE>0)
  		if((cev->gr.det[pos].ge[col].h.EHP&1)!=0)
  		  {
  		    e=cev->gr.det[pos].ge[col].seg[0].E/cal_par->gr.contr_e;

		    if(e>0 && e<S32K) hist[(int)rint(e)]++;
		    else hist[S32K-1000]++;
  		  }
  free(cev);
  return SEPARATOR_DISCARD;
}
/*===============================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  int stop,pos,col;
  char n[132];

  if(argc!=2)
    {
      printf("Griffin_ECalArray master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts ECal histogram of all GRIFFIN cores in array.\n");
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

  if(name->flag.GRIFFIN_cal_par==1)
    {
      printf("Griffin calibration read from %s.\n",name->fname.GRIFFIN_cal_par);
      initialize_GRIFFIN_calibration(&cal_par->gr,name->fname.GRIFFIN_cal_par);
    }
  else
    {
      printf("ERROR!!! Griffin calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  summarize_GRIFFIN_calibration(&cal_par->gr,"in.in");
  sort(name);
  summarize_GRIFFIN_calibration(&cal_par->gr,"out.out");
  
  sprintf(n,"ECalArray.mca");
  if((output=fopen(n,"w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file\n");
      exit(EXIT_FAILURE);
    }
  fwrite(hist,S32K*sizeof(int),1,output);
  fclose(output);
}
