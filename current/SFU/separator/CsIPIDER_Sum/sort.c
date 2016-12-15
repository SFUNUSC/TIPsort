#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int np,na;
  double s,f,r,e;
  int type;
  double chisq;
  
  //if((data->h.setupHP&RF_BIT)==0) 
  //  return SEPARATOR_DISCARD;

  if((data->h.setupHP&CsIArray_BIT)==0) 
    return SEPARATOR_DISCARD;

  np=0;
  na=0;

  for(pos=1;pos<NCSI;pos++)
    if((data->csiarray.h.THP&(one<<pos))!=0)
      {
	type=data->csiarray.wfit[pos].type;
	if(type==1)
	  {
	    e=data->csiarray.csi[pos].charge;
	    //e=data->csiarray.wfit[pos].am[1];
	    s=data->csiarray.wfit[pos].am[3];
	    f=data->csiarray.wfit[pos].am[2];
	    
	    /* for(int i=1;i<=4;i++) */
	    /* printf("amp[%d] %Lf\n",i,data->csiarray.wfit[pos].am[i]); */
	    
	    if(f==0)
	      r=0;
	    else
	      r=s/f*100;
	    
	    r+=100;

	    /* //for "rough" PID from 209Po */
	    /* if(e>=0. && e<=2000.) */
	    /*   if(r>100. && r<=300.) */
	    /*     { */
	    /*     na++; */
	    /*     /\* printf("kept event e %10.3f r %10.3f\n",e,r); *\/ */
	    /*     } */
	    /* /\* getc(stdin); *\/ */
	    
	    if(pGateFlag[pos]==1)
	      if(pGate[pos]->IsInside(e,r))
		np++;
	    if(aGateFlag[pos]==1)
	      if(aGate[pos]->IsInside(e,r))
		na++;
	    
	    /* if(np>0 || na>0) */
	    /*   { */
	    /*     printf("pos %d\n",pos); */
	    /*     printf("e %10.3f r %10.3f\n",e,r); */
	    /*     printf("na %d np %d\n",na,np); */
	    /*     getc(stdin); */
	    /*   } */
	  }
      }
  
  if(np==gate_np)
    if(na==gate_na)
      {
	encode(data,output,enb);
	//printf("Event encoded with np %d na %d\n\nEND OF EVENT\n",np,na);
      }
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  FILE *cluster,*gateNameFile;
  TFile* f;
  char aGateName[132],pGateName[132],det[132],DataFile[132];

  if(argc!=4)
    {
      printf("\n ./separate_CsIArray_PID_ER_Sum master_file_name np na\n");
      printf("\n Master file should specify a cluster file containing .sfu filenames to sort, a gate file, and a gate names file.\n");
      exit(-1);
    }
 
  printf("Program separates events according to the specified CsI gates \n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));

  read_master(argv[1],name);
  gate_np=atoi(argv[2]);
  gate_na=atoi(argv[3]);

  if(name->flag.cluster_file!=1)
    {
      printf("\nInput cluster file not defined\n");
      exit(EXIT_FAILURE);
    }
  
 if(name->flag.out_data!=1)
    {
      printf("\nOutput data file not defined\n");
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
 
  if(name->flag.gate_name_file==1)
    {
      printf("Using gate name file: %s\n",name->fname.gate_name_file);

      if((gateNameFile=fopen(name->fname.gate_name_file,"r"))==NULL)
  	{
  	  printf("The gate file %s can not be opened\n",name->fname.gate_name_file);
  	  exit(1);
  	}

      while(fscanf(gateNameFile,"%s %s %s",det,aGateName,pGateName)!=EOF)
    	{
    	  pos=atoi(det);
    	  if(strcmp(aGateName,"null"))
    	    {
    	      aGate[pos] = (TCutG *) gROOT->FindObject(aGateName);
    	      aGateFlag[pos]=1;
    	    }
    	  if(strcmp(pGateName,"null"))
    	    {
    	      pGate[pos] = (TCutG *) gROOT->FindObject(pGateName);
    	      pGateFlag[pos]=1;
    	    }
    	}
      fclose(gateNameFile);
    }

  /*     //for 209Po only */
  /*     while(fscanf(gateNameFile,"%s %s",det,aGateName)!=EOF) */
  /* 	{ */
  /* 	  pos=atoi(det); */
  /* 	  if(strcmp(aGateName,"null")) */
  /* 	    { */
  /* 	      aGate[pos] = (TCutG *) gROOT->FindObject(aGateName); */
  /* 	      aGateFlag[pos]=1; */
  /* 	      printf("for position %d the gate flag is %d\n",pos,aGateFlag[pos]); */
  /* 	    } */
  /* 	} */
  /*     fclose(gateNameFile); */
  /*   } */
  /* else */
  /*   { */
  /*     printf("ERROR!!! Gate name file not defined!\n"); */
  /*     exit(-1); */
  /*   } */
  /* f->Close(); */

  for(pos=1;pos<NCSI;pos++)
    if((pGateFlag[pos]+aGateFlag[pos])!=0)
      {
  	printf("CsI %2d ",pos);
  	if(aGateFlag[pos]==1)
  	  printf("  alpha gate: %10s ",aGate[pos]->GetName());
  	else
  	  printf("%25s"," ");
  	if(pGateFlag[pos]==1)
  	  printf(" proton gate: %10s ",pGate[pos]->GetName());
  	else
  	  printf("%25s"," ");
  	printf("\n");
      }

  if((output=fopen(name->fname.out_data,"w"))==NULL)
    {
      printf("\nI can't open output file %s for writing\n",name->fname.out_data);
      exit(-2);
    }

  memset(enb,0,sizeof(enb));
  enb[0]=BUFFER_TAG;
  enb[1]++;
  enb[1]++;

  if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
    {
      printf("ERROR!!! I cannot open the cluster file!\n");
      exit(-2);
    }
  
  while(fscanf(cluster,"%s",DataFile)!=EOF)
    {
      printf("Reading data from file %s.\n",DataFile);
      strcpy(name->fname.inp_data,DataFile);
      sort(name);
    }
 
}

