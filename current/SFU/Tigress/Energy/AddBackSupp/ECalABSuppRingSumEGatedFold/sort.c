/* ECalABSuppRingSumEGated */

#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int suppFlag=0;
  int take=0;
  int i=0;
  int j=0;
  double ePreAddBack=0.;
  double eAddBack=0.;

  double* energy;
  int* ring;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  energy=(double*)calloc(cev->tg.h.FA,sizeof(double));
  ring=(int*)calloc(cev->tg.h.FA,sizeof(int));
    
  if(cev->tg.h.FA==fold)
    for(pos=1;pos<NPOSTIGR;pos++)
      {
  	//reset suppression flag for every position
  	suppFlag|=0;
  	//check if the position is in the hit pattern
  	if((cev->tg.h.HHP&(1<<(pos-1)))!=0)
	  //check the position fold
	  if(cev->tg.det[pos].hge.FH>0)
	    //check if the position is in the addback hit pattern
	    if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
	      {
		//reset take for add-back suppression
		take=0;
		//Run through four cores for each position
		for(col=0;col<NCOL;col++)
		  {
		    //check if the position and color is in the hit pattern
		    if((cev->tg.det[pos].hge.HHP&(1<<col))!=0)
		      //check the fold
		      if(cev->tg.det[pos].ge[col].h.FH>0)			
			{
			  //suppress if the position is in the map and has not yet been suppressed
			  if(cev->tg.det[pos].ge[col].suppress>=supLow && cev->tg.det[pos].ge[col].suppress<=supHigh && take==0)
			    {
			      /* once suppression flag is set
				 do not reset it, could remove the take bit
				 and keep resetting suppFlag, but this
				 is nicer */
			      suppFlag=1;
			      take=1;
			      //printf("event at pos %d col %d suppressed\n",pos,col);
			    }
			}
		  }
	      }
      }
  
  //check the Ge add-back fold
  if(cev->tg.h.FA==fold)
    //look through each Tigress position
    for(pos=1;pos<NPOSTIGR;pos++)
      {
	//check if the position is in the hit pattern
	if((cev->tg.h.HHP&(1<<(pos-1)))!=0)
	  //check the position fold
	  if(cev->tg.det[pos].hge.FH>0)
	    //check if the position is in the addback hit pattern
	    if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
	      {
	      //Run through four cores for each position
	      for(col=0;col<NCOL;col++)
		//check if the position and color is in the hit pattern
		if((cev->tg.det[pos].hge.HHP&(1<<col))!=0)
		  //check the fold
		  if(cev->tg.det[pos].ge[col].h.FH>0)
		    {
		      /* ePreAddBack=cev->tg.det[pos].ge[col].seg[0].E/cal_par->tg.contr_e; */
		      /* if(ePreAddBack>0) */
		      /* if(ePreAddBack<S32K)  */
		      /* printf("ePreAddback %.2f pos %d col %d suppFlag %d\n",ePreAddBack,pos,col,suppFlag); */
		    }
		      
	      energy[i]=cev->tg.det[pos].addback.E/cal_par->tg.contr_e;
	      colAddBack=cev->tg.det[pos].addbackC;
	      ring[i]=cev->tg.det[pos].ge[colAddBack].ring+NRING*suppFlag;
	      i++;
	      //printf("E = %f at pos %d col %d ring %d suppFlag1 = %d\n",cev->tg.det[pos].addback.E/cal_par->tg.contr_e,pos,colAddBack,cev->tg.det[pos].ge[colAddBack].ring,suppFlag);
	      }
      }
  
  
  //printf("***** PAIRS *****\n");
  for(i=0;i<fold;i++)
    {
      for(j=i+1;j<fold;j++)
				{
					//printf("energy[%d] = %.2f in ring %d\nenergy[%d] = %.2f in ring %d\n",i,energy[i],ring[i],j,energy[j],ring[j]);
					if(energy[i]>=0)
						if(energy[i]<S32K)
							if(energy[j]>=cal_par->tg.relow[ring[j]])
								if(energy[j]<=cal_par->tg.rehigh[ring[j]])
									if(ring[j]>0)
										if(ring[j]<NRING)
											{
												//printf("====> event [%d] should be incremented in ring %d\n",i,ring[i]);
												hist[ring[i]][(int)(energy[i])]++;
											}
					if(energy[j]>=0)
						if(energy[j]<S32K)
							if(energy[i]>=cal_par->tg.relow[ring[i]])
								if(energy[i]<=cal_par->tg.rehigh[ring[i]])
									if(ring[i]>0)
										if(ring[i]<NRING)
											{
												//printf("====> event [%d] should be incremented in ring %d\n",j,ring[j]);
												hist[ring[j]][(int)(energy[j])]++;
											}
					//printf("----------\n");
				}
    }
  
  //printf("***** END OF EVENT *****\n");
  //getc(stdin);
  
  free(cev);
  free(energy);  
  free(ring);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  FILE *cluster;
  char n[132];

  if(argc!=5)
    {
      printf("TIGRESS_ECalABSuppRingSumEGatedFold master_file_name supLow supHigh fold\n");
      printf("Program sorts ring spectra for TIGRESS with an energy gate applied to all rings \n");
      exit(-1);
    }
  
  printf("Program sorts ring spectra for TIGRESS with an energy gate applied to all rings \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);
  supLow=atof(argv[2]);
  supHigh=atof(argv[3]);
  fold=atoi(argv[4]);

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

  fclose(cluster);

  if((output=fopen("Ring_ECalABSuppSumEGated.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(hist,2*NRING*S32K*sizeof(int),1,output);
  fclose(output);
}

  

