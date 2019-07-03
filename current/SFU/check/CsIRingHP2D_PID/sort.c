#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  uint64_t one=1;
  int pos1,pos2;
  bool allow;
  double s,f,r,e;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  

  if(cev->csiarray.h.FE>0)
    if(data->csiarray.h.TSfold==2)
      for(pos1=1;pos1<NCSI;pos1++)
        if((cev->csiarray.h.EHP[pos1/64]&(one<<pos1%64))!=0)
          for(pos2=pos1;pos2<NCSI;pos2++)
            if((cev->csiarray.h.EHP[pos2/64]&(one<<pos2%64))!=0)
              {
              	if((cev->csiarray.ring[pos1]>=0)&&(cev->csiarray.ring[pos1]<10))
              		if((cev->csiarray.ring[pos2]>=0)&&(cev->csiarray.ring[pos2]<10))
              			{

                      if(data->csiarray.wfit[pos1].type==1)
                        {
                          if(useCharge==1)
                            e=data->csiarray.csi[pos1].charge;
                          else
                            e=data->csiarray.wfit[pos1].am[1];
                          s=data->csiarray.wfit[pos1].am[3];
                          f=data->csiarray.wfit[pos1].am[2];
                        
                          if(f==0)
                            r=0;
                          else
                            r=s/f*100;
                        
                          r+=100;

                          if((pGateFlag[pos1]==1)&&(pGate[pos1]->IsInside(e,r)))
                            {
                              part_type[pos1] = 1; //proton
                            }
                          else if((aGateFlag[pos1]==1)&&(aGate[pos1]->IsInside(e,r)))
                            {
                              part_type[pos1] = 2; //alpha
                            }
                          else
                            {
                              printf("Unidentified particle! e = %f, r = %f\n",e,r);
                              printf("gateFlags = %i, %i\n",pGateFlag[pos1],aGateFlag[pos1]);
                              part_type[pos1] = 0;
                              getc(stdin);
                            }
                        }
                      if(data->csiarray.wfit[pos2].type==1)
                        {
                          if(useCharge==1)
                            e=data->csiarray.csi[pos2].charge;
                          else
                            e=data->csiarray.wfit[pos2].am[1];
                          s=data->csiarray.wfit[pos2].am[3];
                          f=data->csiarray.wfit[pos2].am[2];
                        
                          if(f==0)
                            r=0;
                          else
                            r=s/f*100;
                        
                          r+=100;

                          if((pGateFlag[pos2]==1)&&(pGate[pos2]->IsInside(e,r)))
                            {
                              part_type[pos2] = 1; //proton
                            }
                          else if((aGateFlag[pos2]==1)&&(aGate[pos2]->IsInside(e,r)))
                            {
                              part_type[pos2] = 2; //alpha
                            }
                          else
                            {
                              printf("Unidentified particle! e = %f, r = %f\n",e,r);
                              printf("gateFlags = %i, %i\n",pGateFlag[pos2],aGateFlag[pos2]);
                              part_type[pos2] = 0;
                              getc(stdin);
                            }
                        }

              				//if(cev->csiarray.ring[pos1]==0)
              				//	printf("Pos: %i, ring: %i\n",pos1,cev->csiarray.ring[pos1]);
						          if(pos2!=pos1)
						            {
                            if((part_type[pos1] == 1)&&(part_type[pos2] == 1)){
                              if(cev->csiarray.ring[pos1]<cev->csiarray.ring[pos2])
                                ringHPpp[cev->csiarray.ring[pos1]][cev->csiarray.ring[pos2]]++;
                              else
                                ringHPpp[cev->csiarray.ring[pos2]][cev->csiarray.ring[pos1]]++;
                            }else if((part_type[pos1] == 2)&&(part_type[pos2] == 2)){
                              if(cev->csiarray.ring[pos1]<cev->csiarray.ring[pos2])
                                ringHPaa[cev->csiarray.ring[pos1]][cev->csiarray.ring[pos2]]++;
                              else
                                ringHPaa[cev->csiarray.ring[pos2]][cev->csiarray.ring[pos1]]++;
                            }else if((part_type[pos1] == 1)&&(part_type[pos2] == 2)){
                              ringHPpa[cev->csiarray.ring[pos1]][cev->csiarray.ring[pos2]]++;
                            }else if((part_type[pos1] == 2)&&(part_type[pos2] == 1)){
                              ringHPpa[cev->csiarray.ring[pos2]][cev->csiarray.ring[pos1]]++;
                            }

						            }
						          else
						            {
						              //check that there are no events in all other detectors
						              //(ie. all events are in one) before adding to histogram
						              allow=true;
						              for(int i=1;i<NCSI;i++)
						                if(i!=pos1)
						                  if((cev->csiarray.h.EHP[i/64]&(one<<i%64))!=0)
						                    {
						                      allow=false;
						                      break;
						                    }
						              if(allow==true){
                            if(part_type[pos1] == 1)
                              ringHPpp[cev->csiarray.ring[pos1]][cev->csiarray.ring[pos2]]++;
                            else if(part_type[pos1] == 2)
                              ringHPaa[cev->csiarray.ring[pos1]][cev->csiarray.ring[pos2]]++;
                          }
						            }
				            }
              }

  free(cev);
  return SEPARATOR_DISCARD;
}
/*=======================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  FILE *gateNameFile;
  TFile* gateFile;
  FILE *cluster;
  char DataFile[132];
  char aGateName[132],pGateName[132],det[132];
  int csi;


  if(argc!=3)
    {
      printf("check_CsIRingHP2D_PID master_file useCharge\n");
      printf("Displays 2D hit pattern histogram summed over multiple runs for the CsI detector ring.\nThis code is PID-aware, and reports separate results for protons and alpha particles (use check_CsIRingHP2D for non PID-aware version).\n");     
      exit(-1);
    }
  
  printf("Program sorts 2D fold histogram.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);
  useCharge = atoi(argv[2]);

  if(name->flag.cluster_file!=1)
    {
      printf("\nCluster file not defined in master file!\n");
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

    
  if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
    {
      printf("ERROR!!! I can't open input file %s\n",name->fname.cluster_file);
      exit(-2);
    }

  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      //memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      sort(name);
    }
  
  printf("Proton-Proton\n");
  printf("ringhit1 ringhit2  N\n");
  for(int i=0;i<10;i++)
    for(int j=0;j<10;j++)
    	if(ringHPpp[i][j]>0)
      	printf("%i        %i        %li\n",i,j,ringHPpp[i][j]);
  printf("\n");

  printf("Alpha-Alpha\n");
  printf("ringhit1 ringhit2  N\n");
  for(int i=0;i<10;i++)
    for(int j=0;j<10;j++)
    	if(ringHPaa[i][j]>0)
      	printf("%i        %i        %li\n",i,j,ringHPaa[i][j]);
  printf("\n");

  printf("Proton-Alpha\n");
  printf("ringhit1 ringhit2  N\n");
  for(int i=0;i<10;i++)
    for(int j=0;j<10;j++)
    	if(ringHPpa[i][j]>0)
      	printf("%i        %i        %li\n",i,j,ringHPpa[i][j]);
  printf("\n");

  printf("If using CsI wall, some detectors in rings 3-5 all have higher detector index than ring 1-2 detectors, which is why some bins are omitted.\nRings are indexed according to the ring map parameter file used.\n");
}
