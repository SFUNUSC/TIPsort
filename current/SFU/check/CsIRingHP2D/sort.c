#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos1,pos2;
  bool allow;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  

  if(cev->csiarray.h.FE>0)
    if(data->csiarray.h.TSfold==2)
      for(pos1=1;pos1<NCSI;pos1++)
        if((cev->csiarray.h.EHP&(one<<pos1))!=0)
          for(pos2=pos1;pos2<NCSI;pos2++)
            if((cev->csiarray.h.EHP&(one<<pos2))!=0)
              {
              	if((cev->csiarray.ring[pos1]>=0)&&(cev->csiarray.ring[pos1]<10))
              		if((cev->csiarray.ring[pos2]>=0)&&(cev->csiarray.ring[pos2]<10))
              			{
              				//if(cev->csiarray.ring[pos1]==0)
              				//	printf("Pos: %i, ring: %i\n",pos1,cev->csiarray.ring[pos1]);
						          if(pos2!=pos1)
						            {
						              if(cev->csiarray.ring[pos1]<cev->csiarray.ring[pos2])
						                ringHP[cev->csiarray.ring[pos1]][cev->csiarray.ring[pos2]]++;
						              else
						                ringHP[cev->csiarray.ring[pos2]][cev->csiarray.ring[pos1]]++;
						            }
						          else
						            {
						              //check that there are no events in all other detectors
						              //(ie. all events are in one) before adding to histogram
						              allow=true;
						              for(int i=1;i<NCSI;i++)
						                if(i!=pos1)
						                  if((cev->csiarray.h.EHP&(one<<i))!=0)
						                    {
						                      allow=false;
						                      break;
						                    }
						              if(allow==true)
						                ringHP[cev->csiarray.ring[pos1]][cev->csiarray.ring[pos2]]++;
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
  FILE *cluster;
  char DataFile[132];


  if(argc!=2)
    {
      printf("check_CsIRingHP2D master_file\n");
      printf("Displays 2D hit pattern histogram summed over multiple runs for the CsI detector ring.\n");     
      exit(-1);
    }
  
  printf("Program sorts 2D fold histogram.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);

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
  
  
  printf("ringhit1 ringhit2  N\n");
  for(int i=0;i<10;i++)
    for(int j=i;j<10;j++)
    	if(ringHP[i][j]>0)
      	printf("%i        %i        %li\n",i,j,ringHP[i][j]);
  printf("If using CsI wall, some detectors in rings 3-5 all have higher detector index than ring 1-2 detectors, which is why some bins are omitted.\nRings are indexed according to the ring map parameter file used.\n");
}
