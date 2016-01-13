#include "SFU-cal-TIGRESS.h"
/********************************************************************/
void initialize_TIGRESS_calibration(TIGRESS_calibration_parameters *TIGRESS_cal_par, char *name)
{
  FILE *inp;
  char str1[256],str2[256];

  memset(TIGRESS_cal_par,0,sizeof(*TIGRESS_cal_par));
  TIGRESS_cal_par->contr_e=1.;
  TIGRESS_cal_par->contr_t=1.;
  TIGRESS_cal_par->use_time_fit_tgr=0;
  TIGRESS_cal_par->use_time_fit_bgo=0;
  //For RFUnwrapping
  TIGRESS_cal_par->DoRFUnwrapping=0;

  if((inp=fopen(name,"r"))==NULL)
      {
         printf("\nFile %s can not be opened\n",name);
         exit(EXIT_FAILURE);
      }
  else
    printf("TIGRESS calibration parameters read from file %s\n",name);

   if(fgets(str1,256,inp)!=NULL)
    {
      if(fgets(str1,256,inp)!=NULL)
	{
	  while(fscanf(inp,"%s %s",str1,str2)!=EOF)
	    {
	      if(strcmp(str1,"Energy_spectra_contraction_factor")==0)
		TIGRESS_cal_par->contr_e=atof(str2);	      
	      
	      if(strcmp(str1,"Time_spectra_contraction_factor")==0)
		TIGRESS_cal_par->contr_t=atof(str2);
	      
	      if(strcmp(str1,"Core_energy_calibration_parameters")==0)
		read_TIGRESS_core_e_cal_par(TIGRESS_cal_par,str2);
	      
	      if(strcmp(str1,"Core_time_calibration_parameters")==0)
		read_TIGRESS_core_t_cal_par(TIGRESS_cal_par,str2);

	      if(strcmp(str1,"Ring_map")==0)
		read_TIGRESS_ring_map(TIGRESS_cal_par,str2);

	      if(strcmp(str1,"Ring_energy_gates")==0)
		read_TIGRESS_ring_energy_gates(TIGRESS_cal_par,str2);

	      if(strcmp(str1,"Core_energy_limits")==0)
		read_TIGRESS_core_energy_limits(TIGRESS_cal_par,str2);

	      if(strcmp(str1,"Core_time_limits")==0)
		read_TIGRESS_core_time_limits(TIGRESS_cal_par,str2);
		
		    if(strcmp(str1,"Detector_position_file")==0)
		read_TIGRESS_detector_positions(TIGRESS_cal_par,str2);

	      if(strcmp(str1,"TIGRESS_timing_from")==0)
		{
		  if(strcmp(str2,"fit")==0)
		    TIGRESS_cal_par->use_time_fit_tgr=1;
		  if(strcmp(str2,"FIT")==0)
		    TIGRESS_cal_par->use_time_fit_tgr=1;
		  if(strcmp(str2,"Fit")==0)
		    TIGRESS_cal_par->use_time_fit_tgr=1;
		  //printf("Tigress using fit from timing = %d (0=daq, 1=fit)\n",TIGRESS_cal_par->use_time_fit_tgr);
		  //getc(stdin);
		}
	      

	      if(strcmp(str1,"BGO_timing_from")==0)
		{
		  if(strcmp(str2,"fit")==0)
		    TIGRESS_cal_par->use_time_fit_bgo=1;
		  if(strcmp(str2,"FIT")==0)
		    TIGRESS_cal_par->use_time_fit_bgo=1;
		  if(strcmp(str2,"Fit")==0)
		    TIGRESS_cal_par->use_time_fit_bgo=1;
		  //printf("BGO using fit from timing = %d (0=daq, 1=fit)\n",TIGRESS_cal_par->use_time_fit_bgo);
		  //getc(stdin);
		}


	      //For RFUnwrapping
	      if(strcmp(str1,"RFUnwrapping_option")==0)
                {
                  if(strcmp(str2,"YES")==0)
                    TIGRESS_cal_par->DoRFUnwrapping=1;
                  if(strcmp(str2,"Yes")==0)
                    TIGRESS_cal_par->DoRFUnwrapping=1;
                  if(strcmp(str2,"yes")==0)
                    TIGRESS_cal_par->DoRFUnwrapping=1;
                  if(strcmp(str2,"y")==0)
                    TIGRESS_cal_par->DoRFUnwrapping=1;
                  if(strcmp(str2,"Y")==0)
                    TIGRESS_cal_par->DoRFUnwrapping=1;
                }

	      if(strcmp(str1,"TIGRESS_RFunwrapping_offset")==0)
                {
                  TIGRESS_cal_par->offset=atoi(str2);
                  printf("RFunwrapping TIGRESS_offset is %d\n",TIGRESS_cal_par->offset);
                }
	      
              if(strcmp(str1,"TIGRESS_RFunwrapping_shift")==0)
                {
                  TIGRESS_cal_par->shift=atoi(str2);
                  printf("RFunwrapping TIGRESS_shift is %d\n",TIGRESS_cal_par->shift);
                }
	      
              if(strcmp(str1,"TIGRESS_TTCal_gate_length")==0)
                {
                  TIGRESS_cal_par->TTCal_gate_length=atoi(str2);
                }

	      //for suppression
	      if(strcmp(str1,"Suppression_map")==0)
		read_TIGRESS_suppression_map(TIGRESS_cal_par,str2);
	    }
	}
    }
  else
    {
      printf("Wrong structure of file %s\n",name);
      printf("Aborting sort\n");
      exit(1);
    }    
  fclose(inp);
    
}
/***********************************************************************/
void read_TIGRESS_core_e_cal_par(TIGRESS_calibration_parameters *TIGRESS_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  pos,col,i;
  double a[3];

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nCore energy calibration parameters read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %d %lf %lf %lf",&pos,&col,&a[0],&a[1],&a[2])!=EOF)
	  if(pos>0&&pos<NPOSTIGR)
	    if(col>=0&&col<NCOL)
	      {
		TIGRESS_cal_par->ceflag[pos][col]=1;
		for(i=0;i<3;i++) TIGRESS_cal_par->ce[pos][col][i]=a[i];
	      }
    }
	    else
	    {
	      printf("Wrong structure of file %s\n",filename);
	      printf("Aborting sort\n");
	      exit(1);
	    }
  fclose(inp);
  
}
/***********************************************************************/
void read_TIGRESS_core_t_cal_par(TIGRESS_calibration_parameters *TIGRESS_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  pos,col,i;
  double a[2];

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nCore time calibration parameters read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %d %lf %lf",&pos,&col,&a[0],&a[1])!=EOF)
	  if(pos>0&&pos<NPOSTIGR)
	    if(col>=0&&col<NCOL)
	      {
		TIGRESS_cal_par->ctflag[pos][col]=1;
		for(i=0;i<2;i++) TIGRESS_cal_par->ct[pos][col][i]=a[i];
	      }
    }
	    else
	    {
	      printf("Wrong structure of file %s\n",filename);
	      printf("Aborting sort\n");
	      exit(1);
	    }
  fclose(inp);
  
}

