#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int col,sup;
  int bgoe;
  int suppress,ene;
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  if(cev->tg.h.FE>0)
    if((cev->tg.h.EHP&(1<<(pos-1)))!=0)
      if(cev->tg.det[pos].hge.FE>0)
	{
	  ene=(int)rint(cev->tg.det[pos].addback.E/cal_par->tg.contr_e);
	  suppress=0;
	  if(data->tg.h.BGOfold>0) /* BGO in the event */
	    if((data->tg.h.BGOHP&(1<<(pos-1)))!=0) /* BGO at the right position in the event */
	      if(data->tg.det[pos].h.BGOfold>0) /* BGO at the right position in the event */
		for(col=0;col<NCOL;col++) /* loop over colors */
		  if((data->tg.det[pos].h.BGOHP&(1<<col))!=0)  /* BGO at the right position and color in the event */
		    if(data->tg.det[pos].bgo[col].h.Tfold>0)   /* BGO time information at the right position and color in the event */
		      for(sup=0;sup<NSUP;sup++) /* loop over BGO suppressors */
			if((data->tg.det[pos].bgo[col].h.THP&(1<<sup))!=0) 
			  {
			    /* BGO energy */
			    bgoe=data->tg.det[pos].bgo[col].sup[sup].charge;
			   
			    if(bgoe>elow)
			      if(bgoe<ehigh)
				suppress=1;
			      
			  }
	  if(ene>0)
	    if(ene<S4K)
	      hist[suppress][ene]++;
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

  if(argc!=5)
    {
      printf("\n ./Tigress_Suppression_E master_file_name position elow ehigh\n");
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
  elow=atoi(argv[3]);
  ehigh=atoi(argv[4]);
  sort(name);


  sprintf(n,"pos%1d%1d_suppresed.spn",pos/10,pos-(pos/10)*10);

  if((output=fopen(n,"w"))==NULL)
    {
      printf("\nI can't open file %s\n",n);
      exit(EXIT_FAILURE);
    }
  for(k=0;k<2;k++)
    fwrite(hist[k],S4K*sizeof(int),1,output);
  fclose(output);




}
