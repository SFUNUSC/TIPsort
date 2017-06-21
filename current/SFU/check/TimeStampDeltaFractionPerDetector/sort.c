#include "sort.h"

int analyze_data(raw_event *data)
{
	unsigned long long int one=1;
	unsigned long long int min,max;
	int diff = verify_ts(data,&min,&max);
	
	//look through each Tigress position
	for(pos=1;pos<NPOSTIGR;pos++)
		//check if the position is in the hit pattern
		if((data->tg.h.GeHP&(1<<(pos-1)))!=0)
			//Run through four cores for each position
			for(col=0;col<NCOL;col++)
				//check if the position and color are in the hit pattern
				if((data->tg.det[pos].h.GeHP&(1<<col))!=0)
					{
						if(diff>delta)
							numBadEvents[pos][col]++;
						numEvents[pos][col]++;
					}
	
	//look through each CsI position				
	for(csi=1;csi<NCSI;csi++)
		//check if the position is in the hit pattern
		if((data->csiarray.h.THP&(one<<csi))!=0)
			{
				if(diff>delta)
					numBadCsIEvents[csi]++;
				numCsIEvents[csi]++;
			}
	
  return SEPARATOR_DISCARD;
}
/*========================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  
  if(argc!=3)
    {
      printf("check_TimeStampDeltaFractionPerDetector sfu_input_data delta\n");
      printf("Program checks the number of events containing a hit for a given detector with time stamp inconsistency larger than delta.\n");
      exit(-1);
    }
  
  delta=atoi(argv[2]);
  printf("Program checks the number of events containing a hit for a given detector with time stamp inconsistency larger than delta.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);

	/*cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));*/

  sort(name); 
  
  
  for(pos=1;pos<NPOSTIGR;pos++)
  	for(col=0;col<NCOL;col++)
  		if(numEvents[pos][col]>0)
  			printf("Tigress #%i, core %i: %f pct inconsistent events (%Li / %Li events).\n",pos,col,(100.*numBadEvents[pos][col])/(numEvents[pos][col]*1.0),numBadEvents[pos][col],numEvents[pos][col]);
  		else
  			printf("No events in Tigress #%i, core %i.\n",pos,col);
	for(csi=1;csi<NCSI;csi++)
		if(numCsIEvents[csi]>0)
  		printf("CsI #%i: %f pct inconsistent events (%Li / %Li events).\n",csi,(100.*numBadCsIEvents[csi])/(numCsIEvents[csi]*1.0),numBadCsIEvents[csi],numCsIEvents[csi]);
  	else
  		printf("No events in CsI #%i.\n",csi);
}
