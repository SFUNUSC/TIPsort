#include "sort.h"
/*=========================================================================*/
int analyze_data(raw_event *data)
{
  cal_event* cev;
  double eAddBack=0;
  int suppFlag=0;
  int take=0;
  double ecsi,pval;
  int csi;
  unsigned long long int one=1;
  
  double* energy;
  int* ring;
  int i=0;//counter for number of hits processed
  int j=0;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  memset(beam_p,0,sizeof(beam_p));
  memset(part_p,0,sizeof(part_p));
  memset(gamma_dir,0,sizeof(gamma_dir));

  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  
  energy=(double*)calloc(cev->tg.h.FA,sizeof(double));
  ring=(int*)calloc(cev->tg.h.FA,sizeof(int));
  
  //get beam momentum value from calibration (energy specified in parameter file)
  beam_p[2]=sqrt( ((cal_par->csiarray.Ebeam + cal_par->csiarray.mproj)*(cal_par->csiarray.Ebeam + cal_par->csiarray.mproj)) - (cal_par->csiarray.mproj*cal_par->csiarray.mproj) ); //momentum of incoming beam, relativistic, from KE = mc^2 + m0c^2, mc^2 = sqrt(p^2c^2 + m0^2c^4)
  //printf("Beam p: %f\n",beam_p[2]);
  //getc(stdin);

  
  //check the Ge fold
  if(cev->tg.h.FA>0) //addback fold>0
	  //look through each Tigress position
	  for(pos=1;pos<NPOSTIGR;pos++)
      {
        //reset suppression flag for each position
        suppFlag=0;
        //check if the position is in the hit pattern
        if((cev->tg.h.HHP&(1<<(pos-1)))!=0) 
          //check the energy fold
          if(cev->tg.det[pos].hge.FH>0)
            //check if the position is in the addback hit pattern
            if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
              {
                /* set take=0 for each position for suppression 
                   of all possible clover hits in add-back mode*/
                take=0;
                //Run through four cores for each position for suppression
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
                colAddBack = cev->tg.det[pos].addbackC;
                
                if(eAddBack>=0 && eAddBack<S32K)
                  {
                    
                    if(cev->csiarray.h.FH>0)
                      for(csi=1;csi<NCSI;csi++)
                        if((cev->csiarray.h.HHP&(one<<csi))!=0)
                          {
                          	for(int ind=0;ind<3;ind++)
                              part_dir[csi][ind]=cal_par->csiarray.cpos_xyz[csi][ind];
                            vecMag=sqrt(part_dir[csi][0]*part_dir[csi][0] + part_dir[csi][1]*part_dir[csi][1] + part_dir[csi][2]*part_dir[csi][2]);
                            for(int ind=0;ind<3;ind++)
                            	part_dir[csi][ind]=part_dir[csi][ind]/vecMag;//make vector unit length
                            
                            ecsi=cev->csiarray.csi[csi].E/1000.; /* CsI energy in MeV */
                            pval=sqrt( ((ecsi+cal_par->csiarray.mp)*(ecsi+cal_par->csiarray.mp)) - (cal_par->csiarray.mp*cal_par->csiarray.mp) ); //relativistic p (magnitude)
                          	
                          	for(int ind=0;ind<3;ind++)
		                          part_p[csi][ind]=pval*part_dir[csi][ind];
		                        
		                        //printf("ecsi: %f MeV, pval: %f MeV/c, part_p: [%f %f %f] MeV/c\n",ecsi,pval,part_p[csi][0],part_p[csi][1],part_p[csi][2]);  getc(stdin);
                          }
              
                    //subtract detected particle momenta from beam momentum to get residual momentum
                    for(int ind=0;ind<3;ind++)
                      res_p[ind]=beam_p[ind];
                    for(csi=1;csi<NCSI;csi++)
                      for(int ind=0;ind<3;ind++)
                        res_p[ind]=res_p[ind]-part_p[csi][ind];
                    for(int ind=0;ind<3;ind++)
                      res_p[ind]=res_p[ind]*fudgeFactor;
                    
                    //calculate speed of residual
                    pval=sqrt(res_p[0]*res_p[0] + res_p[1]*res_p[1] + res_p[2]*res_p[2]);
                    beta=sqrt( 1.0 - ((cal_par->csiarray.mr*cal_par->csiarray.mr)/((pval*pval) + (cal_par->csiarray.mr*cal_par->csiarray.mr))) ); //relativistic calculation of beta
                    //printf("beta: %f\n",beta);
                    
                    //construct unit vectors for gamma and residual (source) nucleus
                    for(int ind=0;ind<3;ind++)
                      gamma_dir[ind]=cal_par->tg.tpos_xyz[pos][colAddBack][ind];
                    vecMag=sqrt(gamma_dir[0]*gamma_dir[0] + gamma_dir[1]*gamma_dir[1] + gamma_dir[2]*gamma_dir[2]);
                    for(int ind=0;ind<3;ind++)
                      gamma_dir[ind]=gamma_dir[ind]/vecMag;//make unit vector for gamma ray
                    vecMag=sqrt(res_p[0]*res_p[0] + res_p[1]*res_p[1] + res_p[2]*res_p[2]);
                    for(int ind=0;ind<3;ind++)
                      res_dir[ind]=res_p[ind]/vecMag;//make unit vector for residual nucleus
                    
                    //calculate ds
                    ds=sqrt(1-(beta*beta)) / (1-(beta* (res_dir[0]*gamma_dir[0] + res_dir[1]*gamma_dir[1] + res_dir[2]*gamma_dir[2]) ));
                    //ds=1+(beta* (res_dir[0]*gamma_dir[0] + res_dir[1]*gamma_dir[1] + res_dir[2]*gamma_dir[2]) );
                    
                    /*printf("ds: %f\n",ds);
                    printf("Tigress pos: %i\n",pos);
                    printf("beta: %f, gamma_dir: [%f %f %f]\n",beta,gamma_dir[0],gamma_dir[1],gamma_dir[2]);
                    printf("res_p: [%f %f %f] MeV/c\n",res_p[0],res_p[1],res_p[2]);
                    printf("res_pdir: [%f %f %f]\n",res_dir[0],res_dir[1],res_dir[2]);
                    printf("beam_p: [%f %f %f] MeV/c\n",beam_p[0],beam_p[1],beam_p[2]);
                    for(csi=1;csi<NCSI;csi++)
                      if((part_p[csi][0]+part_p[csi][1]+part_p[csi][2])!=0)
                        printf("In CsI # %i, part_p = [%f %f %f] MeV/c\n",csi,part_p[csi][0],part_p[csi][1],part_p[csi][2]);
                    getc(stdin);*/
                    
                    if(ds!=ds)
                    	{
                    		printf("ERROR: Doppler shift is undefined!\nHave you properly specified all detector postions in the parameter files?\n");
                    		printf("\nINFO DUMP\n---------\n");
												printf("ds: %f\n",ds);
												printf("Tigress pos: %i\n",pos);
												printf("beta: %f, gamma_dir: [%f %f %f]\n",beta,gamma_dir[0],gamma_dir[1],gamma_dir[2]);
												printf("res_p: [%f %f %f] MeV/c\n",res_p[0],res_p[1],res_p[2]);
												printf("res_pdir: [%f %f %f]\n",res_dir[0],res_dir[1],res_dir[2]);
												printf("beam_p: [%f %f %f] MeV/c\n",beam_p[0],beam_p[1],beam_p[2]);
												for(csi=1;csi<NCSI;csi++)
													if((part_p[csi][0]+part_p[csi][1]+part_p[csi][2])!=0)
														printf("In CsI # %i, part_p = [%f %f %f] MeV/c\n",csi,part_p[csi][0],part_p[csi][1],part_p[csi][2]);
												printf("\n");
                    		exit(-1);
                    	}
                    
                    //printf("spectrum: %i, ch: %i\n",0+suppFlag,(int)(eAddBack/ds));
                    eAddBack=eAddBack/ds;
                    
                    energy[i]=eAddBack;
                    ring[i] = cev->tg.det[pos].ge[colAddBack].ring+NRING*suppFlag;
                    
                    if(eAddBack>=0 && eAddBack<S32K)
                    	projhist[ring[i]][(int)(energy[i])]++;
                    
                    i++;//increment event counter
                    //h->Fill(ds);
                  }
                
              }
      }
      
  /*if(i!=cev->tg.h.FA)
  	{
    	printf("WARNING: Addback fold not equal to the number of events seen!\n");
    	printf("Addback fold = %i, number of events = %i\n",cev->tg.h.FA,i);
    }*/
  
  for(i=0;i<cev->tg.h.FA;i++)
    {
      //look for a gamma that falls into the gate
      if((energy[i]>=0)&&(energy[i]<S32K))
        if(energy[i]>=gateELow/cal_par->tg.contr_e)
	        if(energy[i]<=gateEHigh/cal_par->tg.contr_e)
	          if((ring[i]>0)&&(ring[i]<NRING))
              {
                //add all gammas in the event that aren't the gamma that fell into the gate
                for(j=0;j<cev->tg.h.FA;j++)
                	{
		                if(j!=i)
		                  if(energy[j]>=0)
		                    if(energy[j]<S32K)
		                      if(ring[j]>0)
		                        if(ring[j]<NRING)
		                          hist[ring[j]][(int)(energy[j])]++;
                  }
                gatehist[ring[i]][(int)(energy[i])]++;
                break;//don't double count
              }
    }
  
  free(cev);
  free(energy);  
  free(ring);
  return SEPARATOR_DISCARD;
}
/*=========================================================================*/
int main(int argc, char *argv[])
{

  //TCanvas *canvas;
  //TApplication *theApp;
  
  FILE *output, *cluster;
  input_names_type* name;
  char DataFile[132],FileName[132];
  double avgGateE;
  
  if((argc!=6)&&(argc!=7))
    {
      printf("TigressCsI_ECalABSuppDSReconstructedSumEGated master_file_name supLow supHigh gateELow gateEHigh fudge_factor\n");
      printf("Program attempts to generate energy gated gamma ray spectra with all events Doppler unshifted.  Energy gate values (gateEHigh, gateELow) should be specified in keV.\n");
      printf("Relies on beam momentum and velocity values specified in calibration parameters (deltaU.par).  Doesn't work for transitions with lifetimes long enough for the residual nucleus to slow.\n");
      printf("fudge_factor is a multiplicative factor which will be applied to the computed value of the residual nucleus momentum, in order to account for slowing of the beam/compound in the reaction target.  If left empty, a value of 1.0 will be used.\n");
      exit(-1);
    }
  
  //h = new TH1D("DSHistogram","DsHistogram",100,0.95,1.05);
  //h->Reset();
  
  printf("Program attempts to generate gamma ray spectra with all events Doppler unshifted.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  memset(hist,0,sizeof(hist));
  memset(gatehist,0,sizeof(gatehist));
  memset(projhist,0,sizeof(projhist));
  read_master(argv[1],name);
  
  supLow = atof(argv[2]);
  supHigh = atof(argv[3]);
  gateELow = atof(argv[4]);
  gateEHigh = atof(argv[5]);
  if(argc==7)
  	fudgeFactor = atof(argv[6]);
  else
  	fudgeFactor = 1.0;
  
  
  if(gateELow>gateEHigh)
  	{
  		printf("ERROR: cannot have lower gate bound larger than upper gate bound.\n");
  		exit(-1);
  	}
  	
  avgGateE=(gateELow+gateEHigh)/2.;
  
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
  
  //print info
  printf("\n");
	printf("  Beam energy: %f MeV\n",cal_par->csiarray.Ebeam);
	printf("  Projectile mass: %f MeV/c^2\n",cal_par->csiarray.mproj);
	printf("  Particle mass: %f MeV/c^2\n",cal_par->csiarray.mp);
	printf("  Residual mass: %f MeV/c^2\n",cal_par->csiarray.mr);
  if(fudgeFactor>1.0)
  	printf("  WARNING: using fudge factor greater than 1.  This implies that the beam/compound sppeds up in the target, which should not be possible.\n");
  else
  	printf("  Using fudge factor of %f\n",fudgeFactor);
  printf("  Using gamma energy gate between %f and %f keV.\n",gateELow,gateEHigh);
  printf("\n");
  
  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting data from file %s\n", name->fname.inp_data);
      sort(name);
    }
  
  printf("\n");
  
  sprintf(FileName,"DS_ECalABSuppReconstructed_%.0fgated.mca",avgGateE);
  if((output=fopen(FileName,"w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(hist,2*NRING*S32K*sizeof(int),1,output);
  fclose(output);
  printf("Gated spectrum saved to file: %s\n",FileName);
  
  sprintf(FileName,"DS_ECalABSuppReconstructed_%.0fgate.mca",avgGateE);
  if((output=fopen(FileName,"w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(gatehist,2*NRING*S32K*sizeof(int),1,output);
  fclose(output);
  printf("Gate spectrum saved to file: %s\n",FileName);
  
  sprintf(FileName,"DS_ECalABSuppReconstructed_%.0fproj.mca",avgGateE);
  if((output=fopen(FileName,"w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(projhist,2*NRING*S32K*sizeof(int),1,output);
  fclose(output);
   printf("Projection spectrum saved to file: %s\n",FileName);
  
  /*theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("DS","DS",10,10, 500, 300);
  gPad->SetLogy(1);
  h->Draw();
  theApp->Run(kTRUE);*/
  
}
