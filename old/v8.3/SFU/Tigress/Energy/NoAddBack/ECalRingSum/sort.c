#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos,col,ring;
  double eCal;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);

  if(cev->tg.h.FE>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.EHP&(1<<(pos-1)))!=0)
  	if(cev->tg.det[pos].hge.FE>0)
  	  for(col=0;col<NCOL;col++)
  	    if((cev->tg.det[pos].hge.EHP&(1<<col))!=0)
  	      if(cev->tg.det[pos].ge[col].h.FE>0)
  		if((cev->tg.det[pos].ge[col].h.EHP&1)!=0)
  		  {
  		    eCal=cev->tg.det[pos].ge[col].seg[0].E/cal_par->tg.contr_e;
		    ring=cev->tg.det[pos].ge[col].ring;
		    if((eCal>0) && (eCal<S32K))
		      {
			if((ring>0) && (ring<NRING)) hist[ring][(int)rint(eCal)]++;
		      }
		      else hist[ring][S32K-1000]++;
  		  }
  free(cev);
  return SEPARATOR_DISCARD;
}
/*=============================================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *cluster;
  input_names_type* name;
  char DataFile[132];
 
  if(argc!=2)
    {
      printf("Tigress_ECalRingSum master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts ECalRingSum histograms for TIGRESS.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));

  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);

  if(name->flag.cluster_file==1)
    {
      printf("Sorting calibrated energy histograms for TIGRESS clovers and cores based upon the cluster file: %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
        {
          printf("ERROR! I can't open input file %s\n",name->fname.cluster_file);
          exit(-2);
        }}
  else
    {
      printf("ERROR! Cluster file not defined\n");
      exit(-1);
    }
  
  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calibration read from %s.\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR!!! TIGRESS calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }

  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting gamma-ray energy data from file %s\n", name);
      sort(name);
    }
  
  if((output=fopen("Ring_ECalSum.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  
  fwrite(hist,NRING*S32K*sizeof(int),1,output);
  fclose(output);
}
