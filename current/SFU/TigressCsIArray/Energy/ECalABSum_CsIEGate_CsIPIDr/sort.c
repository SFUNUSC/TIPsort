#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  uint64_t one=1;
  int type;
  double eCsI1=0.;
  double eCsI2=0.;
  double eAddBack=0.;
  double chi,s,f,r;
  int take=0;

  //check if all the elements of the setup are present; discard if not
  if((data->h.setupHP&RF_BIT)==0) return SEPARATOR_DISCARD;
  if((data->h.setupHP&CsIArray_BIT)==0) return SEPARATOR_DISCARD;
  if((data->h.setupHP&TIGRESS_BIT)==0) return SEPARATOR_DISCARD;

  ev++;
  na=0;
  np=0;
  
  //work out number and type of particles identified in the event
  for(posCsI=1;posCsI<NCSI;posCsI++)
    if((data->csiarray.h.TSHP[posCsI/64]&(one<<posCsI%64))!=0)
      {
  	if((aGateLowFlag[posCsI]==1)&&(aGateHighFlag[posCsI])&&(pGateHighFlag[posCsI]==1))
  	  {
  	    type=data->csiarray.wfit[posCsI].type;
  	    chi=data->csiarray.wfit[posCsI].chisq;
  	    chi/=data->csiarray.wfit[posCsI].ndf;
  	    if((type>=idmin) && (type<=idmax))
  	      if((chi>=chimin) && (chi<=chimax))
  		{
  		  s=data->csiarray.wfit[posCsI].am[3];
  		  f=data->csiarray.wfit[posCsI].am[2];
  		  r=100+s/f*100;
		  
  		  if((aGateHigh[posCsI]<r) && (r<=pGateHigh[posCsI])) np++;
  		  if((aGateLow[posCsI]<=r) && (r<=aGateHigh[posCsI])) na++;
  		}
  	  }
  	else
  	  {
  	    printf("You didn't input all the proper limits!!!!\n");
  	    exit(EXIT_FAILURE);
  	  }
      }
  
  //what is this??
  //if((np<pMax) && (na<aMax)) 
  //  ch[np][na]++;
  
  sp=id[np][na];

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);

  eCsI1Flag=0;
  eCsI2Flag=0;

  /* work out number and type of particles identified in the good energy event */
  if(cev->csiarray.h.FE>0)
    for(posCsI1=1;posCsI1<NCSI;posCsI1++)
      if((cev->csiarray.h.EHP[posCsI1/64]&(one<<posCsI1%64))!=0)
	if(take==0)
  	{
	  //get CsI 1 energy
  	  eCsI1=cev->csiarray.csi[posCsI1].E/cal_par->csiarray.contr_e;
	  
  	  if(eCsI1<0)
  	    eCsI1=S32K-1000;
  	  if(eCsI1>S32K-10)
  	    eCsI1=S32K-2000;
	  
	  //if energy of the event is good and there is more than one particle, look in the remaining detectors
	  if(eCsI1>eCsILow)
	    if(eCsI1<eCsIHigh)
	      {
		eCsI1Flag=1;
		
		if((np+na)>1)
		  {
		    for(posCsI2=posCsI1+1;posCsI2<NCSI;posCsI2++)
		      if((cev->csiarray.h.EHP[posCsI2/64]&(one<<posCsI2%64))!=0)
			if(take==0)
			  {
			    //get CsI 2 energy
			    eCsI2=cev->csiarray.csi[posCsI2].E/cal_par->csiarray.contr_e;
			    
			    if(eCsI2<0)
			      eCsI2=S32K-1000;
			    if(eCsI2>S32K-10)
			      eCsI2=S32K-2000;
			    
			    if(eCsI2>eCsILow)
			      if(eCsI2<eCsIHigh)
				{
				  eCsI2Flag=1;
				  take=1;
				}
			  }
		  }
		
		if((np+na)==1)
		  if(eCsI1Flag==1)
		    {
		      // CsI histogram for the proper event type
		      histCsI[sp][(int)rint(eCsI1)]++;
		      
		      // Tigress histogram for the proper event type
		      if(cev->tg.h.FH>0)
			for(posTigr=1;posTigr<NPOSTIGR;posTigr++)
			  if(((cev->tg.h.HHP&(one<<(posTigr-1)))!=0) && (cev->tg.det[posTigr].hge.FE>0))
			    if((cev->tg.h.AHP&(1<<(posTigr-1)))!=0)
			      {
				eAddBack = cev->tg.det[posTigr].addback.E/cal_par->tg.contr_e;
				
				if(eAddBack<0 || eAddBack>S32K-10) 
				  eAddBack=S32K-10;
				
				histTigr[sp][(int)(eAddBack)]++;
				
			      }
		    }
		
		if((np+na)==2)
		  if(eCsI1Flag==1 && eCsI2Flag==1)
		    {
		      // CsI histogram for the proper event type
		      histCsI[sp][(int)rint(eCsI1)]++;
		      histCsI[sp][(int)rint(eCsI2)]++;
		      
		      // Tigress histogram for the proper event type
		      if(cev->tg.h.FH>0)
			for(posTigr=1;posTigr<NPOSTIGR;posTigr++)
			  if(((cev->tg.h.HHP&(one<<(posTigr-1)))!=0) && (cev->tg.det[posTigr].hge.FE>0))
			    if((cev->tg.h.AHP&(1<<(posTigr-1)))!=0)
			      {
				eAddBack = cev->tg.det[posTigr].addback.E/cal_par->tg.contr_e;
				
				if(eAddBack<0 || eAddBack>S32K-10) 
				  eAddBack=S32K-10;
				
				histTigr[sp][(int)rint(eAddBack)]++;
				
			      }
		    }
		
	      }
  	}
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{

  input_names_type *name;
  FILE *outCsI,*outTigr,*gateNameFile,*cluster;
  char aGateLowNum[132],aGateHighNum[132],pGateHighNum[132],det[132];
  char DataFile[132];

  
  if(argc!=8)
    {
      printf("CsIArray_ECalSum_CsIPIDr master_file_name idmin idmax chimin chimax eCsILow eCsIHigh \n");
      exit(-1);
    }
  
  printf("Program sorts summed particle gated ECal histograms for the CsIArray.\n");

  memset(ch,0,sizeof(ch));
  ev=0;
  memset(id,0,sizeof(id));
  id[0][0]=1;//random
  id[1][0]=2;//1p
  id[2][0]=3;//2p
  id[0][1]=4;//1a
  id[1][1]=5;//1a1p
  id[0][2]=6;//2a

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));

  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  memset(histCsI,0,sizeof(histCsI));
  memset(histTigr,0,sizeof(histTigr));


  read_master(argv[1],name);

  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  eCsILow=atof(argv[6]);
  eCsIHigh=atof(argv[7]);
  
   if(name->flag.cluster_file==1)
    {
      printf("Sorting calibrated energy histograms for TIGRESS clovers and cores based upon the cluster file: %s\n",name->fname.cluster_file);
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

  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("Tigress calibration read from %s.\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR!!! Tigress calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }  
  
  if(name->flag.gate_name_file==1)
    {
      printf("Using gate name file: %s\n",name->fname.gate_name_file);
      gateNameFile=fopen(name->fname.gate_name_file,"r");
      
      while(fscanf(gateNameFile,"%s %s %s %s",det,aGateLowNum,aGateHighNum,pGateHighNum)!=EOF)
	{
	  posCsI=atoi(det);
	  
	  aGateLow[posCsI] = atoi(aGateLowNum);
	  aGateLowFlag[posCsI]=1;
	  
	  aGateHigh[posCsI] = atoi(aGateHighNum);
	  aGateHighFlag[posCsI]=1;

	  pGateHigh[posCsI] = atoi(pGateHighNum);
	  pGateHighFlag[posCsI]=1;
	}
      fclose(gateNameFile);
    }
  else
    {
      printf("ERROR!!! Gate name file not defined!\n");
      exit(-1);
    }
  
  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting CsI energy data from file %s\n", name);
      sort(name);
    }
  
  if((outCsI=fopen("CsIEGate_CsIPIDr_CsIEnergy.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file\n");
      exit(EXIT_FAILURE);
    }

  //for(posCsI=1;posCsI<NCSI;posCsI++)
    for(np=0;np<NSP;np++)
      //fwrite(histCsI[posCsI][np],S32K*sizeof(int),1,outCsI);
      fwrite(histCsI[np],S32K*sizeof(int),1,outCsI);

  
  if((outTigr=fopen("CsIEGate_CsIPIDr_TigressEnergy.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file\n");
      exit(EXIT_FAILURE);
    }
  
  //for(posTigr=1;posTigr<NPOSTIGR;posTigr++)	
    //for(col=0;col<NCOL;col++) 
      for(np=0;np<NSP;np++)
	//	fwrite(histTigr[posTigr][col][sp],S32K*sizeof(int),1,outTigr);
  //fwrite(histTigr[posTigr][np],S32K*sizeof(int),1,outTigr);
	fwrite(histTigr[np],S32K*sizeof(int),1,outTigr);
  

    //Keep this commented for now, see ECalPosSum for position by
    //position CsI spectra.
/* for(pos=1;pos<NCSI;pos++) */
/*     { */
/*       //stop=0; // initialize detector counter */
/*       for(np=0;np<NSP;np++)  */
 /* 	//stop+=cal_par->tg.ceflag[pos][np]; //is there something for csi? */
/* 	//if(stop>0) //only write spectra if detector is present */
/* 	//	{ */
/* 	  sprintf(mcaFile,"pos%1d%1d_ECalSum_CsIPIDr.mca",pos/10,pos-(pos/10)*10); */
/* 	  if((out=fopen(mcaFile,"w"))==NULL) */
/* 	    { */
/* 	      printf("ERROR! I cannot open the mca file!\n"); */
/* 	      exit(EXIT_FAILURE); */
/* 	    } */
	  
/* 	  for(np=0;np<NSP;np++) */
/* 	    fwrite(hist[pos][np],S32K*sizeof(int),1,out); */
	  
/* 	  fclose(out); */
/* 	  //	} */
/*     } */
  
//fclose(outCsI);
fclose(outTigr);

}
