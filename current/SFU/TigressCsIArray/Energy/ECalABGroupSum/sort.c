#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos,col,group,csi;
  double etig;

  /* double rand = randGen->Uniform(); // random (0,1) for dropping events */
  /* double frac = 0.01; // fraction of events to keep */
  
 if((data->h.setupHP&TIGRESS_BIT)==0)
   return SEPARATOR_DISCARD;
 
 if((data->h.setupHP&RF_BIT)==0)
   return SEPARATOR_DISCARD;
 
 if((data->h.setupHP&CsIArray_BIT)==0)
   return SEPARATOR_DISCARD;
 
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);

  /* if(rand < frac) */
  /*   { */
      /* Require good time and energy for the event */
      if(cev->tg.h.FA>0)
	if(cev->csiarray.h.FH>0)
	  for(pos=1;pos<NPOSTIGR;pos++)
	    if(((cev->tg.h.HHP&(1<<(pos-1)))!=0) && (cev->tg.det[pos].hge.FE>0))
	      if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
		{	
		  etig = cev->tg.det[pos].addback.E/cal_par->tg.contr_e;
		  col = cev->tg.det[pos].addbackC;
		  
		  for(csi=1;csi<NCSI;csi++)
		    if((cev->csiarray.h.HHP[csi/64]&(one<<csi%64))!=0)
		      {
			if(use_gates==1)
			  {
			    // check PID
			    double e = cev->csiarray.PIDe[csi];
			    double r = cev->csiarray.PIDr[csi];
			    // if a recoil, sort to group histogram
			    if(rGateFlag[csi]==1)
			      if(rGate[csi]->IsInside(e,r))
				{
				  group = cal_par->tg.group_map[pos][col][csi];
				  if(etig<0 || etig>S32K-10)
				    etig=S32K-10;
				  hist[group][(int)etig]++;
				  /* printf("hist[%d] pos %d col %d csi %d group %d energy %lf\n",group,pos,col,csi,group,etig); */
				}
			  }
			else
			  {
			    group = cal_par->tg.group_map[pos][col][csi];
			    if(etig<0 || etig>S32K-10)
			      etig=S32K-10;
			    hist[group][(int)etig]++;
			    /* printf("hist[%d] pos %d col %d csi %d group %d energy %lf\n",group,pos,col,csi,group,etig); */
			  }
		      }
		}
    /* } */
  /* printf("---- End of event ----\n"); */
  /* getc(stdin); */		   
  free(cev);
  return SEPARATOR_DISCARD;
}
/*=============================================================================*/
int main(int argc, char *argv[])
{
  FILE *output;
  input_names_type* name;
  FILE *cluster;
  char DataFile[132];
  randGen = new TRandom3();

  // for PID
  FILE *gateNameFile;
  char rGateName[132],det[132];
  TFile* f;
  int pos;
 
  if(argc!=3)
    {
      printf("Tigress_ECalABGroup master_file_name use_gates\n");
      exit(-1);
    }

  use_gates=atoi(argv[2]);
  
  printf("Program sorts ECalABGroup histograms for TIGRESS using PID gates if specified.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));  
  memset(hist,0,sizeof(hist));

  read_master(argv[1],name);
  
  if(name->flag.cluster_file==1)
    {
      printf("Sorting ECalABSuppRingSum_PIDr energy histograms for TIGRESS cores based upon the cluster file: %s\n",name->fname.cluster_file);
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

   if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calibration read from %s.\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  if(name->flag.CSIARRAY_cal_par==1)
    {
      printf("CsIArray calpar read from: %s\n",name->fname.CSIARRAY_cal_par);
      initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
    }
  else
    {
      printf("ERROR!!! TIGRESS calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }

  if(name->flag.root_gate_file==1)
    {
      printf("Using root gate file: %s\n",name->fname.root_gate_file);
      f=new TFile(name->fname.root_gate_file,"READ");
    }
  else
    {
      printf("ERROR!!! Root gate file not defined!\n");
      exit(-1);
    }
  
  // read gate names
  // p for protons, a for alphas, r for recoils, h for heavy recoils 
  if(name->flag.gate_name_file==1)
    {
      printf("Using gate name file: %s\n",name->fname.gate_name_file);
      
      if((gateNameFile=fopen(name->fname.gate_name_file,"r"))==NULL)
  	{
  	  printf("The gate file %s can not be opened\n",name->fname.gate_name_file);
  	  exit(1);
  	}
      
      while(fscanf(gateNameFile,"%s %s",det,rGateName)!=EOF)
    	{
    	  pos=atoi(det);
	  if(strcmp(rGateName,"null"))
    	    {
    	      rGate[pos] = (TCutG *) gROOT->FindObject(rGateName);
    	      rGateFlag[pos]=1;
    	    }
	}
      fclose(gateNameFile);
    }
  // done reading gate names
  
  // print gate list
  for(pos=1;pos<NCSI;pos++)
    if((rGateFlag[pos])!=0)
      {
  	printf("CsI %2d ",pos);
  	if(rGateFlag[pos]==1)
  	  printf("  recoil gate: %3s ",rGate[pos]->GetName());
  	else
  	  printf("%5s"," ");
	printf("\n");
      }
  // done printing gate list
  
  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting data from %s\n",name);
      sort(name);
    }

  if((output=fopen("Group_ECalAB.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  
  fwrite(hist,NGROUP*S32K*sizeof(int),1,output);
  fclose(output);
  return 0;
}
