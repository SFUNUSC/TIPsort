#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  double t;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  if(cev->tg.h.FT>0)
    {
      count_1++;
      count_2+=cev->tg.h.FT;
      for(pos=1;pos<NPOSTIGR;pos++)
	if((cev->tg.h.THP&(1<<(pos-1)))!=0)
	  if(cev->tg.det[pos].hge.FT>0)
	    {
	      count_3+=cev->tg.det[pos].hge.FT;
	      for(col=0;col<NCOL;col++)
		if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
		  if(cev->tg.det[pos].ge[col].h.FT>0)
		    {
		      count_4+=cev->tg.det[pos].ge[col].h.FT;
		      if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
			{
			  count_5++;
			  t=cev->tg.det[pos].ge[col].seg[0].T;
			  if(t>cal_par->tg.ctlow[pos][col] && t<cal_par->tg.cthigh[pos][col])
			    {
			      count_6++;
			      t/=cal_par->tg.contr_t;
			      if(t>0 && t<S4K)
				{
				  count_7++;
				  hist[pos][col][(int)rint(t)]++;
				}
			      else hist[pos][col][S4K-10]++;
			    }}}}}

  free(cev);
  return SEPARATOR_DISCARD;
}
/*=============================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *cluster;
  input_names_type *name;
  char n[132], DataFile[256];
  int stop;

  if(argc!=2)
    {
      printf("Tigress_TCalLimSum master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts TCalSum histograms for Tigress crystal cores from a cluster file with time limits applied.\n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);
  
  if(name->flag.cluster_file==1)
    {
      printf("Sorting calibrated time histograms for TIGRESS clovers and cores based upon the cluster file: %s\n",name->fname.cluster_file);
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
      printf("TIGRESS calibration read from the file: %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);	  
    }
  else
    {
      printf("ERROR! TIGRESS calibration parameters not defined\n");
      exit(EXIT_FAILURE);
    }
  
  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting timing (CFD algorithm) data from file %s\n",DataFile);
      sort(name);
    }
  
  for(pos=1;pos<NPOSTIGR;pos++)
    {
      stop=0;
      for(col=0;col<NCOL;col++)
	stop+=cal_par->tg.ceflag[pos][col];
      if(stop>0)
	{
	  sprintf(n,"pos%1d%1d_CalSumCoreTimeLimits.spn",pos/10,pos-(pos/10)*10);
	  if((output=fopen(n,"w"))==NULL)
	    {
	      printf("ERROR! I can't open file %s\n",n);
	      exit(EXIT_FAILURE);
	    }
	  for(col=0;col<NCOL;col++)
	    fwrite(hist[pos][col],S4K*sizeof(int),1,output);
	  fclose(output);
	}}
 
  printf("The event-dropper increments are %d %d %d %d %d %d %d.\n",count_1,count_2,count_3,count_4,count_5,count_6,count_7);
}
