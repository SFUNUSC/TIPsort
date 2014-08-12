#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos,hp,ene;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  if(cev->tg.h.FE>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.EHP&(1<<(pos-1)))!=0)
  	if(cev->tg.det[pos].hge.FE>0)
	  {
	    ene=(int)rint(cev->tg.det[pos].addback.E/cal_par->tg.contr_e);
	    hp=cev->tg.det[pos].addbackHP;
	    if(ene>0)
	      if(ene<S4K)
		hist[pos][hp][ene]++;
	  }
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  int pos,id;
  char n[132];


  if(argc!=2)
    {
      printf("\n ./TIGRESS_RFAddBack master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts calibrated add back energy histogram for TIGRESS cores \n");
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



  sort(name);

  for(pos=1;pos<NPOSTIGR;pos++)
    {
      sprintf(n,"pos%1d%1d_addback.spn",pos/10,pos-(pos/10)*10);
      if((output=fopen(n,"w"))==NULL)
	{
	  printf("\nI can't open file %s\n",n);
	  exit(EXIT_FAILURE);
	}

      for(id=0;id<NADDBACK;id++)
	{
	  fwrite(hist[pos][id],S4K*sizeof(int),1,output);
	}
      fclose(output);
    }

 
       


}

