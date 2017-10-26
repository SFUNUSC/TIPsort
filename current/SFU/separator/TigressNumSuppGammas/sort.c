#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  double eAddBack=-1.;
  int pos,col;
  int suppFlag=0;
  int take=0;
  int numSuppG=0;
  

  if((data->h.setupHP&TIGRESS_BIT)==0)
    return SEPARATOR_DISCARD;

  //if((data->h.setupHP&RF_BIT)==0)
  //  return SEPARATOR_DISCARD;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);

  //check the Ge fold
  if(cev->tg.h.FA>0)
		//look through each Tigress position
		for(pos=1;pos<NPOSTIGR;pos++)
			{
				//reset suppression flag for every position
				suppFlag=0;
				//check if the position is in the hit pattern
				if((cev->tg.h.HHP&(1<<(pos-1)))!=0)
					//check the fold
					if(cev->tg.det[pos].hge.FH>0)
						//check if the position is in the addback hit pattern
						if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
							{
								//printf("AHP = %d\n",cev->tg.h.AHP);
								//reset take for add-back suppression
								take=0;
								//Run through four cores for each position
								for(col=0;col<NCOL;col++)
									{
										//check if the position and color is in the hit pattern
										if((cev->tg.det[pos].hge.HHP&(1<<col))!=0)
											//check the fold
											if(cev->tg.det[pos].ge[col].h.FH>0)
												{
													//ePreAddBack=cev->tg.det[pos].ge[col].seg[0].E/cal_par->tg.contr_e;
													//printf("pos %d col %d ePreAddback %f\n",pos,col,ePreAddBack);
													//suppress if the position is in the map and has not yet been suppressed
													if(cev->tg.det[pos].ge[col].suppress>=supLow && cev->tg.det[pos].ge[col].suppress<=supHigh && take==0)
														{
															/* once suppression flag is set
															do not reset it, could remove the take bit
															and keep resetting suppFlag, but this
															is nicer */
															suppFlag=1;
															take=1;
															//printf("event at pos %d col %d suppressed\n",pos,col);
														}
												}
									}
								
								eAddBack = cev->tg.det[pos].addback.E/cal_par->tg.contr_e;
								if(eAddBack>=0 && eAddBack<S32K)
									{
										if(suppFlag==0)
											numSuppG++;
									}
									
							}
			}

  free(cev);
    
  
  if(numSuppG<num_gammas)
    return SEPARATOR_DISCARD;
    
  encode(data,output,enb);
    
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  //TCanvas *canvas;
  //TApplication *theApp;

  if(argc!=5)
    {
      printf("separate_Tigress_NumSuppGammas master_file_name supLow supHigh num_gammas\n");
      printf("\n Separates out data containing at least num_gammas suppressed TIGRESS hits.\n");
      exit(-1);
    }
  
  
  supLow = atof(argv[2]);
  supHigh = atof(argv[3]);
  num_gammas=atoi(argv[4]);
  printf("Program sorts data containing at least %i suppressed TIGRESS hits.\n",num_gammas);
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

  read_master(argv[1],name);

  if(name->flag.inp_data!=1)
    {
      printf("\nInput data file not defined\n");
      exit(EXIT_FAILURE);
    }

  if(name->flag.TIGRESS_cal_par==1)
        {
          printf("\nTIGRESS calibration read from the file:\n %s\n",name->fname.TIGRESS_cal_par);
          initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
	  
        }
      else
        {
          printf("\nTIGRESS calibration parameters not defined\n");
          exit(-1);
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

