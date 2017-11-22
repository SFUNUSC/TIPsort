#include "sort.h"

int analyze_data(raw_event *data)
{
  int64_t one=1,none=-1,kill;
  int np,na;
  double s,f,r,e;
  int type;
  
  int64_t flag_csi[4];
  
  //if((data->h.setupHP&RF_BIT)==0) 
  //  return SEPARATOR_DISCARD;

  if((data->h.setupHP&CsIArray_BIT)==0) 
    return SEPARATOR_DISCARD;

	memset(flag_csi,0,sizeof(flag_csi));
	
  np=0;
  na=0;

  for(pos=1;pos<NCSI;pos++)
    if((data->csiarray.h.THP[pos/64]&(one<<pos%64))!=0)
      {
				type=data->csiarray.wfit[pos].type;
				if(type==1)
					{
						if(useCharge==1)
							e=data->csiarray.csi[pos].charge;
						else
							e=data->csiarray.wfit[pos].am[1];
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
						//printf("e %10.3f r %10.3f\n",e,r);
						//getc(stdin);

						if((pGateFlag[pos]==1)&&(pGate[pos]->IsInside(e,r)))
							{
								np++;
								flag_csi[pos/64]|=(one<<pos%64); //flag the hit for preservation
							}
						else if((aGateFlag[pos]==1)&&(aGate[pos]->IsInside(e,r)))
							{
								na++;
								flag_csi[pos/64]|=(one<<pos%64); //flag the hit for preservation
							}
					
						/* if(np>0 || na>0) */
						/*   { */
						/*     printf("pos %d\n",pos); */
						/*     printf("e %10.3f r %10.3f\n",e,r); */
						/*     printf("na %d np %d\n",na,np); */
						/*     getc(stdin); */
						/*   } */
					}
      }

  //printf("Event with np %d na %d\n\nEND OF EVENT\n",np,na);  
  //getc(stdin);
  
	if(np==gate_np)
		if(na==gate_na)
			{
				//drop csi without that aren't flagged for preservation
				//(ie. csi that aren't protons OR alphas)
				for(pos=1;pos<NCSI;pos++)
					if((data->csiarray.h.TSHP[pos/64]&(one<<pos%64))!=0)
						if((flag_csi[pos/64]&(one<<pos%64))==0)
							{
								memset(&data->csiarray.csi[pos],0,sizeof(channel));
								memset(&data->csiarray.wfit[pos],0,sizeof(ShapePar));
								memset(&data->csiarray.t0[pos],0,sizeof(double));
								data->csiarray.h.Efold--;
								data->csiarray.h.Tfold--;	  
								data->csiarray.h.TSfold--;
								kill=none-(one<<pos%64);
								data->csiarray.h.TSHP[pos/64]&=kill;
								data->csiarray.h.EHP[pos/64]&=kill;
								data->csiarray.h.THP[pos/64]&=kill;
							}
				if(data->csiarray.h.TSfold<=0)
					{
						kill=none-CsIArray_BIT; 
						data->h.setupHP&=kill;
						memset(&data->csiarray,0,sizeof(CsIArray));
					}
    
				encode(data,output,enb);
				//printf("Event encoded with np %d na %d\n\nEND OF EVENT\n",np,na);
			}
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  FILE *gateNameFile;
  TFile* f;
  char aGateName[132],pGateName[132],det[132];
  useCharge=0;

  if((argc!=4)&&(argc!=5))
    {
      printf("\n ./separate_CsIArray_PID_ER master_file_name np na useCharge\n");
      printf("\nuseCharge is an optional parameter, set to 1 if charge is to be used instead of fitted amplitude, otherwise leave blank.\n");
      exit(-1);
    }
 
  printf("Program separates events according to the specified CsI gates \n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));

  read_master(argv[1],name);
  gate_np=atoi(argv[2]);
  gate_na=atoi(argv[3]);
  if(argc>4)
  	useCharge=atoi(argv[4]);
  
  if(useCharge==1)
  	printf("Using charge rather than amplitude for PID separation.\n");

  if(name->flag.inp_data!=1)
    {
      printf("\nInput data file not defined\n");
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

  sort(name);
  
	//save the last buffer which will be dropped otherwise
	//if enb[1]==2 then the buffer contains no data, only the header
	if(enb[1]>2)
		fwrite(enb,sizeof(int),BUFFSIZE,output);
 
}