/***********************************************************************/
void read_TIGRESS_ring_map(TIGRESS_calibration_parameters *TIGRESS_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  pos,col,i;
 

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nTIGRESS ring map read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %d %d",&pos,&col,&i)!=EOF)
	  if(pos>0&&pos<NPOSTIGR)
	    if(col>=0&&col<NCOL)
	      {
		TIGRESS_cal_par->ringflag[pos][col]=1;
		TIGRESS_cal_par->ring_map[pos][col]=i;
	      }
    }
  
  else
    {
      printf("Wrong structure of file %s\n",filename);
      printf("Aborting sort\n");
      exit(1);
    }
  fclose(inp);
}

/***********************************************************************/
void read_TIGRESS_suppression_map(TIGRESS_calibration_parameters *TIGRESS_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  tigPos,tigCol,bgoPos,bgoCol,bgoSup;
 

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nTIGRESS suppression map read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %d %d %d %d",&tigPos,&tigCol,&bgoPos,&bgoCol,&bgoSup)!=EOF)
	  if(tigPos>0 && tigPos<NPOSTIGR)
	    if(tigCol>=0 && tigCol<NCOL)
	      if(bgoPos>=0 && bgoPos<NPOSTIGR)
		if(bgoCol>=0 && bgoCol<NCOL)
		  if(bgoSup>=0 && bgoSup<NSUP)
		    {
		      TIGRESS_cal_par->sup_map[tigPos][tigCol][bgoPos][bgoCol][bgoSup]=1;
		      //printf("Got suppression for %d %d %d %d %d\n",tigPos,tigCol,bgoPos,bgoCol,bgoSup);
		    }
    }
  
  else
    {
      printf("Wrong structure of file %s\n",filename);
      printf("Aborting sort\n");
      exit(1);
    }

  fclose(inp);
}	       		   

