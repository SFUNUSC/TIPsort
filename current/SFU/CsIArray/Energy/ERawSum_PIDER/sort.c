#include "sort.h"

int analyze_data(raw_event *data)
{
  uint64_t one=1;
  double s,f,r,e;
  int csi,type;

  //printf("Fold: %i,type: %i\n",data->csiarray.h.Efold,data->csiarray.wfit[csi].type);

  //get the particle types from PID-ER
  if(data->csiarray.h.Efold>0)
    for(csi=1;csi<NCSI;csi++)
      if((data->csiarray.h.EHP[csi/64]&(one<<csi%64))!=0) {

        //printf("csi = %i\n",csi);
        
        type=data->csiarray.wfit[csi].type;
        if(type==1)
          {
            if(useCharge==1)
              e=data->csiarray.csi[csi].charge;
            else
              e=data->csiarray.wfit[csi].am[1];
            s=data->csiarray.wfit[csi].am[3];
            f=data->csiarray.wfit[csi].am[2];
          
            if(f==0)
              r=0;
            else
              r=s/f*100;
          
            r+=100;

            if((pGateFlag[csi]==1)&&(pGate[csi]->IsInside(e,r)))
              {
                part_type[csi] = 1; //proton
              }
            else if((aGateFlag[csi]==1)&&(aGate[csi]->IsInside(e,r)))
              {
                part_type[csi] = 2; //alpha
              }
            else
              {
                printf("Unidentified particle! e = %f, r = %f\n",e,r);
                printf("gateFlags = %i, %i\n",pGateFlag[csi],aGateFlag[csi]);
                part_type[csi] = 0;
                getc(stdin);
              }
            
          }
      }

  if(data->csiarray.h.Efold>0)
    for(csi=1;csi<NCSI;csi++)
      if((data->csiarray.h.EHP[csi/64]&(one<<csi%64))!=0)
        {
          e=data->csiarray.csi[csi].charge;
          if((e>=0) && (e<S32K))
            hist[csi + (part_type[csi]-1)*NCSI][(int)e]++;
        }
  return SEPARATOR_DISCARD;
}
/*==========================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *cluster;
  FILE *gateNameFile;
  TFile* gateFile;
  input_names_type* name;
  char DataFile[132];
  char aGateName[132],pGateName[132],det[132];
  int csi;

  if(argc!=3)
    {
      printf("CsIArray_ERaw master_file_name useCharge\n");
      exit(-1);
    }
  
  printf("Program sorts summed ERaw histograms for the CsIArray from a cluster file.\nA cluster file is specified as the parameter, not a master file.\n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);

  useCharge = atoi(argv[2]);

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

  if(name->flag.root_gate_file==1)
    {
      printf("Using root gate file: %s\n",name->fname.root_gate_file);
      gateFile=new TFile(name->fname.root_gate_file,"READ");
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
    	  csi=atoi(det);
    	  if(strcmp(aGateName,"null"))
    	    {
    	      aGate[csi] = (TCutG *) gROOT->FindObject(aGateName);
    	      aGateFlag[csi]=1;
    	    }
    	  if(strcmp(pGateName,"null"))
    	    {
    	      pGate[csi] = (TCutG *) gROOT->FindObject(pGateName);
    	      pGateFlag[csi]=1;
    	    }
    	}
      fclose(gateNameFile);
    }

  for(csi=1;csi<NCSI;csi++)
    if((pGateFlag[csi]+aGateFlag[csi])!=0)
      {
      	printf("CsI %2d ",csi);
      	if(aGateFlag[csi]==1)
      	  printf("  alpha gate: %10s ",aGate[csi]->GetName());
      	else
      	  printf("%25s"," ");
      	if(pGateFlag[csi]==1)
      	  printf(" proton gate: %10s ",pGate[csi]->GetName());
      	else
      	  printf("%25s"," ");
      	printf("\n");
      }

  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting data from file %s\n", name->fname.inp_data);
      sort(name);
    }
  
  printf("\n");    


  
  if((output=fopen("CsIArray_ERawSum_PIDER.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  for(int csi=0;csi<NCSI*2;csi++) 
    fwrite(hist[csi],S32K*sizeof(int),1,output);
  fclose(output);

  printf("Data written to file: CsIArray_ERawSum_PIDER.mca\nProton gated spectra from spectrum 1 to %i.  Alpha gated spectra from spctrum %i to %i.\n",NCSI-1,NCSI+1,(2*NCSI - 1));
}
