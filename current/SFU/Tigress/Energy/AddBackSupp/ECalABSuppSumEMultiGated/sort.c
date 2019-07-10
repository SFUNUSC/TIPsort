#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int suppFlag=0;
  int take=0;
  int i=0;
  int j=0;
  int k=0;

  double* energy;
  int* ring;
  int* inGate;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  energy=(double*)calloc(cev->tg.h.FA,sizeof(double));
  ring=(int*)calloc(cev->tg.h.FA,sizeof(int));
  inGate=(int*)calloc(cev->tg.h.FA,sizeof(int));
  
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
			          ring[i]=cev->tg.det[pos].ge[colAddBack].ring+NRING*suppFlag;
			          
			          if(energy[i]>=0 && energy[i]<S32K)
                	projhist[ring[i]][(int)(energy[i])]++;

                //reset gate flag
                inGate[i] = -1;
			          
			          i++;//increment event counter
			          
			        }
      }
  
  if(i!=cev->tg.h.FA)
    printf("WARNING: Addback fold not equal to the number of events seen!\n");
  
  //set gate flags
  for(i=0;i<cev->tg.h.FA;i++)
    {
      //look for a gamma that falls into the gate
      if(energy[i]>=0)
	      if(energy[i]<S32K)
          for(k=0;k<numGates;k++)
            if(energy[i]>=gateELow[k]/cal_par->tg.contr_e)
              if(energy[i]<=gateEHigh[k]/cal_par->tg.contr_e){
                inGate[i] = k; //flag gamma as in a gate
              }
      if(inGate[i]>=0)
        gatehist[ring[i]][(int)(energy[i])]++;
    }

  //find number of unique gates gammas are in
  int numUniqueGates = 0;
  for(k=0;k<numGates;k++)
    {
      for(i=0;i<cev->tg.h.FA;i++)
        if(inGate[i] == k){
          numUniqueGates++;
          break;
        }
    }

  //evaluate gate
  if(andor == 1){
    //AND mode
    if(numUniqueGates == numGates){
      //add all gammas in the event that aren't the gammas that fell into the gates
      for(j=0;j<cev->tg.h.FA;j++)
        {
          if(inGate[j] < 0) //not in a gate
            if(energy[j]>=0)
              if(energy[j]<S32K)
                if(ring[j]>0)
                  if(ring[j]<NRING)
                    hist[ring[j]][(int)(energy[j])]++;
        }
    }

  }else{
    //OR mode
    if(numUniqueGates > 0){
      //add all gammas in the event that aren't the gammas that fell into the gates
      for(j=0;j<cev->tg.h.FA;j++)
        {
          if(inGate[j] < 0) //not in a gate
            if(energy[j]>=0)
              if(energy[j]<S32K)
                if(ring[j]>0)
                  if(ring[j]<NRING)
                    hist[ring[j]][(int)(energy[j])]++;
        }
    }
  }
  
  //printf("***** END OF EVENT *****\n");
  //getc(stdin);
  
  free(cev);
  free(energy);  
  free(ring);
  free(inGate);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  FILE *cluster;
  char n[132],FileName[132], inp[256];
  double avgGateE;
  double gateWidth;
  double val;

  if(argc!=5)
    {
      printf("TIGRESS_ECalABSuppSumEMultiGated master_file_name supLow supHigh AND/OR\n");
      printf("Program attempts to generate energy gated gamma ray spectra (not background subtracted).  AND/OR is set to 0 for OR mode, or 1 for AND mode.\n");
      exit(-1);
    }
  
  printf("Program attempts to generate energy gated gamma ray spectra.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  memset(hist,0,sizeof(hist));
  memset(gatehist,0,sizeof(gatehist));
  memset(projhist,0,sizeof(projhist));
  read_master(argv[1],name);
  
  supLow=atof(argv[2]);
  supHigh=atof(argv[3]);

  if(atoi(argv[4]) == 1){
    andor = 1;
    printf("Set gates to AND mode.\n");
  }else{
    andor = 0;
    printf("Set gates to OR mode.\n");
  }

  numGates = 0;
  while(numGates < MAXNUMGATES){
    printf("Enter a gate range in keV (2 numbers separated by a space, -1 if done):\n");
    scanf("%[^\n]%*c",inp); //get string with spaces, until newline
    //printf("input: %s\n",inp);
    if((sscanf(inp,"%lf %lf",&gateELow[numGates],&gateEHigh[numGates])) == 2){
      avgGateE=(gateELow[numGates]+gateEHigh[numGates])/2.;
      printf("Gate between %f and %f keV added (average %f keV).\n",gateELow[numGates],gateEHigh[numGates],avgGateE);
      if(gateELow[numGates]>gateEHigh[numGates]){
        printf("ERROR: cannot have lower gate bound larger than upper gate bound.  Try again:\n");
      }else{
        numGates++;
      }
    }else if((sscanf(inp,"%lf",&val)) == 1){
      if(val < 0){
        break;
      }
    }else{
      printf("Didn't understand input.  Try again:\n");
    }
  }

  printf("Finished adding %i gate(s).\n", numGates);  
  	
  avgGateE=(gateELow[0]+gateEHigh[0])/2.;
  gateWidth=gateEHigh[0]-gateELow[0];

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
  
  printf("\n");
  
  name->flag.inp_data=1; 
  while(fscanf(cluster,"%s",n)!=EOF)
    {
      strcpy(name->fname.inp_data,n);
      printf("Sorting data from file %s\n", name->fname.inp_data);
      sort(name);
    }

  fclose(cluster);
  
  printf("\n");

  sprintf(FileName,"ECalABSuppSum_c%.0f_w%.0fANDORgated.mca",avgGateE,gateWidth);
  if((output=fopen(FileName,"w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(hist,2*NRING*S32K*sizeof(int),1,output);
  fclose(output);
  printf("Gated spectrum saved to file: %s\n",FileName);
  
  sprintf(FileName,"ECalABSuppSum_c%.0f_w%.0fANDORgate.mca",avgGateE,gateWidth);
  if((output=fopen(FileName,"w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(gatehist,2*NRING*S32K*sizeof(int),1,output);
  fclose(output);
  printf("Gate spectrum saved to file: %s\n",FileName);
  
  sprintf(FileName,"ECalABSuppSum_c%.0f_w%.0fproj.mca",avgGateE,gateWidth);
  if((output=fopen(FileName,"w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(projhist,2*NRING*S32K*sizeof(int),1,output);
  fclose(output);
  printf("Projection spectrum saved to file: %s\n",FileName);

}

  

