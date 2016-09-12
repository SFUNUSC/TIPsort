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
  int TIGpos,TIGcol,BGOpos,BGOcol,BGOsup;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  //check the Ge fold
  if(cev->tg.h.FH>0)
    //look through each Tigress position
    for(TIGpos=1;TIGpos<NPOSTIGR;TIGpos++)
      {
	suppFlag=0;
      //check if the position is in the hit pattern
      if((cev->tg.h.HHP&(1<<(TIGpos-1)))!=0)
	//check the energy fold
	if(cev->tg.det[TIGpos].hge.FE>0)
	  //check if the position is in the addback hit pattern
	  if((cev->tg.h.AHP&(1<<(TIGpos-1)))!=0)
	    {
	      //printf("AHP = %d\n",cev->tg.h.AHP);
	      take=0;
	      //Run through four cores for each position
	      for(TIGcol=0;TIGcol<NCOL;TIGcol++)
		{
		  //check if the position and color is in the hit pattern
		  if((cev->tg.det[TIGpos].ge[TIGcol].h.HHP&1)!=0)
		    //suppress if the position is in the map and has not yet been suppressed
		    if(cev->tg.det[TIGpos].ge[TIGcol].suppress && take==0)
		      { 
			/* once suppression flag is set
			   do not reset it, could remove the take bit
			   and keep resetting suppFlag, but this
			   is nicer */   
			suppFlag=1;
			take=1;
			
			/* when suppression flag is set generate a Tigress/BGO time difference
			   spectrum for the Tigress position which was suppressed and *all* BGO in the event 
			   I'm not sure how to do it only for the BGO which was actively suppressed since this
			   information is not available here (done in calibrator and not shared) */
			for(BGOpos=1;BGOpos<NPOSTIGR;BGOpos++)
			  //Check BGO fold
			  if(data->tg.h.BGOfold>0)
			    //check if this position is in the hit pattern with fold > 0
			    if(((data->tg.h.BGOHP&(1<<(BGOpos-1)))!=0) && (data->tg.det[BGOpos].h.BGOfold>0))
			      //run through all the colors
			      for(BGOcol=0;BGOcol<NCOL;BGOcol++)
				 //check if this color is in the hit pattern with a fold > 0
				if(((data->tg.det[BGOpos].h.BGOHP&(1<<BGOcol))!=0) && (data->tg.det[BGOpos].bgo[BGOcol].h.Tfold>0))
				  for(BGOsup=0;BGOsup<NSUP;BGOsup++)
				    //check if this suppressor is in the hit pattern with fold > 0
				    if(((data->tg.det[BGOpos].bgo[BGOcol].h.THP&(1<<BGOsup))!=0))// && (take==0))
				      {	      
					//tigress timing from cfd
					ttg=data->tg.det[TIGpos].ge[TIGcol].seg[0].cfd&0x00ffffff;
					//printf("tigress cfd: %0.f\n",ttg);
					ttg-=(data->tg.det[TIGpos].ge[TIGcol].seg[0].timestamp*16)&0x00ffffff;
					//printf("tigress cfd-timestamp: %0.f\n",ttg);
					
					//bgo timing from cfd
					tbgo=data->tg.det[BGOpos].bgo[BGOcol].sup[BGOsup].cfd&0x00ffffff;
					//printf("bgo cfd: %0.f\n",tbgo);
					tbgo-=(data->tg.det[BGOpos].bgo[BGOcol].sup[BGOsup].timestamp*16)&0x00ffffff;
					//printf("bgo cfd-timestamp: %0.f\n",tbgo);
					
					if((data->h.setupHP&RF_BIT)!=0)
					  {
					    //subtract RF phase from bgo time and tigress time
					    tbgo-=(int)data->rf.sin.t0;
					    ttg-=(int)data->rf.sin.t0;
					    
					    //printf("rf phase: %d\n",(int)data->rf.sin.t0);
					    //printf("bgo cfd-timestamp-RF: %0.f\n",tbgo);
					    //printf("tigress cfd-timestamp-RF: %0.f\n",ttg);
					    
					    //add offset
					    tbgo+=S16K;
					    ttg+=S16K;
					    
					    tdiff=ttg-tbgo;
					    tdiff+=S4K;
					    tdiff/=16;
					    //printf("tdiff = ttg-tbgo: %0.f\n",tdiff);
					    //getc(stdin);
					    
					  }
					
					addBackCol = cev->tg.det[TIGpos].addbackC;
					
					if(tdiff>=0 && tdiff<=S32K)
					  hist[TIGpos][addBackCol][(int)rint(tdiff)]++;
					else
					  hist[TIGpos][addBackCol][S32K-1000]++;
					
				      }
		      }
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

  if(name->flag.cluster_file==1)
    {
      printf("Sorting ECalABSuppSum histograms for TIGRESS clovers and cores based upon the cluster file: %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("ERROR!!! I can't open input file %s\n",name->fname.cluster_file);
	  exit(-2);
	}}
  else
    {
      printf("ERROR!!! Cluster file not defined\n");
      exit(-1);
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
  
  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting data from file %s\n", name);
      sort(name);
    }
  
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
