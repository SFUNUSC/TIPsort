#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  double eAddBack=-1.;
  //double ePreAddBack=-1.;
  int suppFlag=0;
  int take=0;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  //printf("cntr: %f\n",cal_par->tg.contr_e);
  //getc(stdin);
  
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
								colAddBack = cev->tg.det[pos].addbackC;
								ring = cev->tg.det[pos].ge[colAddBack].ring+NRING*suppFlag;
								
								if(eAddBack>=0 && eAddBack<S32K)
									hist[ring][(int)(eAddBack)]++;
								else
									{
										//printf("eAddback %f\n",eAddBack);
										hist[ring][S32K-1000]++;
									}
							}
			}
	//printf("***** END OF EVENT *****\n");
	//getc(stdin);
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*=========================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  
  if(argc!=4)
    {
      printf("Tigress_ECalABSuppRing master_file_name supLow supHigh\n");
      exit(-1);
    }
  
  printf("Program sorts ECalABSuppRing histograms for TIGRESS.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);
  supLow = atof(argv[2]);
  supHigh = atof(argv[3]);

  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined.\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calpar read from: %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR!!! TIGRESS calibration parameters not defined\n");
      exit(EXIT_FAILURE);
    }
  
  sort(name);
  
  if((output=fopen("Ring_ECalABSupp.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(hist,2*NRING*S32K*sizeof(int),1,output);
  fclose(output);
}
