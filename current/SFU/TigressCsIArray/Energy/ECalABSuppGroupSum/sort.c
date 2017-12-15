#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos,col,group,csi;
  double etig;
  int suppFlag=0;
  int take=0;
  
 if((data->h.setupHP&TIGRESS_BIT)==0)
    return SEPARATOR_DISCARD;

  if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;

 if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);

  /* Require good time and energy for the event */
  if(cev->csiarray.h.FH==1)
    if(cev->tg.h.FA>0)
      for(pos=1;pos<NPOSTIGR;pos++)
	{
	  //reset suppression flag for every position
	  suppFlag=0;
	  if((cev->tg.h.HHP&(1<<(pos-1)))!=0)
  	    if(cev->tg.det[pos].hge.FH>0)
  	      if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
  		{
  		  //reset take for add-back suppression
  		  take=0;
  		  for(col=0;col<NCOL;col++)
  		    {
  		      if((cev->tg.det[pos].hge.HHP&(1<<col))!=0)
  			if(cev->tg.det[pos].ge[col].h.FH>0)
			  {
			    //suppress if the position is in the map and has not yet been suppressed
			    if(cev->tg.det[pos].ge[col].suppress>=supLow && cev->tg.det[pos].ge[col].suppress<=supHigh)
			      if(take==0)
				{
				  /* once suppression flag is set
				     do not reset it, could remove the take bit
				     and keep resetting suppFlag, but this
				     is nicer */
				  suppFlag=1;
				  take=1;
				}
			  }
  		    }
		}
	  
	  etig = cev->tg.det[pos].addback.E/cal_par->tg.contr_e;
	  col = cev->tg.det[pos].addbackC;
	  
	  for(csi=1;csi<NCSI;csi++)
	    if((cev->csiarray.h.HHP[csi/64]&(one<<csi%64))!=0)
	      {
		group = cal_par->tg.group_map[pos][col][csi]+NGROUP*suppFlag;

		if(etig<0 || etig>S32K-10)
		  etig=S32K-10;
		
		hist[group][(int)etig]++;
	      }
	}

  free(cev);
  return SEPARATOR_DISCARD;
}
/*=============================================================================*/
int main(int argc, char *argv[])
{
  FILE *output;
  input_names_type* name;
  FILE *cluster;
  char DataFile[132];

  if(argc!=4)
    {
      printf("Tigress_ECalABSuppGroup master_file_name supLow supHigh\n");
      exit(-1);
    }
  
  printf("Program sorts ECalABGroup histograms for TIGRESS.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));  
  memset(hist,0,sizeof(hist));

  read_master(argv[1],name);
  supLow = atof(argv[2]);
  supHigh = atof(argv[3]);

  if(name->flag.cluster_file==1)
    {
      printf("Sorting ECalABSuppRingSum_PIDr energy histograms for TIGRESS cores based upon the cluster file: %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
  	{
  	  printf("ERROR! I can't open input file %s\n",name->fname.cluster_file);
  	  exit(-2);
  	}
    }
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
  if(name->flag.CSIARRAY_cal_par==1)
    {
      printf("CsIArray calpar read from: %s\n",name->fname.CSIARRAY_cal_par);
      initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
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
      
      printf("Sorting data from %s\n",name);
      sort(name);
    }

  if((output=fopen("Group_ECalABSuppSum.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  
  fwrite(hist,2*NGROUP*S32K*sizeof(int),1,output);

  fclose(output);

  return 0;
}
