#include "SFU-cal-PINARRAY.h"
/*******************************************************************/
void initialize_PINARRAY_calibration(PINARRAY_calibration_parameters *PINARRAY_cal_par, char *name)
{
  FILE *inp;
  char str1[256],str2[256];
  
  memset(PINARRAY_cal_par,0,sizeof(*PINARRAY_cal_par));
  PINARRAY_cal_par->contr_e=1.;
  PINARRAY_cal_par->contr_t=1.;
  
  if((inp=fopen(name,"r"))==NULL)
    {
      printf("\nFile %s can not be opened\n",name);
      exit(EXIT_FAILURE);
    }
  else
    printf("PINARRAY calibration parameters read from file %s\n",name);
  
  if(fgets(str1,256,inp)!=NULL)
    {
      if(fgets(str1,256,inp)!=NULL)
	{
	  while(fscanf(inp,"%s %s",str1,str2)!=EOF)
	    {
	      if(strcmp(str1,"Energy_spectra_contraction_factor")==0)
		PINARRAY_cal_par->contr_e=atof(str2);
	      if(strcmp(str1,"Time_spectra_contraction_factor")==0)
		PINARRAY_cal_par->contr_t=atof(str2);
	      if(strcmp(str1,"Energy_calibration_parameters")==0)
		read_PINARRAY_e_cal_par(PINARRAY_cal_par,str2);
	      if(strcmp(str1,"Time_calibration_parameters")==0)
		read_PINARRAY_t_cal_par(PINARRAY_cal_par,str2);
	      if(strcmp(str1,"PINArray_RFunwrapping_offset")==0)
		{
		  PINARRAY_cal_par->offset=atoi(str2);
		  printf("RFunwrapping offset is %d\n",PINARRAY_cal_par->offset);
		}
	      if(strcmp(str1,"PINArray_RFunwrapping_shift")==0)
		{
		  PINARRAY_cal_par->shift=atoi(str2);
		  printf("RFunwrapping shiftt is %d\n",PINARRAY_cal_par->shift);
		}
	      if(strcmp(str1,"Energy_limits")==0)
		read_PINARRAY_energy_limits(PINARRAY_cal_par,str2);
	      if(strcmp(str1,"Time_limits")==0)
		read_PINARRAY_time_limits(PINARRAY_cal_par,str2);	      
	      if(strcmp(str1,"Ring_map")==0)
                read_PINARRAY_ring_map(PINARRAY_cal_par,str2);
	    }}}
  else
    {
      printf("Wrong structure of file %s\n",name);
      printf("Aborting sort\n");
      exit(1);
    }    
  fclose(inp); 
}
/***********************************************************************/
void read_PINARRAY_e_cal_par(PINARRAY_calibration_parameters *PINARRAY_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  pos,i;
  double a[2];

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nPINARRAY energy calibration parameters read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %lf %lf",&pos,&a[0],&a[1])!=EOF)
	  if(pos>0&&pos<NPIN)
	    {
	      PINARRAY_cal_par->ceflag[pos]=1;
	      for(i=0;i<2;i++) PINARRAY_cal_par->ce[pos][i]=a[i];
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
void read_PINARRAY_t_cal_par(PINARRAY_calibration_parameters *PINARRAY_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  pos,i;
  double a[2];

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nPINARRAY time calibration parameters read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %lf %lf",&pos,&a[0],&a[1])!=EOF)
	  if(pos>0&&pos<NPIN)
	    {
	      PINARRAY_cal_par->ctflag[pos]=1;
	      for(i=0;i<2;i++) PINARRAY_cal_par->ct[pos][i]=a[i];
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
void read_PINARRAY_energy_limits(PINARRAY_calibration_parameters *PINARRAY_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  pos;
  float low,high;
 

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nPINARRAY energy limits read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %f %f",&pos,&low,&high)!=EOF)
	  if(pos>0&&pos<NPIN)
	      {
		PINARRAY_cal_par->elow[pos]=low;
		PINARRAY_cal_par->ehigh[pos]=high;
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
void read_PINARRAY_time_limits(PINARRAY_calibration_parameters *PINARRAY_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  pos;
  float low,high;
 

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nPINARRAY time limits read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %f %f",&pos,&low,&high)!=EOF)
	  if(pos>0&&pos<NPIN)
	      {
		PINARRAY_cal_par->tlow[pos]=low;
		PINARRAY_cal_par->thigh[pos]=high;
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
void read_PINARRAY_ring_map(PINARRAY_calibration_parameters *PINARRAY_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  pos,i;


  if((inp=fopen(filename,"r"))==NULL)
    {
      printf("\nI can't open file %s\n",filename);
      exit(EXIT_FAILURE);
    }
  printf("\nPINARRAY ring map read from the file:\n %s\n",filename);
  
  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
        while(fscanf(inp,"%d %d",&pos,&i)!=EOF)
          if(pos>0&&pos<NPIN)
	    {
	      PINARRAY_cal_par->ringflag[pos]=1;
	      PINARRAY_cal_par->ring_map[pos]=i;
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
/********************************************************************/
void summarize_PINARRAY_calibration(PINARRAY_calibration_parameters *PINARRAY_cal_par, char *filename)
{
  FILE *out;
  int  pos;



  if((out=fopen(filename,"w"))==NULL)
      {
         printf("\nFile %s can not be opened\n",filename);
         exit(EXIT_FAILURE);
      }

  fprintf(out,"PINARRAY energy calibration parameters\n");
  fprintf(out,"  pos#       a0             a1 \n");
  for(pos=1;pos<NPIN;pos++)
      if(PINARRAY_cal_par->ceflag[pos]==1)
	fprintf(out,"   %02d  %12.5e   %12.5e\n",pos,PINARRAY_cal_par->ce[pos][0],PINARRAY_cal_par->ce[pos][1]);
     
  fprintf(out,"PINARRAY time calibration parameters\n");
  fprintf(out,"  pos#  a0             a1 \n");
  for(pos=1;pos<NPIN;pos++)
    if(PINARRAY_cal_par->ctflag[pos]==1)
      fprintf(out,"   %02d  %12.5e   %12.5e\n",pos,PINARRAY_cal_par->ct[pos][0],PINARRAY_cal_par->ct[pos][1]);
  fclose(out);
    
}
/********************************************************************/
void calibrate_PINARRAY(raw_event* rev, PINARRAY_calibration_parameters *PINARRAY_cal_par, cPINARRAY *cp)
{
  unsigned long long int one=1; 
  int pos;
  double trf,func;
  double ran,ren,e,t;
  long int a;

  memset(cp,0,sizeof(cPINARRAY));

  /* energy calibration */
  if(rev->pinarray.h.Efold>0)
    for(pos=1;pos<NPIN;pos++)
      if(PINARRAY_cal_par->ceflag[pos]==1)
	if((rev->pinarray.h.EHP&(one<<pos))!=0)
	  if(rev->pinarray.pin[pos].charge>0)
  	    {
  	      ran=(double)rand()/(double)RAND_MAX-0.5;
  	      ren=rev->pinarray.pin[pos].charge+ran;
  	      e=PINARRAY_cal_par->ce[pos][0]+PINARRAY_cal_par->ce[pos][1]*ren;
	      if((e>=0) && (e<4*S32K))
		{
		  cp->pin[pos].E=e;
		  cp->ring=PINARRAY_cal_par->ring_map[pos];
		  cp->h.EHP|=(one<<pos);
		  cp->h.FE++;
		}
	      else if(e<0) printf("Warning! Calibrated PIN energy is less than 0!\n");
	      else         printf("Warning! Calibrated PIN energy is greater than 4*S23K! Raw energy was %f.\n",(ren-ran));
	    }
  
  /* time calibration */
  if(rev->pinarray.h.Tfold>0)
    for(pos=1;pos<NPIN;pos++)
      if(PINARRAY_cal_par->ctflag[pos]==1)
	if((rev->pinarray.h.THP&(one<<pos))!=0)
	  if(rev->pinarray.pin[pos].cfd>0)
	    {
	      t=rev->pinarray.pin[pos].cfd&0x00ffffff;
	      t-=(rev->pinarray.pin[pos].timestamp*16)&0x00ffffff;
	      if((rev->h.setupHP&RF_BIT)!=0)
		{
		  trf=rev->rf.sin.t0;
		  
		  func=(1.000*trf)-(RFphase+PINARRAY_cal_par->offset);
		  a=fmod(t+PINARRAY_cal_par->shift,RFphase);
		  
		  if(a>func) trf+=RFphase;
		  
		  t-=trf;
		  t+=S16K;
		    ran=(double)rand()/(double)RAND_MAX-0.5;
		  ren=t+ran;

		  t=PINARRAY_cal_par->ct[pos][0]+PINARRAY_cal_par->ct[pos][1]*ren;
		  if((t>0) && (t<S65K))
		    {
		      cp->pin[pos].T=t;
		      cp->ring=PINARRAY_cal_par->ring_map[pos];
		      cp->h.THP|=(one<<pos);
		      cp->h.FT++;
		    }}}

  if((cp->h.FE>0) && (cp->h.FT>0))
    for(pos=1;pos<NPIN;pos++)
      if(((cp->h.EHP&(one<<pos))!=0) && ((cp->h.THP&(one<<pos))!=0))
	{
	  cp->h.HHP|=(one<<pos);
	  cp->h.FH++;
	}}
