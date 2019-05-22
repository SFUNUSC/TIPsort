/* ECalABSuppRingSumRingDopplerCorrected */

#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  double eAddBack=0.;
  int suppFlag=0;
  int take=0;
  int i=0;
  int j=0;

  double* energy;
  int* ring;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  energy=(double*)calloc(cev->tg.h.FA,sizeof(double));
  ring=(int*)calloc(cev->tg.h.FA,sizeof(int));

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
                
                eAddBack = cev->tg.det[pos].addback.E/cal_par->tg.contr_e;  
                colAddBack = cev->tg.det[pos].addbackC;
                if(cev->tg.det[pos].ge[colAddBack].ring<NRING)
                  eAddBack=eAddBack*dshift[cev->tg.det[pos].ge[colAddBack].ring];
					   
		            energy[i]=eAddBack;
			          colAddBack=cev->tg.det[pos].addbackC;
			          ring[i]=cev->tg.det[pos].ge[colAddBack].ring+NRING*suppFlag;
			          i++;
			          //printf("E = %f at pos %d col %d ring %d suppFlag1 = %d\n",cev->tg.det[pos].addback.E/cal_par->tg.contr_e,pos,colAddBack,cev->tg.det[pos].ge[colAddBack].ring,suppFlag);
			        }
      }
  
  if(i!=cev->tg.h.FA)
    printf("WARNING: Addback fold not equal to the number of events seen!\n");
  
  for(i=0;i<cev->tg.h.FA;i++)
    {
      //look for a gamma that falls into the gate
      if(energy[i]>=0)
	      if(energy[i]<S32K)
	        if(energy[i]>=gateLow/cal_par->tg.contr_e)
		        if(energy[i]<=gateHigh/cal_par->tg.contr_e)
		          if(ring[i]>0)
		            if(ring[i]<NRING)
		              {
		                //add all gammas in the event that aren't the gamma that fell into the gate
		                for(j=0;j<cev->tg.h.FA;j++)
		                  if(j!=i)
		                    if(energy[j]>=0)
	                        if(energy[j]<S32K)
		                        if(ring[j]>0)
		                          if(ring[j]<NRING)
		                            hist[ring[j]][(int)(energy[j])]++;
		                break;//don't double count
		              }
    }

  //printf("***** END OF EVENT *****\n");
  //getc(stdin);
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*=========================================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *cluster;
  input_names_type* name;
  char DataFile[132];
  char mcaFile[132];
  int ring1=0;
  double avgGateE, gateWidth;
  
  if(argc!=7)
    {
      printf("Tigress_ECalABSuppRingSumRingDopplerCorrectedEGated master_file_name supLow supHigh shift_file gateLow gateHigh\n");
      printf("Program sorts ECalABSuppRingSumRingDopplerCorrected histograms for TIGRESS.\n");
      printf("The shift file specifies energy correction factors for each ring (one correction factor per line, starting with ring 0).\n");
      printf("The energy gate applies to Doppler corrected energies, and is in keV.\n");
      exit(-1);
    }
  
  printf("Program sorts ECalABSuppRingSumRingDopplerCorrected histograms for TIGRESS.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);

  supLow = atof(argv[2]);
  supHigh = atof(argv[3]);
  gateLow = atof(argv[5]);
  gateHigh = atof(argv[6]);

  if(gateHigh <= gateLow){
    printf("ERROR: energy gate width is zero or negative!\n");
    exit(-1);
  }
  
  if(name->flag.cluster_file==1)
    {
      printf("Sorting ECalABSuppRingSum histograms for TIGRESS clovers and cores based upon the cluster file: %s\n",name->fname.cluster_file);
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
  
  //get doppler shifts  
  if((shiftFile=fopen(argv[4],"r"))==NULL)
    {
      printf("ERROR!!! Cannot open the ring doppler shift file %s\n",argv[4]);
      printf("Press ENTER to exit.\n");
      getc(stdin);
      exit(-1);
    }
  num_rings=0;
  while(!(feof(shiftFile)))//go until the end of file is reached
    {
      if(num_rings<(NRING+1))
        if(fgets(str,256,shiftFile)!=NULL)
          {
            dshift[num_rings]=atof(str);
            num_rings++;
          }
    }
  num_rings--;
  fclose(shiftFile);
  
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

  fclose(cluster);
  
  avgGateE = (gateLow + gateHigh)/2.;
  gateWidth = gateHigh - gateLow;
  sprintf(mcaFile,"Ring_ECalABSuppSumEGated_c%.0f_w%.0f.mca",avgGateE,gateWidth);
  if((output=fopen(mcaFile,"w"))==NULL)
  	{
  	  printf("ERROR!!! I cannot open the output file: %s\n",mcaFile);
  	  exit(EXIT_FAILURE);
  	}

  for(ring1=0;ring1<NRING*2;ring1++)
    {
    	fwrite(hist[ring1],S32K*sizeof(int),1,output);   
    }
  fclose(output);
  printf("Spectrum written to file: %s\n",mcaFile);
}
