#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos,csi;
  double etig,ecsi;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);

 if((data->h.setupHP&TIGRESS_BIT)==0)
    return SEPARATOR_DISCARD;

 //if((data->h.setupHP&RF_BIT)==0)
 //   return SEPARATOR_DISCARD;

 if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;

 if(cev->tg.h.FH>0)
   if(cev->csiarray.h.FH>0)
     for(pos=1;pos<NPOSTIGR;pos++)
       if(((cev->tg.h.HHP&(1<<(pos-1)))!=0) && (cev->tg.det[pos].hge.FE>0))
				 if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
					 {	
						 etig = cev->tg.det[pos].addback.E; /* TIGRESS energy in keV */
						 
						 if(cev->csiarray.h.FH>0)
							 for(csi=1;csi<NCSI;csi++)
								 if((cev->csiarray.h.HHP&(one<<csi))!=0)
									 {
										 //ecsi=cev->csiarray.csi[csi].E/1000.; /* CsI energy in MeV */
										 ecsi=cev->csiarray.csi[csi].E; /* CsI energy in keV */
										 
										 if(etig>=eLow && etig<=eHigh)
											 {
												 if(ecsi<0 || ecsi>S32K-10) 
													 ecsi=S32K-10;
												 
												 hist[csi][(int)(ecsi)]++;
											 }
									 }
					 }
 
 free(cev);
 return SEPARATOR_DISCARD;
}
/*=============================================================================*/
int main(int argc, char *argv[])
{
  FILE *output;
  input_names_type* name;
 
  if(argc!=4)
    {
      printf("TigressCsIArray_CsIECal_TigEGate master_file_name Elow [keV] Ehigh [keV] \n");
      exit(-1);
    }

  //CsI energies for gating
  eLow=atof(argv[2]);
  eHigh=atof(argv[3]);
  
  printf("Program sorts CsI energy histograms for events with TIGRESS hits in the specified energy range.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  memset(hist,0,sizeof(hist));

  read_master(argv[1],name);
  
  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calibration read from %s.\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  if(name->flag.CSIARRAY_cal_par==1)
    {
      printf("CsIArray calpar read from: %s\n",name->fname.CSIARRAY_cal_par);
      initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
    }
  else
    {
      printf("ERROR!!! TIGRESS calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  sort(name);

  if((output=fopen("CsIECal_TigEGate.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  
  fwrite(hist,NCSI*S32K*sizeof(int),1,output);

  fclose(output);

  return 0;
}
