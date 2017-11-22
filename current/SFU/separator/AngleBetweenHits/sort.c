#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  uint64_t one=1;
  int pos1;
  
  
  //if((data->h.setupHP&RF_BIT)==0) 
  //  return SEPARATOR_DISCARD;

  //if((data->h.setupHP&CsIArray_BIT)==0) 
  //  return SEPARATOR_DISCARD;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  
  
  int csiHitIndex=0;
	double angle=0.;
  if(cev->csiarray.h.FE>0)
    for(pos1=1;pos1<NCSI;pos1++)
      if((cev->csiarray.h.EHP[pos1/64]&(one<<pos1%64))!=0)
				{
					//printf("Hit found...\n");
					if(csiHitIndex<2)
						{
							for(int ind=0;ind<3;ind++)
								res_dir[csiHitIndex][ind]=cal_par->csiarray.cpos_xyz[pos1][ind];
							csiHitIndex++;
						}
				}

	//get the angle between the hits
	if(csiHitIndex>=2) //are we able to get an angle (have 2 hits?)
		{
			//printf("Calculating angle...\n");
			for(int i=0;i<2;i++)
				res_dir_mag[i]=sqrt(res_dir[i][0]*res_dir[i][0] + res_dir[i][1]*res_dir[i][1] + res_dir[i][2]*res_dir[i][2]);
			for(int i=0;i<3;i++)
				angle += res_dir[0][i] * res_dir[1][i]; //dot product
			angle = angle/(res_dir_mag[0]*res_dir_mag[1]); //normalization
			//angle = acos(angle);
			//angle *= 180./3.14159265359; //convert to degrees
			//printf("Angle between hits: %f\n",angle);
			//printf("Angle: %f, minAngle: %f, maxAngle: %f\n",angle,minAngle,maxAngle);
			if(angle>=minAngle)
				if(angle<=maxAngle)
					{
						//printf("Keeping event with angle: %f\n",angle);
						encode(data,output,enb);
					}
		}
  free(cev);

  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;

  if(argc!=4)
    {
      printf("\n ./separate_AngleBetweenHits master_file_name minAngle maxAngle\n");
      exit(-1);
    }
 
  printf("Program separates events according to the angle between hits in the CsIArray.\n");
  printf("minAngle, maxAngle are in units of cos(theta).\n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

  read_master(argv[1],name);
  minAngle=atof(argv[2]);
  maxAngle=atof(argv[3]);

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

  if((output=fopen(name->fname.out_data,"w"))==NULL)
    {
      printf("\nI can't open output file %s for writing\n",name->fname.out_data);
      exit(-2);
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

