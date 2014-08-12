#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos,col,ring;
  double e,t;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  if(cev->tg.h.FH>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.HHP&(1<<(pos-1)))!=0)
  	if(cev->tg.det[pos].hge.FH>0)
  	  for(col=0;col<NCOL;col++)
  	    if((cev->tg.det[pos].hge.HHP&(1<<col))!=0)
  	      if(cev->tg.det[pos].ge[col].h.FH>0)
  		if((cev->tg.det[pos].ge[col].h.HHP&1)!=0)
  		  {
  		    e=cev->tg.det[pos].ge[col].seg[0].E;
  		    t=cev->tg.det[pos].ge[col].seg[0].T;
		    ring=cev->tg.det[pos].ge[col].ring;
		    if(ring>0)
		      if(ring<NRING)
			if(e>cal_par->tg.celow[pos][col])
			  if(e<cal_par->tg.cehigh[pos][col])
			    if(t>cal_par->tg.ctlow[pos][col])
			      if(t<cal_par->tg.cthigh[pos][col])
				{
				  e/=cal_par->tg.contr_e;
				  if(e>0)
				    if(e<S4K)
				      hist[ring][(int)rint(e)]++;
				}		    
  		  }
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output,*cluster;
  input_names_type* name;
  char n[132];
 

  if(argc!=2)
    {
      printf("\n ./TIGRESS_ECalRingLimCluster master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts calibrated energy histogram for TIGRESS rings with limits on TIGRESS and PINARRAY energies and times \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);

  if(name->flag.cluster_file==1)
        {
          printf("\nSorting data from cluster file:\n %s\n",name->fname.cluster_file);
	  if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	    {
	      printf("\nI can't open input file %s\n",name->fname.cluster_file);
	      exit(-2);
	    }
        }
      else
        {
          printf("\nCluster file not defined\n");
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



  name->flag.inp_data=1; 
  while(fscanf(cluster,"%s",n)!=EOF)
    {
      strcpy(name->fname.inp_data,n);
      sort(name);
    }


  if((output=fopen("ring_cal_core_energy.spn","w"))==NULL)
    {
      printf("\nI can't open file ring_cal_core_energy.spn\n");
      exit(EXIT_FAILURE);
    }
	  
  fwrite(hist,NRING*S4K*sizeof(int),1,output);
	 
  fclose(output);


}

