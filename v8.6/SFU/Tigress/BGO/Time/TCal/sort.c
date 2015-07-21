#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos,col,sup;
  double t;
  int tt=0;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));

  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  //check hit fold
  if(cev->tg.h.BFT>0)
    //BGO part - loop through positions, cores; check associated hit patterns
    for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.BTHP&(1<<(pos-1)))!=0)
	if(cev->tg.det[pos].hbgo.BFT>0)
	  for(col=0;col<NCOL;col++)
	    if((cev->tg.det[pos].hbgo.BTHP&(1<<col))!=0)
	      if(cev->tg.det[pos].bgo[col].h.BFT>0)
		for(sup=0;sup<NSUP;sup++)
		  if((cev->tg.det[pos].bgo[col].h.BTHP&(1<<sup))!=0)
		    {
		      //get BGO time from calibrated event file
		      t=cev->tg.det[pos].bgo[col].sup[sup].T/cal_par->tg.contr_t;

		      //testing
		      //t*=16;
		      //t/=10;
		      //t-=S16K;

		      tt=(int)rint(t);
		      
		      /* if(pos==7 && col==0 && sup==3) */
		      /* 	{ */
		      /* 	  printf("BGO TCal pos %d col %d sup %d time %.4f time %d\n",pos,col,sup,t,tt); */
		      /* 	  getc(stdin); */
		      /* 	} */

		      if((tt>=0) && (tt<S32K))
			hist[pos][col][sup][tt]++;
		      
		      else
			hist[pos][col][sup][S32K-5000]++;
		    }
   
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  FILE * output;
  char mcaFile[132];
  int stop;

  if(argc!=2)
    {
      printf("TigressBGO_TCal master_file_name\n");
      exit(-1);
    }
  printf("Program sorts calibrated BGO time spectra\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  memset(hist,0,sizeof(hist));

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

  sort(name);

  for(pos=1;pos<NPOSTIGR;pos++)
    {
      stop=0;
      
      for(col=0;col<NCOL;col++)
	stop+=cal_par->tg.ctflag[pos][col];
      
      if(stop>0)
	{
	  
	  for(col=0;col<NCOL;col++)
	    {
	      sprintf(mcaFile,"pos%1d%1d_col%d_BGOTCal.mca",pos/10,pos-(pos/10)*10,col);
	      if((output=fopen(mcaFile,"w"))==NULL)
		{
		  printf("ERROR!!! I cannot open the mca file!\n");
		  exit(EXIT_FAILURE);
		}
	      for(sup=0;sup<NSUP;sup++) 
		fwrite(hist[pos][col][sup],S32K*sizeof(int),1,output);
	      
	      fclose(output);
	    }
	  
	}
      
    }
}
