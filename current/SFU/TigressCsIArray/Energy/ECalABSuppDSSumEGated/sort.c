#include "sort.h"

//checks whether a value falls within the range of two bounds (lower bound inclusive)
//the order of the bounds doesn't matter
bool valueInRange(double value, double bound1, double bound2)
{

  if(bound1>bound2)
    {
      if((value>=bound2)&&(value<bound1))
        return true;
      else
        return false;
    }
  else if(bound2>bound1)
    {
      if((value>=bound1)&&(value<bound2))
        return true;
      else
        return false;
    }
  else if(bound1==bound2)
    {
      if(value==bound1)
        return true;
      else
        return false;
    }
    
  return false;

}
/*=========================================================================*/
int analyze_data(raw_event *data)
{
  cal_event* cev;
  int suppFlag=0;
  int take=0;
  int i=0;
  int j=0;
  double eAddBack=0;
  double ecsi;
  int csi;
  unsigned long long int one=1;

  double* energy;
  int* dsring;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  
  energy=(double*)calloc(cev->tg.h.FA,sizeof(double));
  dsring=(int*)calloc(cev->tg.h.FA,sizeof(int));
  
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
			          colAddBack=cev->tg.det[pos].addbackC;
			          
			          if(eAddBack>=0 && eAddBack<S32K)
                  {
                    //construct vectors
                    for(int ind=0;ind<3;ind++)
                      gamma_dir[ind]=cal_par->tg.tpos_xyz[pos][colAddBack][ind];
                    vecMag=sqrt(gamma_dir[0]*gamma_dir[0] + gamma_dir[1]*gamma_dir[1] + gamma_dir[2]*gamma_dir[2]);
                    for(int ind=0;ind<3;ind++)
                      gamma_dir[ind]=gamma_dir[ind]/vecMag;//make unit vector
                    
                    if(cev->csiarray.h.FH>0)
                      for(csi=1;csi<NCSI;csi++)
                        if((cev->csiarray.h.HHP&(one<<csi))!=0)
                          {
                            for(int ind=0;ind<3;ind++)
                              part_p[csi][ind]=cal_par->csiarray.cpos_xyz[csi][ind];
                            vecMag=sqrt(part_p[csi][0]*part_p[csi][0] + part_p[csi][1]*part_p[csi][1] + part_p[csi][2]*part_p[csi][2]);
                            ecsi=cev->csiarray.csi[csi].E/1000.; /* CsI energy in MeV */
                            for(int ind=0;ind<3;ind++)
                              part_p[csi][ind]=part_p[csi][ind]*(sqrt(2*ecsi*cal_par->csiarray.mp))/vecMag;//make vector proper length for momentum
                          }
              
                    //subtract detected particle momenta from beam momentum to get residual momentum
                    for(int ind=0;ind<3;ind++)
                      res_p[ind]=beam_p[ind];
                    for(csi=1;csi<NCSI;csi++)
                      for(int ind=0;ind<3;ind++)
                        res_p[ind]=res_p[ind]-part_p[csi][ind];
                    //make momentum vector into direction vector  
                    for(int ind=0;ind<3;ind++)
                      res_dir[ind]=res_p[ind]/sqrt(res_p[0]*res_p[0] + res_p[1]*res_p[1] + res_p[2]*res_p[2]);
                    //calculate speed of residual
                    beta=sqrt(res_p[0]*res_p[0] + res_p[1]*res_p[1] + res_p[2]*res_p[2]);
                    beta/=cal_par->csiarray.mr;
                    //printf("beta: %f\n",beta);
                    
                    //calculate ds
                    ds=sqrt(1-beta*beta) / (1-(beta*(res_dir[0]*gamma_dir[0] + res_dir[1]*gamma_dir[1] + res_dir[2]*gamma_dir[2])));
                	}
			          
			          for(int ind=0;ind<num_gates;ind++)
                	if(valueInRange(ds,gates[ind],gates[ind+1]))
                  	dsring[i]=ind + num_gates*suppFlag;
			          
			          energy[i]=eAddBack;
			          dsring[i]=cev->tg.det[pos].ge[colAddBack].ring+NRING*suppFlag;
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
	        if(energy[i]>=cal_par->tg.relow[dsring[i]]/cal_par->tg.contr_e)
		        if(energy[i]<=cal_par->tg.rehigh[dsring[i]]/cal_par->tg.contr_e)
		          if(dsring[i]>0)
		            if(dsring[i]<num_gates)
		              {
		                //add all gammas in the event that aren't the gamma that fell into the gate
		                for(j=0;j<cev->tg.h.FA;j++)
		                  if(j!=i)
		                    if(energy[j]>=0)
	                        if(energy[j]<S32K)
		                        if(dsring[j]>0)
		                          if(dsring[j]<NRING)
		                            hist[dsring[j]][(int)(energy[j])]++;
		                break;//don't double count
		              }
    }
  
  //printf("***** END OF EVENT *****\n");
  //getc(stdin);
  
  free(cev);
  free(energy);  
  free(dsring);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  FILE *cluster;
  char n[132];
  int ring=0;

  if(argc!=5)
    {
      printf("TIGRESS_ECalABSuppSumEGated master_file_name supLow supHigh gate_file\n");
      printf("Program sorts Doppler shift gated spectra for TIGRESS with an energy gate applied to all Doppler shift gates.\n");
      printf("Energy gates are specified in the parameter files.\n");
      exit(-1);
    }
  
  printf("Program sorts ring spectra for TIGRESS with an energy gate applied to all rings \n");
  if(num_gates>NGATES)
  	{
  		printf("ERROR: number of gates is larger than NGATES.  Exiting...\n");
  		exit(-1);
  	}
  
  
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

	//get gates  
  if((gateFile=fopen(argv[4],"r"))==NULL)
    {
      printf("ERROR!!! Cannot open the custom gate file %s\n",argv[4]);
      exit(-1);
    }
  num_gates=0;
  while(!(feof(gateFile)))//go until the end of file is reached
    {
      if(num_gates<(NGATES+1))
        if(fgets(str,256,gateFile)!=NULL)
          {
            gates[num_gates]=atof(str);
            num_gates++;
          }
    }
  num_gates--;
  fclose(gateFile);
  for(int ind=0;ind<num_gates;ind++)
    printf("Gate %i from %f to %f\n",ind,gates[ind],gates[ind+1]);
  //getc(stdin);

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
      printf("ERROR! CsIArray calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  
  name->flag.inp_data=1; 
  while(fscanf(cluster,"%s",n)!=EOF)
    {
      strcpy(name->fname.inp_data,n);
      sort(name);
    }

  fclose(cluster);

  if((output=fopen("DS_ECalABSuppSumEGated.mca","w"))==NULL)
  	{
  	  printf("ERROR!!! I cannot open the mca file!\n");
  	  exit(EXIT_FAILURE);
  	}
  
  for(ring=0;ring<NRING;ring++) 
    fwrite(hist[ring],S32K*sizeof(int),1,output);

  fclose(output);
}

  

