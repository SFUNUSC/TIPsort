#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int type;
  double eAddBack=0;
  double chi,s,f,r;
  
  //check if all the elements of the setup are present; discard if not
  if((data->h.setupHP&RF_BIT)==0) return SEPARATOR_DISCARD;
  if((data->h.setupHP&CsIArray_BIT)==0) return SEPARATOR_DISCARD;
  if((data->h.setupHP&TIGRESS_BIT)==0) return SEPARATOR_DISCARD;
  
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
		}}
	else
	  {
	    printf("You didn't input all the proper limits!!!!\n");
	    exit(EXIT_FAILURE);
	  }}
  
  sp=id[np][na];
  
  /* now calibrate TIGRESS (earlier is just a waste of CPU) */
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);

  if(cev->tg.h.FH>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if(((cev->tg.h.HHP&(one<<(pos-1)))!=0) && (cev->tg.det[pos].hge.FE>0))
	if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
	  {
	    suppFlag = cev->tg.det[pos].suppress;
	    if(suppFlag==0)
	      {
		eAddBack = cev->tg.det[pos].addback.E/cal_par->tg.contr_e;
		ring=cev->tg.det[pos].addbackR;

		if(eAddBack<0 || eAddBack>S32K-10) eAddBack=S32K-10;
		hist[sp][ring][(int)rint(eAddBack)]++;
	      }}
  free(cev);
  return SEPARATOR_DISCARD;
}
/*=======================================================================*/
int main(int argc, char *argv[])
{
  input_names_type *name;
  FILE *gateNameFile,*cluster,*out;
  char aGateLowNum[132],aGateHighNum[132],pGateHighNum[132],det[132];
  char DataFile[132],fileName[132],evaporationChannel[132];

  if(argc!=6)
    {
      printf("Tigress_ECalABSuppRingSum_CsIPIDr master_file_name idmin idmax chimin chimax\n");
      exit(-1);
    }
  
  printf("Program generates Tigress ECalABSuppRing histograms separated on CsIPIDr.\n");
  
  memset(ch,0,sizeof(ch));
  ev=0;
  memset(id,0,sizeof(id));
  id[0][0]=1;//random
  id[1][0]=2;//1p
  id[2][0]=3;//2p
  id[0][1]=4;//1a
  id[1][1]=5;//1a1p
  id[0][2]=6;//2a
  //everything else goes to spectrum 0
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
      printf("Sorting ECalABSuppRingSum_PIDr energy histograms for TIGRESS cores based upon the cluster file: %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("ERROR! I can't open input file %s\n",name->fname.cluster_file);
	  exit(-2);
	}}
  else
    {
      printf("ERROR! Cluster file not defined\n");
      exit(-1);
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
      
      printf("Sorting data from %s\n", name);
      sort(name); 
    }
  
  for(int m=0;m<NSP;m++)
    {
      if(m==0) sprintf(evaporationChannel,"other");
      if(m==1) sprintf(evaporationChannel,"rand");
      if(m==2) sprintf(evaporationChannel,"1p");
      if(m==3) sprintf(evaporationChannel,"2p");
      if(m==4) sprintf(evaporationChannel,"1a");
      if(m==5) sprintf(evaporationChannel,"1a1p");
      if(m==6) sprintf(evaporationChannel,"2a");

      sprintf(fileName,"ECalABSuppRingSum_%s_Gated.mca",evaporationChannel);
      
      if((out=fopen(fileName,"w"))==NULL)
	{
	  printf("ERROR!!! I cannot open the mca file\n");
	  exit(EXIT_FAILURE);
	}
      
      for(int n=0;n<NRING;n++)
	fwrite(hist[m][n],S32K*sizeof(int),1,out);

      fclose(out);
    }}
