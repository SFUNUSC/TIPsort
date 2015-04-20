#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int TIGpos,TIGcol;
  //int BGOpos,BGOcol,BGOsup;
  int tdiff=0;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  //all the checks are done in the calibrator - this only iterates time difference spectrum
  //for events which are suppressed in the suppression algorithm
  for(TIGpos=1;TIGpos<NPOSTIGR;TIGpos++)
    for(TIGcol=0;TIGcol<NCOL;TIGcol++)
      if(cev->tg.det[TIGpos].ge[TIGcol].suppress)
	{
	  tdiff = (int)rint(cev->tg.det[TIGpos].ge[TIGcol].suppress);
	  hist[TIGpos][TIGcol][tdiff]++;
	}
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  FILE *cluster;
  char DataFile[132];
  FILE *output;
  int stop,pos,col;
  char n[132];
  
  if(argc!=2)
    {
      printf("TigressBGO_TDiff master_file_name\n");
      exit(-1);
    }

  printf("Program sorts spectra for Tigress/BGO time difference using the CFD algorithm\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

  read_master(argv[1],name);

if(name->flag.cluster_file==1)
    {
      printf("Sorting Tigress/BGO time difference histograms from cluster file: %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("ERROR! I can't open input file %s\n",name->fname.cluster_file);
	  exit(-2);
	}
    }
  else
    {
      printf("ERROR! Cluster file not defined\n");
      exit(-1);
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

  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting data from %s\n", name);
      sort(name); 
    }

  for(pos=1;pos<NPOSTIGR;pos++)
    {
      stop=0;
      for(col=0;col<NCOL;col++)
	stop+=cal_par->tg.ceflag[pos][col];
      if(stop>0)
	{
	  sprintf(n,"pos%1d%1d_TigressBGO_TDiffSum.mca",pos/10,pos-(pos/10)*10);
	  if((output=fopen(n,"w"))==NULL)
	    {
	      printf("ERROR! I can't open: %s\n",n);
	      exit(EXIT_FAILURE);
	    }
	  
	  for(col=0;col<NCOL;col++)
	    fwrite(hist[pos][col],S32K*sizeof(int),1,output);
	  
	  fclose(output);
	}
    }

}

