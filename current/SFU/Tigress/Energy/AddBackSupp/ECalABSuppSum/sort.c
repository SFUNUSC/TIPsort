/* ECalABSuppSum */

#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  double eAddBack=-1.;
  int suppFlag=0;
  int take=0;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
 
  //check the Ge fold
  if(cev->tg.h.FA>0)
    //look through each Tigress position
    for(pos=1;pos<NPOSTIGR;pos++)
      {
  	//reset suppression flag for every position
  	suppFlag=0;
  	//check if the position is in the hit pattern
  	//if((cev->tg.h.HHP&(1<<(pos-1)))!=0)
  	if((cev->tg.h.HHP&(1<<(pos-1)))!=0)
  	  if(cev->tg.det[pos].hge.FH>0)
  	    //check the fold
  	    if(cev->tg.det[pos].hge.FH>0)
  	      //check if the position is in the addback hit pattern
  	      if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
  		{
  		  //printf("AHP = %d\n",cev->tg.h.AHP);
  		  //reset take for add-back suppression
  		  take=0;
  		  //Run through four cores for each position
  		  for(col=0;col<NCOL;col++)
  		    {
		      //Check if this color is indicated in the hit pattern
		      if((cev->tg.det[pos].hge.HHP&(1<<col))!=0)
			//Check that this combination has a fold great than zero
			if(cev->tg.det[pos].ge[col].h.FH>0)
			  //Check if this combination is indicated in the hit pattern
			  if((cev->tg.det[pos].ge[col].h.HHP&1)!=0)
			    //suppress if the position is in the map and has not yet been suppressed
			    if(cev->tg.det[pos].ge[col].suppress>=supLow && cev->tg.det[pos].ge[col].suppress<=supHigh && take==0)
  			    {
  			      /* once suppression flag is set
  				 do not reset it, could remove the take bit
  				 and keep resetting suppFlag, but this
  				 is nicer */
  			      suppFlag=1;
  			      take=1;
  			    }
  		    }
		  
  		  eAddBack = cev->tg.det[pos].addback.E/cal_par->tg.contr_e;
  		  col = cev->tg.det[pos].addbackC;
  		  col += NCOL*suppFlag;
		  
  		  if(eAddBack>=0 && eAddBack<S32K)
  		    hist[pos][col][(int)(eAddBack)]++;
  		  else
  		    {
		    //printf("eAddback %d\n",eAddBack);
  		    hist[pos][col][S32K-1000]++;
  		    }
  		}
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
  
  if(argc!=4)
    {
      printf("Tigress_ECalABSuppSum master_file_name supLow supHigh\n");
      exit(-1);
    }
  
  printf("Program sorts ECalABSuppSum histograms for TIGRESS.\n");
  
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
      printf("Sorting ECalABSuppSum histograms for TIGRESS clovers and cores based upon the cluster file: %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("ERROR!!! I can't open input file %s\n",name->fname.cluster_file);
	  exit(-2);
	}
    }
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
      sprintf(mcaFile,"pos%1d%1d_ECalABSuppSum.mca",pos/10,pos-(pos/10)*10);
      if((output=fopen(mcaFile,"w"))==NULL)
	{
	  printf("ERROR!!! I cannot open the mca file!\n");
	  exit(EXIT_FAILURE);
	}
      
      for(col=0;col<NCOL*2;col++) 
	fwrite(hist[pos][col],S32K*sizeof(int),1,output);
      fclose(output);
    }
}
