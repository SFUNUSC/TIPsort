//Comments by PJV. Code copied, then modified, from SFU-cal-TIGRESS.c
#include "SFU-cal-GRIFFIN.h"
/********************************************************************/
void initialize_GRIFFIN_calibration(GRIFFIN_calibration_parameters *GRIFFIN_cal_par, char *filename)
{
  FILE *inp;
  char str1[256],str2[256];

  memset(GRIFFIN_cal_par,0,sizeof(*GRIFFIN_cal_par));
  GRIFFIN_cal_par->contr_e=1.0;
  GRIFFIN_cal_par->contr_t=1.0;

  if((inp=fopen(filename,"r"))==NULL)
    {
      printf("Cannot open %s.\n",filename);
      exit(EXIT_FAILURE);
    }
  else
    printf("GRIFFIN calibration parameters read from %s.\n",filename);
  
  if(fgets(str1,256,inp)!=NULL)
    {
      if(fgets(str1,256,inp)!=NULL)
	{
	  while(fscanf(inp,"%s %s",str1,str2)!=EOF)
	    {
	      if(strcmp(str1,"Energy_spectra_contraction_factor")==0)
		{
		  GRIFFIN_cal_par->contr_e=atof(str2); 
		  printf("contraction E is %f\n",GRIFFIN_cal_par->contr_e);
		}
	      if(strcmp(str1,"GRIFFIN_RFunwrapping_offset")==0)
		{
		  GRIFFIN_cal_par->offset=atoi(str2);
		  printf("RFunwrapping GRIFFIN_offset is %d\n",GRIFFIN_cal_par->offset);
		}   	      
	      if(strcmp(str1,"GRIFFIN_RFunwrapping_shift")==0)
		{
		  GRIFFIN_cal_par->shift=atoi(str2);
		  printf("RFunwrapping GRIFFIN_shift is %d\n",GRIFFIN_cal_par->shift);
		}   	      
	      if(strcmp(str1,"Time_spectra_contraction_factor")==0)
		{
		  GRIFFIN_cal_par->contr_t=atof(str2);
		  printf("contraction T is %f\n",GRIFFIN_cal_par->contr_t);
		}
	      if(strcmp(str1,"Core_energy_calibration_parameters")==0)
		read_GRIFFIN_core_e_cal_par(GRIFFIN_cal_par,str2);
	      
	      if(strcmp(str1,"Core_time_calibration_parameters")==0)
		read_GRIFFIN_core_t_cal_par(GRIFFIN_cal_par,str2);
	      
	      if(strcmp(str1,"Ring_map")==0)
		read_GRIFFIN_ring_map(GRIFFIN_cal_par,str2);
	      
	      if(strcmp(str1,"Core_energy_limits")==0)
		read_GRIFFIN_core_energy_limits(GRIFFIN_cal_par,str2);
	      
	      if(strcmp(str1,"Core_time_limits")==0)
		read_GRIFFIN_core_time_limits(GRIFFIN_cal_par,str2);
	    }}}
  else
    {
      printf("Wrong structure of %s.\n",filename);
      printf("Aborting sort.\n");
      exit(1);
    }    
  fclose(inp);
}
/***********************************************************************/
void read_GRIFFIN_core_e_cal_par(GRIFFIN_calibration_parameters *GRIFFIN_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  pos,col,i;
  double a[3];
  
  if((inp=fopen(filename,"r"))==NULL)
    {
      printf("Cannot open %s.\n",filename);
      exit(EXIT_FAILURE);
    }
  printf("Core energy calibration parameters read from %s.\n",filename);
  
  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %d %lf %lf %lf",&pos,&col,&a[0],&a[1],&a[2])!=EOF)
	  if((pos>0&&pos<NPOSGRIF) && (col>=0&&col<NCOL))
	    {
	      GRIFFIN_cal_par->ceflag[pos][col]=1;
	      for(i=0;i<3;i++) GRIFFIN_cal_par->ce[pos][col][i]=a[i];
	    }}
  else
    {
      printf("Wrong structure of %s.\n",filename);
      printf("Aborting sort.\n");
      exit(1);
    }
  fclose(inp);
}
/***********************************************************************/
void read_GRIFFIN_core_t_cal_par(GRIFFIN_calibration_parameters *GRIFFIN_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  pos,col,i;
  double a[2];
  
  if((inp=fopen(filename,"r"))==NULL)
    {
      printf("Cannot open %s.\n",filename);
      exit(EXIT_FAILURE);
    }
  printf("Core time calibration parameters read from %s.\n",filename);
  
  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %d %lf %lf",&pos,&col,&a[0],&a[1])!=EOF)
	  if((pos>0&&pos<NPOSGRIF) && (col>=0&&col<NCOL))
	    {
	      GRIFFIN_cal_par->ctflag[pos][col]=1;
	      for(i=0;i<2;i++) GRIFFIN_cal_par->ct[pos][col][i]=a[i];
	    }}
  else
    {
      printf("Wrong structure of %s.\n",filename);
      printf("Aborting sort.\n");
      exit(1);
    }
  fclose(inp);
}
/***********************************************************************/
void read_GRIFFIN_ring_map(GRIFFIN_calibration_parameters *GRIFFIN_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  pos,col,i;
 
  if((inp=fopen(filename,"r"))==NULL)
    {
	printf("Cannot open %s.\n",filename);
	exit(EXIT_FAILURE);
    }
  printf("GRIFFIN ring map read from %s.\n",filename);
  
  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %d %d",&pos,&col,&i)!=EOF)
	  if((pos>0&&pos<NPOSGRIF) && (col>=0&&col<NCOL))
	    {
	      GRIFFIN_cal_par->ringflag[pos][col]=1;
	      GRIFFIN_cal_par->ring_map[pos][col]=i;
	    }}
  else
    {
      printf("Wrong structure of %s.\n",filename);
      printf("Aborting sort.\n");
      exit(1);
    }
  fclose(inp);
}
/***********************************************************************/
void read_GRIFFIN_core_energy_limits(GRIFFIN_calibration_parameters *GRIFFIN_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  pos,col;
  float low,high;
  
  if((inp=fopen(filename,"r"))==NULL)
    {
      printf("Cannot open %s.\n",filename);
      exit(EXIT_FAILURE);
    }
  printf("GRIFFIN energy limits read from %s.\n",filename);
  
  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %d %f %f",&pos,&col,&low,&high)!=EOF)
	  if((pos>0&&pos<NPOSGRIF) && (col>=0&&col<NCOL))
	    {
	      GRIFFIN_cal_par->celow[pos][col]=low;
	      GRIFFIN_cal_par->cehigh[pos][col]=high;
	    }}
  else
    {
      printf("Wrong structure of %s.\n",filename);
      printf("Aborting sort.\n");
      exit(1);
    }
  fclose(inp);
}
/***********************************************************************/
void read_GRIFFIN_core_time_limits(GRIFFIN_calibration_parameters *GRIFFIN_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  pos,col;
  float low,high;
  
  if((inp=fopen(filename,"r"))==NULL)
    {
      printf("Cannot open %s.\n",filename);
      exit(EXIT_FAILURE);
    }
  printf("GRIFFIN time limits read from %s.\n",filename);
  
  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %d %f %f",&pos,&col,&low,&high)!=EOF)
	  if((pos>0&&pos<NPOSGRIF) && (col>=0&&col<NCOL))
	    {
	      GRIFFIN_cal_par->ctlow[pos][col]=low;
	      GRIFFIN_cal_par->cthigh[pos][col]=high;
	    }}
  else
    {
      printf("Wrong structure of %s.\n",filename);
      printf("Aborting sort.\n");
      exit(1);
    }
  fclose(inp);
}
/*******************************************************************/
void summarize_GRIFFIN_calibration(GRIFFIN_calibration_parameters *GRIFFIN_cal_par, char *filename)
{
  FILE *out;
  int  pos,col;

  if((out=fopen(filename,"w"))==NULL)
    {
      printf("Cannot open %s.\n",filename);
      exit(EXIT_FAILURE);
    }
  
  fprintf(out,"GRIFFIN core energy calibration parameters\n");
  fprintf(out,"  pos#  col#    a0             a1             a2\n");
  for(pos=1;pos<NPOSGRIF;pos++)
    for(col=0;col<NCOL;col++)
      if(GRIFFIN_cal_par->ceflag[pos][col]==1)
	fprintf(out,"   %02d  %02d  %12.5e   %12.5e   %12.5e\n",pos,col,GRIFFIN_cal_par->ce[pos][col][0],
		GRIFFIN_cal_par->ce[pos][col][1],GRIFFIN_cal_par->ce[pos][col][2]);
  fprintf(out,"GRIFFIN core time calibration parameters\n");
  fprintf(out,"  pos#  col#    a0             a1 \n");
  for(pos=1;pos<NPOSGRIF;pos++)
    for(col=0;col<NCOL;col++)
      if(GRIFFIN_cal_par->ctflag[pos][col]==1)
	fprintf(out,"   %02d  %02d  %12.5e   %12.5e\n",pos,col,GRIFFIN_cal_par->ct[pos][col][0],
		GRIFFIN_cal_par->ct[pos][col][1]);
  fclose(out);
}
/********************************************************************/
void calibrate_GRIFFIN(raw_event *raw_event, GRIFFIN_calibration_parameters *GRIFFIN_cal_par, cGRIFFIN *GRIFFIN_cal_ev)
{
  long long int one=1;
  int pos,col,addback_c=-1,addback_n=0;
  double ran,ren,e,t;
  double addback_e=0.0,addback_t=0.0,addback_max=-1;
  double trf;//,func;
  //  long int a;
  
  memset(GRIFFIN_cal_ev,0,sizeof(cGRIFFIN));
  
  /* core energy calibration */
  if(raw_event->gr.h.Gefold>0)
    for(pos=1;pos<NPOSGRIF;pos++)
      if((raw_event->gr.h.GeHP&(one<<(pos-1)))!=0)
	if(raw_event->gr.det[pos].h.Gefold>0)
	  for(col=0;col<NCOL;col++)
	    if(GRIFFIN_cal_par->ceflag[pos][col]==1)
	      if(((raw_event->gr.det[pos].h.GeHP)&(one<<col))!=0)
		if(raw_event->gr.det[pos].ge[col].h.Efold>0)
		  if((raw_event->gr.det[pos].ge[col].h.EHP&1)!=0)
		    {
		      ran=(double)rand()/(double)RAND_MAX-0.5;
		      ren=raw_event->gr.det[pos].ge[col].seg[0].charge+ran;
		      e=GRIFFIN_cal_par->ce[pos][col][0]+GRIFFIN_cal_par->ce[pos][col][1]*ren+GRIFFIN_cal_par->ce[pos][col][2]*ren*ren;
		      if((e>=0) && (e<4*S32K))
			{
			  GRIFFIN_cal_ev->det[pos].ge[col].seg[0].E=e;
			  GRIFFIN_cal_ev->det[pos].ge[col].h.FE++;
			  GRIFFIN_cal_ev->det[pos].ge[col].h.EHP|=1;
			  GRIFFIN_cal_ev->det[pos].ge[col].ring=GRIFFIN_cal_par->ring_map[pos][col];
			  GRIFFIN_cal_ev->det[pos].hge.FE++;
			  GRIFFIN_cal_ev->det[pos].hge.EHP|=(one<<col);
			  GRIFFIN_cal_ev->h.FE++;
			  GRIFFIN_cal_ev->h.EHP|=(one<<(pos-1));
			}
		      else if(e<0) printf("Warning! Calibrated Griffin energy is less than 0!\n");
		      else         printf("Warning! Calibrated Griffin energy is greater than 4*S23K! Raw energy was %f.\n",(ren-ran));
		    }
  
  /* core time calibration */
  if(raw_event->gr.h.Gefold>0)
    for(pos=1;pos<NPOSGRIF;pos++)
      if((raw_event->gr.h.GeHP&(one<<(pos-1)))!=0)
	if(raw_event->gr.det[pos].h.Gefold>0)
	  for(col=0;col<NCOL;col++)
	    if(GRIFFIN_cal_par->ctflag[pos][col]==1)
	      if(((raw_event->gr.det[pos].h.GeHP)&(one<<col))!=0)
		if(raw_event->gr.det[pos].ge[col].h.Tfold>0)
		  if((raw_event->gr.det[pos].ge[col].h.THP&1)!=0)
		    {
		      //The bitmask is redundant here since it has already
		      //been applied in tig-format.c. I checked by generating output
		      //with the mask in place and removed. "t" didn't change.
		      t=raw_event->gr.det[pos].ge[col].seg[0].cfd&0x00ffffff;
		      //Multiplying by 16 in decimal is like a left bitshift by 1 in hex
		      //truly, there is no such thing, but is shifts and adds 0 to the right
		      //so what this is really like is multiplying by 10 in decimal
		      t-=(raw_event->gr.det[pos].ge[col].seg[0].timestamp*16)&0x00ffffff;
		      
		      /* if((raw_event->h.setupHP&RF_BIT)!=0) */
		      /* 	{ */
		      /* 	  trf=raw_event->rf.sin.t0; //RF is in CFD units now. (see map.c) */
			  
		      /* 	  //			  func=(1.000*trf)-(RFphase+GRIFFIN_cal_par->offset); */
		      /* 	  //			  a=fmod(t+GRIFFIN_cal_par->shift,RFphase); */
			  			  
		      /* 	  //			  if(a>func) trf+=RFphase; */
			  
		      /* 	  t-=trf; */
			  t+=S16K;
			  ran=(double)rand()/(double)RAND_MAX-0.5;
			  ren=t+ran;

			  t=GRIFFIN_cal_par->ct[pos][col][0]+GRIFFIN_cal_par->ct[pos][col][1]*ren;
			  if(t>0 && t<S65K)
			    {
			      GRIFFIN_cal_ev->det[pos].ge[col].seg[0].T=t;
			      GRIFFIN_cal_ev->det[pos].ge[col].h.FT++;
			      GRIFFIN_cal_ev->det[pos].ge[col].h.THP|=1;
			      GRIFFIN_cal_ev->det[pos].ge[col].ring=GRIFFIN_cal_par->ring_map[pos][col];
			      GRIFFIN_cal_ev->det[pos].hge.FT++;
			      //one is left-shifted by col. so col=3 would be 1000, col=0 would be 1.
			      GRIFFIN_cal_ev->det[pos].hge.THP|=(one<<col); 
			      GRIFFIN_cal_ev->h.FT++;
			      GRIFFIN_cal_ev->h.THP|=(one<<(pos-1));
			    }}//}
  
  if(GRIFFIN_cal_ev->h.FE>0)
    if(GRIFFIN_cal_ev->h.FT>0)
      for(pos=1;pos<NPOSGRIF;pos++)
	if((GRIFFIN_cal_ev->h.EHP&(one<<(pos-1)))!=0)
	  if((GRIFFIN_cal_ev->h.THP&(one<<(pos-1)))!=0)
	    if(GRIFFIN_cal_ev->det[pos].hge.FE>0)
	      if(GRIFFIN_cal_ev->det[pos].hge.FT>0)
		for(col=0;col<NCOL;col++)
		  if((GRIFFIN_cal_ev->det[pos].hge.EHP&(one<<col))!=0)
		    if((GRIFFIN_cal_ev->det[pos].hge.THP&(one<<col))!=0)
		      if(GRIFFIN_cal_ev->det[pos].ge[col].h.FE>0)
			if(GRIFFIN_cal_ev->det[pos].ge[col].h.FT>0)
			  if((GRIFFIN_cal_ev->det[pos].ge[col].h.EHP&1)!=0)
			    if((GRIFFIN_cal_ev->det[pos].ge[col].h.THP&1)!=0)
			      {
				GRIFFIN_cal_ev->det[pos].ge[col].h.HHP|=1;
				GRIFFIN_cal_ev->det[pos].ge[col].h.FH++;
				GRIFFIN_cal_ev->det[pos].hge.HHP|=(one<<col);
				GRIFFIN_cal_ev->det[pos].hge.FH++;
				GRIFFIN_cal_ev->h.HHP|=(one<<(pos-1));
				GRIFFIN_cal_ev->h.FH++;
			      }
  
  /* add back */
  if(GRIFFIN_cal_ev->h.FE>0)
    for(pos=1;pos<NPOSGRIF;pos++)
      if((GRIFFIN_cal_ev->h.EHP&(one<<(pos-1)))!=0)
	if(GRIFFIN_cal_ev->det[pos].hge.FE>0)
	  {
	    GRIFFIN_cal_ev->det[pos].addbackHP=0;
	    addback_e=0.;
	    addback_t=0.;
	    addback_n=0;
	    addback_c=-1;
	    addback_max=-1.;
	    for(col=0;col<NCOL;col++)
	      if((GRIFFIN_cal_ev->det[pos].hge.EHP&(one<<col))!=0)
		if(GRIFFIN_cal_ev->det[pos].ge[col].h.FE>0)
		  if((GRIFFIN_cal_ev->det[pos].ge[col].h.EHP&1)!=0)
		    {
		      e=GRIFFIN_cal_ev->det[pos].ge[col].seg[0].E;
		      //if((e>GRIFFIN_cal_par->celow[pos][col]) && (e<GRIFFIN_cal_par->cehigh[pos][col]))
		      t=GRIFFIN_cal_ev->det[pos].ge[col].seg[0].T;
		      //if((t>GRIFFIN_cal_par->ctlow[pos][col]) && (t<GRIFFIN_cal_par->cthigh[pos][col]))
		      GRIFFIN_cal_ev->det[pos].addbackHP|=(one<<col);

		      addback_e+=e;
		      addback_t+=t;
		      addback_n++;
		      
		      if(e>addback_max)
			{
			  addback_c=col;
			  addback_max=e;
			}}
	    
	    if(addback_e!=0)
	      {
		addback_t/=addback_n;
		GRIFFIN_cal_ev->det[pos].addback.E=addback_e;
		GRIFFIN_cal_ev->det[pos].addback.T=addback_t;
		GRIFFIN_cal_ev->det[pos].addbackF=addback_n;
		GRIFFIN_cal_ev->det[pos].addbackC=addback_c;
		GRIFFIN_cal_ev->h.AHP|=(one<<(pos-1));
		GRIFFIN_cal_ev->h.FA++;
	      }}}
