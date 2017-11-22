#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int trigger;
  int pos,col,csi;
  uint64_t one=1;
  unsigned long long tsCsI,tsTIG;
  double ttg,tcsi;

  if((data->h.setupHP&TIGRESS_BIT)==0)
    return SEPARATOR_DISCARD;
  
  if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;

 if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;
  
  ttg=0.;
  tcsi=0.;
  tsCsI=0x0;
  tsTIG=0x0;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);

  trigger=data->h.trig_num&0x7fffffff;
  
  if(cev->tg.h.FT>0)
    if(cev->csiarray.h.FT>0)
      for(pos=1;pos<NPOSTIGR;pos++)
	if((cev->tg.h.THP&(1<<(pos-1)))!=0)
	  if(cev->tg.det[pos].hge.FT>0)
	    for(col=0;col<NCOL;col++)
	      if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
		if(cev->tg.det[pos].ge[col].h.FT>0)
		  if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
		    {
		      ttg=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;
		      tsTIG=((unsigned long long)data->tg.det[pos].ge[col].seg[0].timestamp&0x00ffffff);	
		      for(csi=1;csi<NCSI;csi++)
			if((cev->csiarray.h.THP[csi/64]&(one<<csi%64))!=0)
			  if((data->csiarray.h.TSHP[csi/64]&(one<<csi%64))!=0) //why?
			  {
			    tcsi=cev->csiarray.csi[csi].T/cal_par->csiarray.contr_t;
			    tsCsI=((unsigned long long)data->csiarray.csi[csi].timestamp&0x00ffffff);
			    if(tcsi>=tcsi_low && tcsi<=tcsi_high)
			      if(ttg>=ttg_low && ttg<=ttg_high)
				{
				  printf("trigger %6d ttg %.2f tsTig %6.6llx tcsi %.2f tsCsI %6.6llx\n",trigger,ttg,tsTIG,tcsi,tsCsI);
				  //getc(stdin);
				}
			  }
		    }
  
  /* trigger=cev->h.trig_num&0x7fffffff; */
  /* printf("==================================\n"); */
  /* printf("trigger %d\n",trigger); */

    
  /* printf("==================================\n"); */
  /* getc(stdin); */
  
  return SEPARATOR_DISCARD;
}
/*========================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  
  if(argc!=6)
    {
      printf("check_TimeStampDifference master_file_name tcsi_low tcsi_high ttg_low ttg_high\n");
      exit(-1);
    }
  
  printf("Program checks trigger numbers for events within a given range of calibrated TIGRESS and CsI times.\n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  strcpy(name->fname.inp_data,argv[1]);

  tcsi_low=atof(argv[2]);
  tcsi_high=atof(argv[3]);
  ttg_low=atof(argv[4]);
  ttg_high=atof(argv[5]);

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

  if(name->flag.CSIARRAY_cal_par==1)
        {
          printf("\nCSIARRAY calibration read from the file:\n %s\n",name->fname.CSIARRAY_cal_par);
          initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
	  
        }
      else
        {
          printf("\nCSIARRAY calibration parameters not defined\n");
          exit(-1);
        }

  sort(name); 
}
