#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  double eAddBack=0,ePreAddBack=0;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  if(cev->tg.h.FH>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if(((cev->tg.h.HHP&(1<<(pos-1)))!=0) && (cev->tg.det[pos].hge.FE>0))
	if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
	  {
	    /*
	    //Let's output some samples of add back at work...
	    for(col=0;col<NCOL;col++)
	      if(((cev->tg.det[pos].hge.HHP&(1<<col))!=0) && (cev->tg.det[pos].ge[col].h.FH>0))
		{
		  ePreAddBack = cev->tg.det[pos].ge[col].seg[0].E/cal_par->tg.contr_e;
		  if(cev->tg.det[pos].hge.FE > 2) printf("For Position %d and Core %d the energy is %f.\n",pos,col,ePreAddBack);
		}
	    */
	    eAddBack = cev->tg.det[pos].addback.E/cal_par->tg.contr_e;  
	    col = cev->tg.det[pos].addbackC;
	    if(eAddBack>=0 && eAddBack<S32K) hist[pos][col][(int)(eAddBack)]++;
	    else hist[pos][col][S32K-1000]++;
	    /*
	      if(cev->tg.det[pos].hge.FE > 2) printf("The add back energy is ---> %f.\n",eAddBack);
	    */
	  }

  free(cev);
  return SEPARATOR_DISCARD;
}
/*=========================================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *cluster;
  input_names_type* name;
  char mcaFile[132],DataFile[132];
  
  if(argc!=2)
    {
      printf("Tigress_ECalABSum master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts ECalABSum histograms for TIGRESS.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);

  if(name->flag.cluster_file==1)
    {
      printf("Sorting ECalABSum histograms for TIGRESS clovers and cores based upon the cluster file: %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("ERROR!!! I can't open input file %s\n",name->fname.cluster_file);
	  exit(-2);
	}}
  else
    {
      printf("ERROR!!! Cluster file not defined\n");
      exit(-1);
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
  
  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting data from file %s\n", name);
      sort(name);
    }
  
  for(pos=1;pos<NPOSTIGR;pos++)
    {
      sprintf(mcaFile,"pos%1d%1d_ECalABSum.mca",pos/10,pos-(pos/10)*10);
      if((output=fopen(mcaFile,"w"))==NULL)
	{
	  printf("ERROR!!! I cannot open the mca file!\n");
	  exit(EXIT_FAILURE);
	}
      
      for(col=0;col<NCOL;col++) fwrite(hist[pos][col],S32K*sizeof(int),1,output);
      fclose(output);
    }}
