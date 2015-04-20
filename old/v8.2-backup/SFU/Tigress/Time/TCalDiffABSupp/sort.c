#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int suppFlag=0;
  int take=0;
  int addBackCol=0;

  //for time spectra
  double ttg,tbgo;
  double tdiff=-1.;
  int tigPos,tigCol,bgoPos,bgoCol,bgoSup;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  //check the Ge fold
  //if(cev->tg.h.FH>0)
    //look through each Tigress position
    for(tigPos=1;tigPos<NPOSTIGR;tigPos++)
      {
	//suppFlag=0;
	for(tigCol=0;tigCol<NCOL;tigCol++)
	  {
	    
	    //suppress if the position is in the map and has not yet been suppressed
	    if(cev->tg.det[tigPos].ge[tigCol].suppress)
	      { 
	    /* once suppression flag is set
	       do not reset it, could remove the take bit
	       and keep resetting suppFlag, but this
	       is nicer */   
		//note - need to remove take now that calibrated event suppression encodes the time difference
		//suppFlag=1;
		//take=1;
		
		//addBackCol = cev->tg.det[tigPos].addbackC;
		tdiff=cev->tg.det[tigPos].ge[tigCol].suppress;

		//contracton by 16
		//tdiff/=16;

		//unnecessary check since this is done in the calibrator
		if(tdiff>=0 && tdiff<S32K)
		  hist[tigPos][tigCol][(int)rint(tdiff)]++;
		else
		  hist[tigPos][tigCol][S32K-1000]++;

	      }
	  }
      }
  
  free(cev);
  return SEPARATOR_DISCARD;
  
}
/*=========================================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *cluster;
  input_names_type* name;
  char mcaFile[132],DataFile[132];
  int pos,col;
  
  if(argc!=2)
    {
      printf("Tigress_ECalABSuppSum master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts ECalABSuppSum histograms for TIGRESS.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);

  if(name->flag.inp_data!=1)
    {
      printf("ERROR! Input data file not defined.\n");
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
  
  printf("Sorting data from file %s\n", name);
  sort(name);
 
  for(pos=1;pos<NPOSTIGR;pos++)
    {
      sprintf(mcaFile,"pos%1d%1d_TCalDiffABSuppSum.mca",pos/10,pos-(pos/10)*10);
      if((output=fopen(mcaFile,"w"))==NULL)
	{
	  printf("ERROR!!! I cannot open the mca file!\n");
	  exit(EXIT_FAILURE);
	}
      
      for(col=0;col<NCOL;col++) fwrite(hist[pos][col],S32K*sizeof(int),1,output);
      fclose(output);
    }
}
