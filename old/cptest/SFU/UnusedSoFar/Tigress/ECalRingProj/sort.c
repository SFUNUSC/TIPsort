#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos,col,ring;
  double e;
  

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  if(cev->tg.h.FE>0)
    {
      //     printf("*");
   for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.EHP&(1<<(pos-1)))!=0)
  	if(cev->tg.det[pos].hge.FE>0)
  	  for(col=0;col<NCOL;col++)
  	    if((cev->tg.det[pos].hge.EHP&(1<<col))!=0)
  	      if(cev->tg.det[pos].ge[col].h.FE>0)
  		if((cev->tg.det[pos].ge[col].h.EHP&1)!=0)
  		  {

  		    e=cev->tg.det[pos].ge[col].seg[0].E/cal_par->tg.contr_e;
		    ring=cev->tg.det[pos].ge[col].ring;
		    if(e>0)
		      if(e<S4K)
			if(ring>0)
			  if(ring<NRING)
			    hist[ring][(int)rint(e)]++;
  		  }
    }
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output,*pcluster;
  input_names_type* name;
  char ni[132],no[132];
 

  if(argc!=2)
    {
      printf("\n ./TIGRESS_ECalRingProj master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts calibrated energy histogram for TIGRESS rings \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  read_master(argv[1],name);


  if(name->flag.pcluster_file==1)
        {
          printf("\nSorting projections files based on the cluster file:\n %s\n",name->fname.pcluster_file);
	  if((pcluster=fopen(name->fname.pcluster_file,"r"))==NULL)
	    {
	      printf("\nI can't open input file %s\n",name->fname.pcluster_file);
	      exit(-2);
	    }
        }
      else
        {
          printf("\nProjection cluster file not defined\n");
          exit(-1);
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


  while(fscanf(pcluster,"%s %s",ni,no)!=EOF)
    {
      summarize_TIGRESS_calibration(&cal_par->tg,"test.dat");
      strcpy(name->fname.inp_data,ni);
      strcpy(name->fname.out_data,no);
      printf("Sorting from file %s\n",name->fname.inp_data);
      printf("          to file %s\n",name->fname.out_data);
      memset(hist,0,NRING*S4K*sizeof(int));
      sort(name);

      if((output=fopen(name->fname.out_data,"w"))==NULL)
	{
	  printf("\nI can't open output file %s\n",name->fname.out_data);
	  exit(EXIT_FAILURE);
	}
      
      fwrite(hist,NRING*S4K*sizeof(int),1,output);
	 
      fclose(output);
    }
  fclose(pcluster);

}

