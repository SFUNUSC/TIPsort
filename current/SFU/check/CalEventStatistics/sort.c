#include "sort.h"

int analyze_data(raw_event *data)
{
	unsigned long long int one=1;
	
	cal_event* cev;
	cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
	
	//check the Ge fold
  if(cev->tg.h.FH>0)
	  //look through each Tigress position
	  for(pos=1;pos<NPOSTIGR;pos++)
      //check if the position is in the hit pattern
      if((cev->tg.h.HHP&(1<<(pos-1)))!=0) 
        //check the energy fold
        if(cev->tg.det[pos].hge.FH>0)
          //check if the position is in the addback hit pattern
          if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
            //Run through four cores for each position for suppression
            for(col=0;col<NCOL;col++)
              //Check if this color is indicated in the hit pattern
              if((cev->tg.det[pos].hge.HHP&(1<<col))!=0)
                //Check that this combination has a fold great than zero
                if(cev->tg.det[pos].ge[col].h.FH>0)
                  //Check if this combination is indicated in the hit pattern
                  if((cev->tg.det[pos].ge[col].h.HHP&1)!=0)
                  	{
                  		numEvents[pos][col]++;
											totalNumEvents++;
											ring=cev->tg.det[pos].ge[col].ring;
											if(ring>=0&&ring<NRING)
												ringNumEvents[ring]++;
                  	}

	//check the CsI fold
	if(cev->csiarray.h.FH>0)
		//look through each CsI position	
    for(csi=1;csi<NCSI;csi++)
      if((cev->csiarray.h.HHP&(one<<csi))!=0)
        {
        	numCsIEvents[csi]++;
					totalNumCsIEvents++;
					ring=cev->csiarray.ring[csi];
					if(ring>=0&&ring<NCSIRING)
						ringNumCsIEvents[ring]++;
        }
	
	free(cev);
  return SEPARATOR_DISCARD;
}
/*========================================================================*/
int main(int argc, char *argv[])
{
	FILE *cluster;
  input_names_type* name;
  char DataFile[132];
  
  if(argc!=2)
    {
      printf("check_CalEventStatistics master_file_name\n");
      printf("Program prints statistics for calibrated events in the SFU file(s) specified under input_data or cluster_file in the given master file.\n");
      exit(-1);
    }
    
  printf("Program prints statistics for calibrated events in the SFU file(s) specified under input_data or cluster_file in the given master file.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);

	
  
  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calibration read from %s.\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR!!! TIGRESS calibration parameters not defined!\n");
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
  
  if(name->flag.cluster_file==1)
    {
      printf("Sorting based on the cluster file: %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
				{
					printf("ERROR! I can't open input file %s\n",name->fname.cluster_file);
					exit(-2);
				}
			while(fscanf(cluster,"%s",DataFile) != EOF)
				{
				  memset(name,0,sizeof(input_names_type));
				  strcpy(name->fname.inp_data,DataFile);
				  
				  printf("Sorting gamma-ray energy data from file %s\n",name->fname.inp_data);
				  sort(name);
				}
		}
	else if(name->flag.inp_data==1)
    {
      printf("Sorting based on a single input file.\n");
      sort(name); 
    }
  else
    {
      printf("ERROR! Input data file or cluster file not defined\n");
      exit(-1);
    }
  	
  printf("\n");
  printf("========================\n");
  printf("SFU DATA FILE STATISTICS\n");
  printf("========================\n");
  printf("\n");
  
  printf("Total events containing TIGRESS hit(s): %Li\n",totalNumEvents);
  printf("Total events containing CsI hit(s): %Li\n",totalNumCsIEvents);
  
  //print TIGRESS core info
  printf("\nTIGRESS CORE INFO\n");
  if(totalNumEvents>0)
		{
			for(pos=1;pos<NPOSTIGR;pos++)
				for(col=0;col<NCOL;col++)
					if(numEvents[pos][col]>0)
						printf("Tigress #%i, core %i: %Li events (%f pct of events).\n",pos,col,numEvents[pos][col],(100.*numEvents[pos][col])/(totalNumEvents*1.0));
					else
						printf("No events in Tigress #%i, core %i.\n",pos,col);
		}
	else
		{
			printf("No events in the TIGRESS array.\n");
		}
  
  
  //print CsI detector info
  printf("\nCsI DETECTOR INFO\n");
  if(totalNumCsIEvents>0)
		{
			for(csi=1;csi<NCSI;csi++)
				if(numCsIEvents[csi]>0)
					printf("CsI #%i: %Li events (%f pct of events).\n",csi,numCsIEvents[csi],(100.*numCsIEvents[csi])/(totalNumCsIEvents*1.0));
				else
					printf("No events in CsI #%i.\n",csi);
		}
	else
		{
			printf("No events in the CsI array.\n");
		}
  
  //print TIGRESS ring info
  printf("\nTIGRESS RING INFO\n");
  if(totalNumEvents>0)
		{
			for(ring=1;ring<NRING;ring++)
				if(ringNumEvents[ring]>0)
					printf("Tigress ring #%i: %Li events (%f pct of events).\n",ring,ringNumEvents[ring],(100.*ringNumEvents[ring])/(totalNumEvents*1.0));
				else
					printf("No events in Tigress ring #%i.\n",ring);
  	}
  else
		{
			printf("No events in the TIGRESS array.\n");
		}
  
  //print CsI ring info
  printf("\nCsI RING INFO\n");
  if(totalNumCsIEvents>0)
		{
			for(ring=0;ring<NCSIRING;ring++)
				if(ringNumCsIEvents[ring]>0)
					printf("CsI ring #%i: %Li events (%f pct of events).\n",ring,ringNumCsIEvents[ring],(100.*ringNumCsIEvents[ring])/(totalNumCsIEvents*1.0));
				else
					printf("No events in CsI ring #%i.\n",ring);
		}
	else
		{
			printf("No events in the CsI array.\n");
		}
  
  printf("\n");
  free(cal_par);
  free(name);
  
}
