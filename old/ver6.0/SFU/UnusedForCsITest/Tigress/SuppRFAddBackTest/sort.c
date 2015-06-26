#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int ene;
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  if(cev->tg.h.FE>0)
    if((cev->tg.h.EHP&(1<<(pos-1)))!=0)
      if(cev->tg.det[pos].hge.FE>0)
	{
	  
	  ene=(int)rint(cev->tg.det[pos].addback.E/cal_par->tg.contr_e);
	  if(ene>0)
	    if(ene<S32K)
	      hist[cev->tg.det[pos].suppress][ene]++;
	}
  free(cev);

  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  char n[132];
  int k;

  if(argc!=3)
    {
      printf("\n ./Tigress_SuppRFAddBackTest master_file_name position\n");
      exit(-1);
    }
  
  printf("Program sorts BGO suppressed Tigress addback spectra \n");

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

  if(name->flag.TIGRESS_cal_par==1)
        {
          printf("\nTIGRESS calibration read from the file:\n %s\n",name->fname.TIGRESS_cal_par);
          initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
	  
        }
      else
        {
          printf("\nTIGRESS calibration parameters not defined\n");
          exit(EXIT_FAILURE);
        }

  pos=atoi(argv[2]);

  summarize_TIGRESS_calibration(&cal_par->tg,"in.in"); 
  sort(name);
  summarize_TIGRESS_calibration(&cal_par->tg,"out.out");

  sprintf(n,"pos%1d%1d_suppresed.mca",pos/10,pos-(pos/10)*10);

  if((output=fopen(n,"w"))==NULL)
    {
      printf("\nI can't open file %s\n",n);
      exit(EXIT_FAILURE);
    }
  for(k=0;k<2;k++)
    fwrite(hist[k],S32K*sizeof(int),1,output);
  fclose(output);




}
