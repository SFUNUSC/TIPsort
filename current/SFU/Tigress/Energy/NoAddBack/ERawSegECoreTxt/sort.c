#include "sort.h"

int analyze_data(raw_event *data)
{
	cal_event* cev;
  int pos,col,sgm;
  double e,eseg;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  if(cev->tg.h.FE>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.EHP&(1<<(pos-1)))!=0)
				if(cev->tg.det[pos].hge.FE>0)
					for(col=0;col<NCOL;col++)
					  if((cev->tg.det[pos].hge.EHP&(1<<col))!=0)
					    if(cev->tg.det[pos].ge[col].h.FE>0)
								if((cev->tg.det[pos].ge[col].h.EHP&1)!=0)
									{
										e=cev->tg.det[pos].ge[col].seg[0].E/cal_par->tg.contr_e; //core energy
										for(sgm=1;sgm<9;sgm++)//loop over real segments
											{
												if((data->tg.det[pos].ge[col].h.EHP&(1<<sgm))!=0)
												//if((data->tg.det[pos].ge[col].h.EHP&1)!=0)
													{
														eseg=data->tg.det[pos].ge[col].seg[sgm].charge; //segment energy
													
														if(e>=ecorelow && e<ecorehigh)
															if(eseg>=eseglow && eseg<eseghigh)
																{
																	//print data to ASCII file(s)
																	fprintf(output[pos-1][col][sgm-1],"%f %f\n",eseg,e);
																}
													}
											}
									}
								
	free(cev);
  return SEPARATOR_DISCARD;
}
/*=======================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  char OutName[132];
  int pos,col,sgm;

  if(argc!=6)
    {
      printf("Tigress_ERawSegECoreTxt master_file_name ecorelow ecorehigh eseglow eseghigh\n");
      printf("Program sorts raw energy data for Tigress segments vs calibrated core energies, and prints this data on an event by event basis to text files, for use in plotting/fitting software in order to calibrate segments.\n");
      printf("The last 4 parameters specify energy gates for the core and segment data.\n");
      exit(-1);
    }
  
  printf("Program sorts raw energy data for Tigress segments vs calibrated core energies.\n");
				
	name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  read_master(argv[1],name);
  ecorelow=atof(argv[2]);
  ecorehigh=atof(argv[3]);
  eseglow=atof(argv[4]);
  eseghigh=atof(argv[5]);

  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined!\n");
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
	
	//open all ASCII output data files
	for(pos=1;pos<NPOSTIGR;pos++)
		for(col=0;col<NCOL;col++)
			for(sgm=1;sgm<9;sgm++)
				{
					sprintf(OutName,"ESegECore_pos%1d_col%1d_seg%1d.txt",pos,col,sgm);
					if((output[pos-1][col][sgm-1]=fopen(OutName,"w"))==NULL)
						{
							printf("ERROR: cannot open the output file %s\n",OutName);
							exit(EXIT_FAILURE);
						}
					else
						{
							//write header for fitting in the gridlock code
							fprintf(output[pos-1][col][sgm-1],"FIT  par1\n");
							fprintf(output[pos-1][col][sgm-1],"PLOT  1d\n");
							fprintf(output[pos-1][col][sgm-1],"LINEAR_FILTER 0.3\n");
							fprintf(output[pos-1][col][sgm-1],"COEFFICIENTS\n");
						}
				}

  sort(name);
  
  //close all ASCII output data files
	for(pos=1;pos<NPOSTIGR;pos++)
		for(col=0;col<NCOL;col++)
			for(sgm=1;sgm<9;sgm++)
				fclose(output[pos-1][col][sgm-1]);

}
