#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int type;
  double e=0;
  double chi,s,f,r;

//check if all the elements of the setup are present; discard if not
  if((data->h.setupHP&RF_BIT)==0) return SEPARATOR_DISCARD;
  if((data->h.setupHP&CsIArray_BIT)==0) return SEPARATOR_DISCARD;

  ev++;
  na=0;
  np=0;
  
  /* work out number and type of particles identified in the event */
  for(pos=1;pos<NCSI;pos++)
    if((data->csiarray.h.TSHP&(one<<pos))!=0)
      {
	if((aGateLowFlag[pos]==1)&&(aGateHighFlag[pos])&&(pGateHighFlag[pos]==1))
	  {
	    type=data->csiarray.wfit[pos].type;
	    chi=data->csiarray.wfit[pos].chisq;
	    chi/=data->csiarray.wfit[pos].ndf;
	    if((type>=idmin) && (type<=idmax))
	      if((chi>=chimin) && (chi<=chimax))
		{
		  s=data->csiarray.wfit[pos].am[3];
		  f=data->csiarray.wfit[pos].am[2];
		  r=100+s/f*100;
		  
		  if((aGateHigh[pos]<r) && (r<=pGateHigh[pos])) np++;
		  if((aGateLow[pos]<=r) && (r<=aGateHigh[pos])) na++;
		}
	  }
	else
	  {
	    printf("You didn't input all the proper limits!!!!\n");
	    exit(EXIT_FAILURE);
	  }
      }
  
  //what is this doing here? why does ch[np][na] even exist? its taking memory for no reason!
  if((np<pMax) && (na<aMax)) 
    ch[np][na]++;
  
  sp=id[np][na];
  
  /* now calibrate CsI (earlier is just a waste of CPU) */

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);

  //printf("loop = %d\n",cev->csiarray.h.FE);
  //  getc(stdin);

  if(cev->csiarray.h.FE>0)
    for(pos=1;pos<NCSI;pos++)
      if((cev->csiarray.h.EHP&(one<<pos))!=0)
	{
	  e=cev->csiarray.csi[pos].E/cal_par->csiarray.contr_e;
	  
	  if(e<0)
	    e=S32K-1000;
	  if(e>S32K-10) 
	    e=S32K-2000;
	  
	  //hist[sp][(int)rint(e)]++;
	  hist[pos][sp][(int)rint(e)]++;
	}


  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{

  input_names_type *name;
  FILE *out,*gateNameFile,*cluster;
  char aGateLowNum[132],aGateHighNum[132],pGateHighNum[132],det[132];
  char mcaFile[132],DataFile[132];

  
  if(argc!=6)
    {
      printf("CsIArray_ECalSum_CsIPIDr master_file_name idmin idmax chimin chimax\n");
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
  
  memset(hist,0,sizeof(hist));

  read_master(argv[1],name);

  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  
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
  
  
  if(name->flag.gate_name_file==1)
    {
      printf("Using gate name file: %s\n",name->fname.gate_name_file);
      gateNameFile=fopen(name->fname.gate_name_file,"r");
      
      while(fscanf(gateNameFile,"%s %s %s %s",det,aGateLowNum,aGateHighNum,pGateHighNum)!=EOF)
	{
	  pos=atoi(det);
	  
	  aGateLow[pos] = atoi(aGateLowNum);
	  aGateLowFlag[pos]=1;
	  
	  aGateHigh[pos] = atoi(aGateHighNum);
	  aGateHighFlag[pos]=1;

	  pGateHigh[pos] = atoi(pGateHighNum);
	  pGateHighFlag[pos]=1;
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
  
  /* if((out=fopen("CsIArray_ECal_CsIPIDr.mca","w"))==NULL) */
  /*   { */
  /*     printf("ERROR!!! I cannot open the mca file\n"); */
  /*     exit(EXIT_FAILURE); */
  /*   } */
	  
  //  for(np=0;np<NSP;np++)
  //fwrite(hist[np],S32K*sizeof(int),1,out);

for(pos=1;pos<NCSI;pos++)
    {
      //stop=0; //what is this
      for(np=0;np<NSP;np++) 
	//stop+=cal_par->tg.ceflag[pos][np]; //again, what is this? what is tg.ceflag?? tigress calibrated event?
	//if(stop>0) //what is the point of this loop? only writes to file if some condition is met based on tg.ceflag.
	//	{ i think if this is changed to some csi.ceflag it will only write if the csi detector is present
	  sprintf(mcaFile,"pos%1d%1d_ECalSum_CsIPIDr.mca",pos/10,pos-(pos/10)*10);
	  if((out=fopen(mcaFile,"w"))==NULL)
	    {
	      printf("ERROR! I cannot open the mca file!\n");
	      exit(EXIT_FAILURE);
	    }
	  
	  for(np=0;np<NSP;np++)
	    fwrite(hist[pos][np],S32K*sizeof(int),1,out);
	  
	  fclose(out);
	  //	}
    }
  
//fclose(out);
}
