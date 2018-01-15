#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int suppFlag=0;
  int take=0;
  int i=0;
  int j=0;
  int csi1,csi2;

  double* energy;
  int* group;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  energy=(double*)calloc(cev->tg.h.FA,sizeof(double));
  group=(int*)calloc(cev->tg.h.FA,sizeof(int));
  
  if(cev->tg.h.FA>0) //addback fold>0
    //look through each Tigress position
    for(pos=1;pos<NPOSTIGR;pos++)
      {
      	//reset suppression flag for every position
      	suppFlag=0;
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
					   
		            energy[i]=cev->tg.det[pos].addback.E/cal_par->tg.contr_e;
			          colAddBack=cev->tg.det[pos].addbackC;

                for(csi1=1;csi1<NCSI;csi1++)
                  if((cev->csiarray.h.HHP[csi1/64]&(one<<csi1%64))!=0)
                    {
                      if(cal_par->tg.groupflag[pos][colAddBack][csi1][0]==1)
                        {
                          //groups defined by one csi hit
                          group[i] = cal_par->tg.group_map[pos][colAddBack][csi1][0]+NGROUP*suppFlag;
                        }
                      else
                        {
                          //groups defined by two csi hits
                          for(csi2=csi1+1;csi2<NCSI;csi2++)
                            if((cev->csiarray.h.HHP[csi2/64]&(one<<csi2%64))!=0)
                              if(cal_par->tg.groupflag[pos][colAddBack][csi1][csi2]==2)
                                {
                                  group[i] = cal_par->tg.group_map[pos][colAddBack][csi1][csi2]+NGROUP*suppFlag;
                                }
                        }
                    }

			          i++;
			          
			        }
      }
  
  if(i!=cev->tg.h.FA)
    printf("WARNING: Addback fold not equal to the number of events seen!\n");
  
  for(i=0;i<cev->tg.h.FA;i++)
    {
      //look for a gamma that falls into the gate
      if(energy[i]>=0)
	      if(energy[i]<S32K)
	        if(energy[i]>=cal_par->tg.gelow[group[i]]/cal_par->tg.contr_e)
		        if(energy[i]<=cal_par->tg.gehigh[group[i]]/cal_par->tg.contr_e)
		          if(group[i]>0)
		            if(group[i]<NGROUP)
		              {
		                //add all gammas in the event that aren't the gamma that fell into the gate
		                for(j=0;j<cev->tg.h.FA;j++)
		                  if(j!=i)
		                    if(energy[j]>=0)
	                        if(energy[j]<S32K)
		                        if(group[j]>0)
		                          if(group[j]<NGROUP)
		                            hist[group[j]][(int)(energy[j])]++;
		                break;//don't double count
		              }        
	      
      /*for(j=i+1;j<cev->tg.h.FA;j++)
	      {
	        //printf("energy[%d] = %.2f in ring %d\nenergy[%d] = %.2f in ring %d\n",i,energy[i],ring[i],j,energy[j],ring[j]);
	        if(energy[i]>=0)
	          if(energy[i]<S32K)
	            if(energy[j]>=cal_par->tg.relow[ring[j]]/cal_par->tg.contr_e)
		            if(energy[j]<=cal_par->tg.rehigh[ring[j]]/cal_par->tg.contr_e)
		              if(ring[j]>0)
		                if(ring[j]<NRING)
		                  {
			                  //printf("====> event [%d] should be incremented in ring %d\n",i,ring[i]);
			                  hist[ring[i]][ring[j]][(int)(energy[i])]++;
		                  }
	        if(energy[j]>=0)
	          if(energy[j]<S32K)
	            if(energy[i]>=cal_par->tg.relow[ring[i]]/cal_par->tg.contr_e)
		            if(energy[i]<=cal_par->tg.rehigh[ring[i]]/cal_par->tg.contr_e)
		              if(ring[i]>0)
		                if(ring[i]<NRING)
		                  {
			                  //printf("====> event [%d] should be incremented in ring %d\n",j,ring[j]);
			                  hist[ring[j]][ring[i]][(int)(energy[j])]++;
		                  }
	      }*/
    }
  
  //printf("***** END OF EVENT *****\n");
  //getc(stdin);
  
  free(cev);
  free(energy);  
  free(group);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  FILE *cluster;
  char n[132];
  char mcaFile[132];
  int group=0;
  double avgGateE;

  if(argc!=4)
    {
      printf("TigressCsIArray_ECalABSuppGroupSumEGatedByGroup master_file_name supLow supHigh\n");
      printf("Program sorts Doppler shift group spectra for TIGRESS with an energy gate applied to all groups.\nGroup energy gates are specified in TIGRESS_par.dat under the entry 'Group_energy_gates'\n");
      exit(-1);
    }
  
  printf("Program sorts Doppler shift group spectra for TIGRESS with an energy gate applied to all groups.\nGroup energy gates are specified in TIGRESS_par.dat under the entry 'Group_energy_gates'\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  memset(hist,0,sizeof(hist));
  /* memset(mat,0,sizeof(mat)); */
  read_master(argv[1],name);
  
  supLow=atof(argv[2]);
  supHigh=atof(argv[3]);

  /* h = new TH2D("Tigress EECalABSupp Sum","Tigress EECalABSuppSum",S1K,0,S4K-1,S1K,0,S4K-1); */
  /* h->Reset(); */

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
  
  if(name->flag.CSIARRAY_cal_par==1)
    {
      printf("CsIArray calpar read from: %s\n",name->fname.CSIARRAY_cal_par);
      initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
    }
  else
    {
      printf("\nCSIARRAY calibration parameters not defined\n");
      exit(EXIT_FAILURE);
    }

  avgGateE=0.;
  int numGates=0;
  printf("\nTIGRESS group gates:\n");
  for(int i=0;i<NGROUP;i++)
  	{
  		printf("Group %i: %f keV to %f keV\n",i,cal_par->tg.gelow[i],cal_par->tg.gehigh[i]);
      if(cal_par->tg.gelow[i]!=0.)
        {
          avgGateE+=(cal_par->tg.gelow[i] + cal_par->tg.gehigh[i])/2.;
          numGates++;
        }
      
  	}
  printf("\n");
  avgGateE=avgGateE/numGates;
    
  name->flag.inp_data=1; 
  while(fscanf(cluster,"%s",n)!=EOF)
    {
      strcpy(name->fname.inp_data,n);
      sort(name);
    }

  fclose(cluster);

	sprintf(mcaFile,"Ring_ECalABSuppSumEGated_c%.0f.mca",avgGateE);
  if((output=fopen(mcaFile,"w"))==NULL)
  	{
  	  printf("ERROR!!! I cannot open the output file: %s\n",mcaFile);
  	  exit(EXIT_FAILURE);
  	}

  for(group=0;group<NGROUP*2;group++)
    {
    	fwrite(hist[group],S32K*sizeof(int),1,output);   
    }
  fclose(output);
  printf("Spectrum written to file: %s\n",mcaFile);
}

  