/***********************************************************************/
void read_TIGRESS_core_energy_limits(TIGRESS_calibration_parameters *TIGRESS_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  pos,col;
  float low,high;
 

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nTIGRESS energy limits read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %d %f %f",&pos,&col,&low,&high)!=EOF)
	  if(pos>0&&pos<NPOSTIGR)
	    if(col>=0&&col<NCOL)
	      {
		TIGRESS_cal_par->celow[pos][col]=low;
		TIGRESS_cal_par->cehigh[pos][col]=high;
	      }
    }
	    else
	    {
	      printf("Wrong structure of file %s\n",filename);
	      printf("Aborting sort\n");
	      exit(1);
	    }
  fclose(inp);
  
}
/***********************************************************************/
void read_TIGRESS_core_time_limits(TIGRESS_calibration_parameters *TIGRESS_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  pos,col;
  float low,high;
 

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nTIGRESS time limits read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %d %f %f",&pos,&col,&low,&high)!=EOF)
	  if(pos>0&&pos<NPOSTIGR)
	    if(col>=0&&col<NCOL)
	      {
		TIGRESS_cal_par->ctlow[pos][col]=low;
		TIGRESS_cal_par->cthigh[pos][col]=high;
	      }
    }
	    else
	    {
	      printf("Wrong structure of file %s\n",filename);
	      printf("Aborting sort\n");
	      exit(1);
	    }
  fclose(inp);
  
}
/***********************************************************************/
void read_TIGRESS_detector_positions(TIGRESS_calibration_parameters *TIGRESS_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  pos,col;
  double x,y,z; 
  double R,theta,phi;

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nTIGRESS detector positions read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %d %lf %lf %lf",&pos,&col,&x,&y,&z)!=EOF)
	  if(pos>0&&pos<NPOSTIGR)
	    if(col>=0&&col<NCOL)
	    {
	      TIGRESS_cal_par->tposflag[pos][col]=1;

	      R = sqrt(x*x+y*y+z*z);
	      theta = acos(z/R);
	      phi = atan2(y,x);
	      
	      TIGRESS_cal_par->tpos[pos][col][0]=R;
	      TIGRESS_cal_par->tpos[pos][col][1]=theta;
	      TIGRESS_cal_par->tpos[pos][col][2]=phi;
        TIGRESS_cal_par->tpos_xyz[pos][col][0]=x;
	      TIGRESS_cal_par->tpos_xyz[pos][col][1]=y;
	      TIGRESS_cal_par->tpos_xyz[pos][col][2]=z;
	    }
    }
  else
    {
      printf("Wrong structure of file %s\n",filename);
      printf("Aborting sort\n");
      exit(1);
    }
  fclose(inp);
}
/*******************************************************************/
void read_TIGRESS_ring_energy_gates(TIGRESS_calibration_parameters *TIGRESS_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  ring;
  float low,high;
 

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nTIGRESS ring energy gate limits read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %f %f",&ring,&low,&high)!=EOF)
	  if(ring>0 && ring<NRING)
	      {
		TIGRESS_cal_par->relow[ring]=low;
		TIGRESS_cal_par->rehigh[ring]=high;
		//printf("ring %d elow %.2f ehigh %.2f\n",ring,TIGRESS_cal_par->relow[ring],TIGRESS_cal_par->rehigh[ring]);
		//getc(stdin);
	      }
    }
	    else
	    {
	      printf("Wrong structure of file %s\n",filename);
	      printf("Aborting sort\n");
	      exit(1);
	    }
  fclose(inp);
  
}
/*******************************************************************/
void summarize_TIGRESS_calibration(TIGRESS_calibration_parameters *TIGRESS_cal_par, char *filename)
{
  FILE *out;
  int  pos,col;



  if((out=fopen(filename,"w"))==NULL)
      {
         printf("\nFile %s can not be opened\n",filename);
         exit(EXIT_FAILURE);
      }

  fprintf(out,"TIGRESS core energy calibration parameters\n");
  fprintf(out,"  pos#  col#    a0             a1             a2\n");
  for(pos=1;pos<NPOSTIGR;pos++)
    for(col=0;col<NCOL;col++)
      if(TIGRESS_cal_par->ceflag[pos][col]==1)
	fprintf(out,"   %02d  %02d  %12.5e   %12.5e   %12.5e\n",pos,col,TIGRESS_cal_par->ce[pos][col][0],TIGRESS_cal_par->ce[pos][col][1],TIGRESS_cal_par->ce[pos][col][2]);
     
  fprintf(out,"TIGRESS core time calibration parameters\n");
  fprintf(out,"  pos#  col#    a0             a1 \n");
  for(pos=1;pos<NPOSTIGR;pos++)
    for(col=0;col<NCOL;col++)
      if(TIGRESS_cal_par->ctflag[pos][col]==1)
	fprintf(out,"   %02d  %02d  %12.5e   %12.5e\n",pos,col,TIGRESS_cal_par->ct[pos][col][0],TIGRESS_cal_par->ct[pos][col][1]);
  fclose(out);
    
}
/********************************************************************/
void calibrate_TIGRESS(raw_event *raw_event, TIGRESS_calibration_parameters *TIGRESS_cal_par, cTIGRESS *TIGRESS_cal_ev)
{

  long long int one=1;
  int pos,col,sup,addback_n;
  double ran,ren,e,t;
  double addback_e,addback_t,addback_max;
  int    addback_c,addback_r;
  //For RFUnwrapping
  double trf,func;
  long int a;
  int tigPos,tigCol,bgoPos,bgoCol,bgoSup;

   memset(TIGRESS_cal_ev,0,sizeof(cTIGRESS));

  /* TIGRESS core energy calibration */
  if(raw_event->tg.h.Gefold>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((raw_event->tg.h.GeHP&(one<<(pos-1)))!=0)
	if(raw_event->tg.det[pos].h.Gefold>0)
	  for(col=0;col<NCOL;col++)
	    if(TIGRESS_cal_par->ceflag[pos][col]==1)
	      if(((raw_event->tg.det[pos].h.GeHP)&(one<<col))!=0)
		if(raw_event->tg.det[pos].ge[col].h.Efold>0)
		  if((raw_event->tg.det[pos].ge[col].h.EHP&1)!=0)
		    {
		      ran=(double)rand()/(double)RAND_MAX-0.5;
		      ren=raw_event->tg.det[pos].ge[col].seg[0].charge+ran;
		      e=TIGRESS_cal_par->ce[pos][col][0]+TIGRESS_cal_par->ce[pos][col][1]*ren+TIGRESS_cal_par->ce[pos][col][2]*ren*ren;
		      if(e>0)
			if(e<S65K)
			  {
			    TIGRESS_cal_ev->det[pos].ge[col].seg[0].E=e;
			    TIGRESS_cal_ev->det[pos].ge[col].h.FE++;
			    TIGRESS_cal_ev->det[pos].ge[col].h.EHP|=1;
			    TIGRESS_cal_ev->det[pos].ge[col].ring=TIGRESS_cal_par->ring_map[pos][col];
			    TIGRESS_cal_ev->det[pos].hge.FE++;
			    TIGRESS_cal_ev->det[pos].hge.EHP|=(one<<col);
			    TIGRESS_cal_ev->h.FE++;
			    TIGRESS_cal_ev->h.EHP|=(one<<(pos-1));
			  }
		    }
  /* end TIGRESS core energy calibration */

  /* TIGRESS core time calibration from the DAQ CFD */
  if(TIGRESS_cal_par->use_time_fit_tgr==0)
      if(raw_event->tg.h.Gefold>0)
	for(pos=1;pos<NPOSTIGR;pos++)
	  if((raw_event->tg.h.GeHP&(one<<(pos-1)))!=0)
	    if(raw_event->tg.det[pos].h.Gefold>0)
	      for(col=0;col<NCOL;col++)
		if(TIGRESS_cal_par->ctflag[pos][col]==1)
		  if(((raw_event->tg.det[pos].h.GeHP)&(one<<col))!=0)
		    if(raw_event->tg.det[pos].ge[col].h.Tfold>0)
		      if((raw_event->tg.det[pos].ge[col].h.THP&1)!=0)
			{
			  /* t = tcfd-ts */
			  t=raw_event->tg.det[pos].ge[col].seg[0].cfd&0x00ffffff;
			  t-=(raw_event->tg.det[pos].ge[col].seg[0].timestamp*16)&0x00ffffff;
			  
			  //set RF time only if RF is in the hit pattern
			  if((raw_event->h.setupHP&RF_BIT)!=0)
			    {
			      trf=raw_event->rf.sin.t0;
			      if(TIGRESS_cal_par->DoRFUnwrapping==1)
				{
				  func=(1.000*trf)-(RFphase+TIGRESS_cal_par->offset);
				  a=fmod(t+TIGRESS_cal_par->shift,RFphase);
				  if(a>func) trf+=RFphase;
				}
			    }
			  //else set it to 0
			  else
			    trf=0.;
			  
			  /* overwrite rf time for test defined by CP */
			  //trf=0.;
			  
			  /* t = tcfd-ts-trf+offset */
			  t-=trf;
			  t+=S16K;

			  /* printf("traw: %d ts: %d trf: %f t-ts-trf+16K: %f\n",raw_event->tg.det[pos].ge[col].seg[0].cfd&0x00ffffff,(raw_event->tg.det[pos].ge[col].seg[0].timestamp*16)&0x00ffffff,trf,t); */
			  /* getc(stdin); */
			  
			  ran=(double)rand()/(double)RAND_MAX-0.5;
			  ren=t+ran;
			  t=TIGRESS_cal_par->ct[pos][col][0]+TIGRESS_cal_par->ct[pos][col][1]*ren;
			  
			  if(t>0)
			    if(t<S65K)
			      {
				TIGRESS_cal_ev->det[pos].ge[col].seg[0].T=t;
				TIGRESS_cal_ev->det[pos].ge[col].h.FT++;
				TIGRESS_cal_ev->det[pos].ge[col].h.THP|=1;
				TIGRESS_cal_ev->det[pos].ge[col].ring=TIGRESS_cal_par->ring_map[pos][col];
				TIGRESS_cal_ev->det[pos].hge.FT++;
				TIGRESS_cal_ev->det[pos].hge.THP|=(one<<col);
				TIGRESS_cal_ev->h.FT++;
				TIGRESS_cal_ev->h.THP|=(one<<(pos-1));
			      }			  
			}
  /* end TIGRESS core time calibration from the DAQ CFD */

  /* TIGRESS core time calibration from the fit */
  if(TIGRESS_cal_par->use_time_fit_tgr==1)	   
    if(raw_event->tg.h.Gefold>0)
      for(pos=1;pos<NPOSTIGR;pos++)
	if((raw_event->tg.h.GeHP&(one<<(pos-1)))!=0)
	  if(raw_event->tg.det[pos].h.Gefold>0)
	    for(col=0;col<NCOL;col++)
	      if(TIGRESS_cal_par->ctflag[pos][col]==1)
		if(((raw_event->tg.det[pos].h.GeHP)&(one<<col))!=0)
		  {
		    /* t=tfit*16 (in ADC units) */
		    t=raw_event->tg.det[pos].ge[col].t0[0]*16; 
		    //t=raw_event->tg.det[pos].ge[col].t0[0]; 
		    
		    //set RF time only if RF is in the hit pattern
		    if((raw_event->h.setupHP&RF_BIT)!=0)
		      {
			trf=raw_event->rf.sin.t0;
			
			if(TIGRESS_cal_par->DoRFUnwrapping==1)
			  {
			    func=(1.000*trf)-(RFphase+TIGRESS_cal_par->offset);
			    a=fmod(t+TIGRESS_cal_par->shift,RFphase);
			    if(a>func) trf+=RFphase;
			  }
		      }
		    //else set it to 0
		    else
		      trf=0.;
		    
		    /* t=tfit-trf+offset */
		    t-=trf;
		    t+=S16K;

		    /* printf("trigger %d\n",raw_event->h.trig_num&0x7fffffff); */
		    /* printf("tfit: %f traw: %f trf: %f t-trf+16K: %f\n",raw_event->tg.det[pos].ge[col].t0[0],raw_event->tg.det[pos].ge[col].t0[0]*16,trf,t); */
		    /* getc(stdin); */
		    
		    ran=(double)rand()/(double)RAND_MAX-0.5;
		    ren=t+ran;
		    /* printf("ran %f ren %f\n",ran,ren); */
		    t=TIGRESS_cal_par->ct[pos][col][0]+TIGRESS_cal_par->ct[pos][col][1]*ren;
		    /* printf("cal t %f\n",t); */
		    /* getc(stdin); */		    

		    if(t>0)
		      if(t<S65K)
			{
			  TIGRESS_cal_ev->det[pos].ge[col].seg[0].T=t;
			  TIGRESS_cal_ev->det[pos].ge[col].h.FT++;
			  TIGRESS_cal_ev->det[pos].ge[col].h.THP|=1;
			  TIGRESS_cal_ev->det[pos].ge[col].ring=TIGRESS_cal_par->ring_map[pos][col];
			  TIGRESS_cal_ev->det[pos].hge.FT++;
			  TIGRESS_cal_ev->det[pos].hge.THP|=(one<<col);
			  TIGRESS_cal_ev->h.FT++;
			  TIGRESS_cal_ev->h.THP|=(one<<(pos-1));
			}	
		  }
  /* end TIGRESS core time calibration from the waveform fit */

  /* TIGRESS energy+time hit patterns */
  if(TIGRESS_cal_ev->h.FE>0)
    if(TIGRESS_cal_ev->h.FT>0)
      for(pos=1;pos<NPOSTIGR;pos++)
	if((TIGRESS_cal_ev->h.EHP&(one<<(pos-1)))!=0)
	  if((TIGRESS_cal_ev->h.THP&(one<<(pos-1)))!=0)
	    if(TIGRESS_cal_ev->det[pos].hge.FE>0)
	      if(TIGRESS_cal_ev->det[pos].hge.FT>0)
		for(col=0;col<NCOL;col++)
		  if((TIGRESS_cal_ev->det[pos].hge.EHP&(one<<col))!=0)
		    if((TIGRESS_cal_ev->det[pos].hge.THP&(one<<col))!=0)
		      if(TIGRESS_cal_ev->det[pos].ge[col].h.FE>0)
			 if(TIGRESS_cal_ev->det[pos].ge[col].h.FT>0)
			    if((TIGRESS_cal_ev->det[pos].ge[col].h.EHP&1)!=0)
			      if((TIGRESS_cal_ev->det[pos].ge[col].h.THP&1)!=0)
				{
				  TIGRESS_cal_ev->det[pos].ge[col].h.HHP|=1;
				  TIGRESS_cal_ev->det[pos].ge[col].h.FH++;
				  TIGRESS_cal_ev->det[pos].hge.HHP|=(one<<col);
				  TIGRESS_cal_ev->det[pos].hge.FH++;
				  TIGRESS_cal_ev->h.HHP|=(one<<(pos-1));
				  TIGRESS_cal_ev->h.FH++;
				}
  /* end TIGRESS energy+time hit patterns */

  /* BGO timing from the DAQ cfd */
  if(TIGRESS_cal_par->use_time_fit_bgo==0)
  if(raw_event->tg.h.BGOfold>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((raw_event->tg.h.BGOHP&(1<<(pos-1)))!=0)
	if(raw_event->tg.det[pos].h.BGOfold>0)
	  for(col=0;col<NCOL;col++)
	    if(((raw_event->tg.det[pos].h.BGOHP)&(1<<col))!=0)
	      if(raw_event->tg.det[pos].bgo[col].h.Tfold>0)
		for(sup=0;sup<NSUP;sup++)
		  if(((raw_event->tg.det[pos].bgo[col].h.THP&(1<<sup))!=0))
		    {
		      t=raw_event->tg.det[pos].bgo[col].sup[sup].cfd&0x00ffffff;
		      t-=(raw_event->tg.det[pos].bgo[col].sup[sup].timestamp*16)&0x00ffffff;

		      //set RF time only if RF is in the hit pattern
		      if((raw_event->h.setupHP&RF_BIT)!=0)
			{
			  trf=raw_event->rf.sin.t0;

			  if(TIGRESS_cal_par->DoRFUnwrapping==1)
			    {
			      func=(1.000*trf)-(RFphase+TIGRESS_cal_par->offset);
			      a=fmod(t+TIGRESS_cal_par->shift,RFphase);
			      if(a>func)
			      	trf+=RFphase;
			    }
			}
		      //else set it to 0
		      else
			trf=0.;
		      
		      t-=trf;
		      t+=S16K;
		      
		      ran=(double)rand()/(double)RAND_MAX-0.5;
		      ren=t+ran;
		      t=0.625*ren;
		    
		      if(t>0)
			if(t<S65K)
			  {
				TIGRESS_cal_ev->det[pos].bgo[col].sup[sup].T=t;
				TIGRESS_cal_ev->det[pos].bgo[col].h.BFT++;
				TIGRESS_cal_ev->det[pos].bgo[col].h.BTHP|=(one<<sup);
				TIGRESS_cal_ev->det[pos].hbgo.BFT++;
				TIGRESS_cal_ev->det[pos].hbgo.BTHP|=(one<<col);
				TIGRESS_cal_ev->h.BFT++;
				TIGRESS_cal_ev->h.BTHP|=(one<<(pos-1));
			  }			  
		    }
  /* end BGO timing from the DAQ cfd */

  /* BGO timing from the waveform fit */
  if(TIGRESS_cal_par->use_time_fit_bgo==1)
  if(raw_event->tg.h.BGOfold>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((raw_event->tg.h.BGOHP&(1<<(pos-1)))!=0)
	if(raw_event->tg.det[pos].h.BGOfold>0)
	  for(col=0;col<NCOL;col++)
	    if(((raw_event->tg.det[pos].h.BGOHP)&(1<<col))!=0)
	      if(raw_event->tg.det[pos].bgo[col].h.Tfold>0)
		for(sup=0;sup<NSUP;sup++)
		  if(((raw_event->tg.det[pos].bgo[col].h.THP&(1<<sup))!=0))
		    {
		      //printf("pos %d col %d sup %d\n",pos,col,sup);
		      t=raw_event->tg.det[pos].bgo[col].t0[sup]*16;

		      //printf("raw time %f\n",t); 
		      
		      //set RF time only if RF is in the hit pattern
		      if((raw_event->h.setupHP&RF_BIT)!=0)
			{
			  trf=raw_event->rf.sin.t0;

			  if(TIGRESS_cal_par->DoRFUnwrapping==1)
			    {
			      func=(1.000*trf)-(RFphase+TIGRESS_cal_par->offset);
			      a=fmod(t+TIGRESS_cal_par->shift,RFphase);
			      if(a>func)
			      	trf+=RFphase;
			    }
			}
		      //else set it to 0
		      else
			trf=0.;
		      
		      t-=trf;
		      //printf("trf %f t-trf %f\n",trf,t); 
		      t+=S16K;
		      //printf("t-trf+S16K %f\n",t); 

		      
		      ran=(double)rand()/(double)RAND_MAX-0.5;
		      ren=t+ran;
		      t=0.625*ren;
		      //printf("calibrated time %f\n",t); 
		      //getc(stdin);
		    
		      if(t>0)
			if(t<S65K)
			  {
				TIGRESS_cal_ev->det[pos].bgo[col].sup[sup].T=t;
				TIGRESS_cal_ev->det[pos].bgo[col].h.BFT++;
				TIGRESS_cal_ev->det[pos].bgo[col].h.BTHP|=(one<<sup);
				TIGRESS_cal_ev->det[pos].hbgo.BFT++;
				TIGRESS_cal_ev->det[pos].hbgo.BTHP|=(one<<col);
				TIGRESS_cal_ev->h.BFT++;
				TIGRESS_cal_ev->h.BTHP|=(one<<(pos-1));
			  }			  
		    }
  /* end BGO timing from the waveform fit */
  
  /* add back */
  if(TIGRESS_cal_ev->h.FE>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((TIGRESS_cal_ev->h.EHP&(one<<(pos-1)))!=0)
  	if(TIGRESS_cal_ev->det[pos].hge.FE>0)
	  {
	    TIGRESS_cal_ev->det[pos].addbackHP=0;
	    addback_e=0.;
	    addback_t=0.;
	    addback_n=0;
	    addback_c=-1;
	    addback_r=-1;
	    addback_max=-1.;

	    for(col=0;col<NCOL;col++)
  	    if((TIGRESS_cal_ev->det[pos].hge.EHP&(one<<col))!=0)
  	      if(TIGRESS_cal_ev->det[pos].ge[col].h.FE>0)
  		if((TIGRESS_cal_ev->det[pos].ge[col].h.EHP&1)!=0)
  		  {
  		    e=TIGRESS_cal_ev->det[pos].ge[col].seg[0].E;
		    if(e>TIGRESS_cal_par->celow[pos][col])
		      if(e<TIGRESS_cal_par->cehigh[pos][col])
			{
			  t=TIGRESS_cal_ev->det[pos].ge[col].seg[0].T;
			  if(t>TIGRESS_cal_par->ctlow[pos][col])
			    if(t<TIGRESS_cal_par->cthigh[pos][col])
			      {
				//bitwise OR assignment x|=y -> x = x|y
				TIGRESS_cal_ev->det[pos].addbackHP|=(one<<col);
				addback_e+=e;
				addback_t+=t;
				addback_n++;
				if(e>addback_max)
				  {
				    addback_c=col;
				    addback_r=TIGRESS_cal_ev->det[pos].ge[col].ring;
				    addback_max=e;
				  }
			      }
	
			}
  		  }
	    if(addback_e!=0)
	      {
		//addback time set as average
		addback_t/=addback_n;
		TIGRESS_cal_ev->det[pos].addback.E=addback_e;
		TIGRESS_cal_ev->det[pos].addback.T=addback_t;
		TIGRESS_cal_ev->det[pos].addbackF=addback_n;
		TIGRESS_cal_ev->det[pos].addbackC=addback_c;
		TIGRESS_cal_ev->det[pos].addbackR=addback_r;
		//bitwise OR assignment x|=y -> x = x|y
		TIGRESS_cal_ev->h.AHP|=(one<<(pos-1));
		TIGRESS_cal_ev->h.FA++;
	      }
	  }
  /* end add back */

  /* The idea for the new suppression scheme: BGO "at the right position" comes from the map
     If there is a BGO event at the right position for a given crystal, set the suppression flag for
     that HPGe. This loop structure is a bit counterintuitive: the Tigress loop is first to avoid 
     resetting the suppression flag once it has been set to 1. There are other ways to do this. */
  double tbgo=0.;
  double ttg=0.;
  double tdiff=0.;
  
  //initialize suppression flag to 0
  for(tigPos=1;tigPos<NPOSTIGR;tigPos++)
    for(tigCol=0;tigCol<NCOL;tigCol++)
      {
  	TIGRESS_cal_ev->det[tigPos].ge[tigCol].suppress=0;
      }
  
  //check TIGRESS hit fold
  if(TIGRESS_cal_ev->h.FT>0)
    //check BGO hit fold
    if(TIGRESS_cal_ev->h.BFT>0)
      //TIGRESS part - loop through positions, cores; check associated hit patterns
      for(tigPos=1;tigPos<NPOSTIGR;tigPos++)
	if((TIGRESS_cal_ev->h.THP&(1<<(tigPos-1)))!=0)
	  if(TIGRESS_cal_ev->det[tigPos].hge.FT>0)
	    for(tigCol=0;tigCol<NCOL;tigCol++)
	      if((TIGRESS_cal_ev->det[tigPos].hge.THP&(1<<tigCol))!=0)
		if(TIGRESS_cal_ev->det[tigPos].ge[tigCol].h.FT>0)
		  if((TIGRESS_cal_ev->det[tigPos].ge[tigCol].h.THP&1)!=0)
		    //BGO part - loop through positions, cores; check associated hit patterns
		    for(bgoPos=1;bgoPos<NPOSTIGR;bgoPos++)
		      if((TIGRESS_cal_ev->h.BTHP&(1<<(bgoPos-1)))!=0)
			if(TIGRESS_cal_ev->det[bgoPos].hbgo.BFT>0)
			  for(bgoCol=0;bgoCol<NCOL;bgoCol++)
			    if((TIGRESS_cal_ev->det[bgoPos].hbgo.BTHP&(1<<bgoCol))!=0)
			      if(TIGRESS_cal_ev->det[bgoPos].bgo[bgoCol].h.BFT>0)
				for(bgoSup=0;bgoSup<NSUP;bgoSup++)
				  if((TIGRESS_cal_ev->det[bgoPos].bgo[bgoCol].h.BTHP&(1<<bgoSup))!=0)
				    {
				      //printf("BGO suppressor %d col %d sup %d hit\n",bgoPos,bgoCol,bgoSup);
				      //getc(stdin);
				      
				      //if BGO in the suppression map
				      if(TIGRESS_cal_par->sup_map[tigPos][tigCol][bgoPos][bgoCol][bgoSup]==1)
					{
					  tbgo=TIGRESS_cal_ev->det[bgoPos].bgo[bgoCol].sup[bgoSup].T/TIGRESS_cal_par->contr_t;
					  //printf("bgo cfd: %0.f\n",tbgo);
					  //tbgo-=(raw_event->tg.det[bgoPos].bgo[bgoCol].sup[bgoSup].timestamp*16)&0x00ffffff;
					  //printf("bgo cfd-timestamp: %0.f\n",tbgo);
					  
					  ttg=TIGRESS_cal_ev->det[tigPos].ge[tigCol].seg[0].T/TIGRESS_cal_par->contr_t;
					  //printf("tigress cfd: %0.f\n",ttg);
					  //ttg-=(raw_event->tg.det[tigPos].ge[tigCol].seg[0].timestamp*16)&0x00ffffff;
					  //printf("tigress cfd-timestamp: %0.f\n",ttg);
					  
					  tdiff = ttg-tbgo;
					  tdiff+=S4K;
					  //tdiff/=16;
					  
					  if(tdiff>=0 && tdiff<S32K)
					    TIGRESS_cal_ev->det[tigPos].ge[tigCol].suppress=tdiff;
					  
					  //printf("events at HPGe pos %d and col %d suppressed with %f\n",tigPos,tigCol,TIGRESS_cal_ev->det[tigPos].ge[tigCol].suppress);
					  //getc(stdin);
					  
					  /* if(TIGRESS_cal_ev->det[tigPos].ge[tigCol].suppress<=0 || TIGRESS_cal_ev->det[tigPos].ge[tigCol].suppress >=S32K) */
					  /*   { */
					  /*     printf("events at HPGe pos %d and col %d suppressed with %f\n",tigPos,tigCol,TIGRESS_cal_ev->det[tigPos].ge[tigCol].suppress); */
					  /*     getc(stdin); */
					  /*   } */
					  
					}
				    }
  
  
//DEFAULT SUPPRESSION METHOD
  /* for(pos=1;pos<NPOSTIGR;pos++) */
  /*   { */
  /*     TIGRESS_cal_ev->det[pos].suppress=0; */
  /*     if(raw_event->tg.h.BGOfold>0) /\* BGO in the event *\/ */
  /* 	if((raw_event->tg.h.BGOHP&(one<<(pos-1)))!=0) /\* BGO at the right position in the event *\/ */
  /* 	  if(raw_event->tg.det[pos].h.BGOfold>0) /\* BGO at the right position in the event *\/ */
  /* 	    TIGRESS_cal_ev->det[pos].suppress=1; */
  /*   } */
}
