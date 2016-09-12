#include "SFU-cal-S3.h"
/*******************************************************************/
/*           RINGS GO FIRST                                        */
/*******************************************************************/
void initialize_S3ring_calibration(S3ring_calibration_parameters *S3ring_cal_par, char *name)
{
  FILE *inp;
  char str1[256],str2[256];

  memset(S3ring_cal_par,0,sizeof(S3ring_calibration_parameters));
  S3ring_cal_par->contr_e=1.;
  S3ring_cal_par->contr_t=1.;

  if((inp=fopen(name,"r"))==NULL)
      {
         printf("\nFile %s can not be opened\n",name);
         exit(EXIT_FAILURE);
      }
  else
    printf("S3 calibration parameters read from file %s\n",name);

 
  if(fgets(str1,256,inp)!=NULL)
    {
      if(fgets(str1,256,inp)!=NULL)
	{
	  while(fscanf(inp,"%s %s",str1,str2)!=EOF)
	    {
	      if(strcmp(str1,"Energy_spectra_contraction_factor")==0)
		S3ring_cal_par->contr_e=atof(str2);
	      
	      
	      if(strcmp(str1,"Time_spectra_contraction_factor")==0)
		S3ring_cal_par->contr_t=atof(str2);
	      
	      if(strcmp(str1,"Energy_calibration_parameters")==0)
		read_S3ring_e_cal_par(S3ring_cal_par,str2);
	      
	      if(strcmp(str1,"Time_calibration_parameters")==0)
		read_S3ring_t_cal_par(S3ring_cal_par,str2);

	      if(strcmp(str1,"Energy_limits")==0)
		read_S3ring_energy_limits(S3ring_cal_par,str2);

	      if(strcmp(str1,"Time_limits")==0)
		read_S3ring_time_limits(S3ring_cal_par,str2);	      
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
void read_S3ring_e_cal_par(S3ring_calibration_parameters *S3ring_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  ring,i;
  double a[2];

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nS3 ring energy calibration parameters read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %lf %lf",&ring,&a[0],&a[1])!=EOF)
	  if(ring>0&&ring<NS3RING)
	    {
	      S3ring_cal_par->ceflag[ring]=1;
	      for(i=0;i<2;i++) S3ring_cal_par->ce[ring][i]=a[i];
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
void read_S3ring_t_cal_par(S3ring_calibration_parameters *S3ring_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  ring,i;
  double a[2];

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nS3 ring time calibration parameters read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %lf %lf",&ring,&a[0],&a[1])!=EOF)
	  if(ring>0&&ring<NS3RING)
	    {
	      S3ring_cal_par->ctflag[ring]=1;
	      for(i=0;i<2;i++) S3ring_cal_par->ct[ring][i]=a[i];
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
void read_S3ring_energy_limits(S3ring_calibration_parameters *S3ring_cal_par, char *filename)
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
  printf("\nS3 ring energy limits read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %f %f",&ring,&low,&high)!=EOF)
	  if(ring>0&&ring<NS3RING)
	      {
		S3ring_cal_par->elow[ring]=low;
		S3ring_cal_par->ehigh[ring]=high;
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
void read_S3ring_time_limits(S3ring_calibration_parameters *S3ring_cal_par, char *filename)
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
  printf("\nS3 ring time limits read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %f %f",&ring,&low,&high)!=EOF)
	  if(ring>0&&ring<NS3RING)
	      {
		S3ring_cal_par->tlow[ring]=low;
		S3ring_cal_par->thigh[ring]=high;
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
void summarize_S3ring_calibration(S3ring_calibration_parameters *S3ring_cal_par, char *filename)
{
  FILE *out;
  int  ring;



  if((out=fopen(filename,"w"))==NULL)
      {
         printf("\nFile %s can not be opened\n",filename);
         exit(EXIT_FAILURE);
      }

  fprintf(out,"S3 ring energy calibration parameters\n");
  fprintf(out," ring#       a0             a1 \n");
  for(ring=1;ring<NS3RING;ring++)
      if(S3ring_cal_par->ceflag[ring]==1)
	fprintf(out,"   %02d  %12.5e   %12.5e\n",ring,S3ring_cal_par->ce[ring][0],S3ring_cal_par->ce[ring][1]);
     
  fprintf(out,"S3 ring time calibration parameters\n");
  fprintf(out,"  ring#  a0             a1 \n");
  for(ring=1;ring<NS3RING;ring++)
    if(S3ring_cal_par->ctflag[ring]==1)
      fprintf(out,"   %02d  %12.5e   %12.5e\n",ring,S3ring_cal_par->ct[ring][0],S3ring_cal_par->ct[ring][1]);
  fclose(out);
    
}
/********************************************************************/
void calibrate_S3ring(raw_event* rev, S3ring_calibration_parameters *S3ring_cal_par, cS3 *cp)
{
  unsigned long long int one=1; 
  int ring;
  double ran,ren,e,t;
  memset(&cp->rh,0,sizeof(lheader));
  memset(&cp->ring,0,NS3RING*sizeof(hit));
  /* energy calibration */
  if(rev->s3.rh.Efold>0)
    for(ring=1;ring<NS3RING;ring++)
      if(S3ring_cal_par->ceflag[ring]==1)
	if((rev->s3.rh.EHP&(one<<ring))!=0)
	  if(rev->s3.ring[ring].charge>0)
  	    {
  	      ran=(double)rand()/(double)RAND_MAX-0.5;
  	      ren=rev->s3.ring[ring].charge+ran;
  	      e=S3ring_cal_par->ce[ring][0]+S3ring_cal_par->ce[ring][1]*ren;
	      if(e>0)
		if(e<S65K)
		  {
		    cp->ring[ring].E=e;
		    cp->rh.EHP|=(one<<ring);
		    cp->rh.FE++;
		  }
 	    }

  /* time calibration */
  if(rev->s3.rh.Tfold>0)
    for(ring=1;ring<NS3RING;ring++)
      if(S3ring_cal_par->ctflag[ring]==1)
  	if((rev->s3.rh.THP&(one<<ring))!=0)
  	  if(rev->s3.ring[ring].cfd>0)
  	    {
  	      t=rev->s3.ring[ring].cfd&0x00ffffff;
  	      t-=(rev->s3.ring[ring].timestamp*16)&0x00ffffff;
  	      if((rev->h.setupHP&RF_BIT)!=0)
  		{
  		  t-=rev->rf.sin.t0;
  		  t+=S16K;
  		  ran=(double)rand()/(double)RAND_MAX-0.5;
  		  ren=t+ran;

  		  t=S3ring_cal_par->ct[ring][0]+S3ring_cal_par->ct[ring][1]*ren;
  		  if(t>0)
  		    if(t<S65K)
  		      {
  			cp->ring[ring].T=t;
  			cp->rh.THP|=(one<<ring);
  			cp->rh.FT++;
  		      }
  		}
  	  }

  if(cp->rh.FE>0)
    if(cp->rh.FT>0)
      for(ring=1;ring<NS3RING;ring++)
  	if((cp->rh.EHP&(one<<ring))!=0)
  	  if((cp->rh.THP&(one<<ring))!=0)
  	    {
  	      cp->rh.HHP|=(one<<ring);
  	      cp->rh.FH++;
  	    }
}
/*******************************************************************/
/*           SECTORS GO NEXT                                       */
/*******************************************************************/
void initialize_S3sec_calibration(S3sec_calibration_parameters *S3sec_cal_par, char *name)
{
  FILE *inp;
  char str1[256],str2[256];

  memset(S3sec_cal_par,0,sizeof(S3sec_calibration_parameters));
  S3sec_cal_par->contr_e=1.;
  S3sec_cal_par->contr_t=1.;

  if((inp=fopen(name,"r"))==NULL)
      {
         printf("\nFile %s can not be opened\n",name);
         exit(EXIT_FAILURE);
      }
  else
    printf("S3 calibration parameters read from file %s\n",name);

 
  if(fgets(str1,256,inp)!=NULL)
    {
      if(fgets(str1,256,inp)!=NULL)
	{
	  while(fscanf(inp,"%s %s",str1,str2)!=EOF)
	    {
	      if(strcmp(str1,"Energy_spectra_contraction_factor")==0)
		S3sec_cal_par->contr_e=atof(str2);
	      
	      
	      if(strcmp(str1,"Time_spectra_contraction_factor")==0)
		S3sec_cal_par->contr_t=atof(str2);
	      
	      if(strcmp(str1,"Energy_calibration_parameters")==0)
		read_S3sec_e_cal_par(S3sec_cal_par,str2);
	      
	      if(strcmp(str1,"Time_calibration_parameters")==0)
		read_S3sec_t_cal_par(S3sec_cal_par,str2);

	      if(strcmp(str1,"Energy_limits")==0)
		read_S3sec_energy_limits(S3sec_cal_par,str2);

	      if(strcmp(str1,"Time_limits")==0)
		read_S3sec_time_limits(S3sec_cal_par,str2);	      
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
void read_S3sec_e_cal_par(S3sec_calibration_parameters *S3sec_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  sec,i;
  double a[2];

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nS3 sector energy calibration parameters read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %lf %lf",&sec,&a[0],&a[1])!=EOF)
	  if(sec>0&&sec<NS3SEC)
	    {
	      S3sec_cal_par->ceflag[sec]=1;
	      for(i=0;i<2;i++) S3sec_cal_par->ce[sec][i]=a[i];
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
void read_S3sec_t_cal_par(S3sec_calibration_parameters *S3sec_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  sec,i;
  double a[2];

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nS3 sector time calibration parameters read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %lf %lf",&sec,&a[0],&a[1])!=EOF)
	  if(sec>0&&sec<NS3SEC)
	    {
	      S3sec_cal_par->ctflag[sec]=1;
	      for(i=0;i<2;i++) S3sec_cal_par->ct[sec][i]=a[i];
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
void read_S3sec_energy_limits(S3sec_calibration_parameters *S3sec_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  sec;
  float low,high;
 

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nS3 sector energy limits read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %f %f",&sec,&low,&high)!=EOF)
	  if(sec>0&&sec<NS3SEC)
	      {
		S3sec_cal_par->elow[sec]=low;
		S3sec_cal_par->ehigh[sec]=high;
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
void read_S3sec_time_limits(S3sec_calibration_parameters *S3sec_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int  sec;
  float low,high;
 

  if((inp=fopen(filename,"r"))==NULL)
      {
         printf("\nI can't open file %s\n",filename);
         exit(EXIT_FAILURE);
      }
  printf("\nS3 sector time limits read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %f %f",&sec,&low,&high)!=EOF)
	  if(sec>0&&sec<NS3SEC)
	      {
		S3sec_cal_par->tlow[sec]=low;
		S3sec_cal_par->thigh[sec]=high;
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
void summarize_S3sec_calibration(S3sec_calibration_parameters *S3sec_cal_par, char *filename)
{
  FILE *out;
  int  sec;



  if((out=fopen(filename,"w"))==NULL)
      {
         printf("\nFile %s can not be opened\n",filename);
         exit(EXIT_FAILURE);
      }

  fprintf(out,"S3 sector energy calibration parameters\n");
  fprintf(out,"  sec#       a0             a1 \n");
  for(sec=1;sec<NS3SEC;sec++)
      if(S3sec_cal_par->ceflag[sec]==1)
	fprintf(out,"   %02d  %12.5e   %12.5e\n",sec,S3sec_cal_par->ce[sec][0],S3sec_cal_par->ce[sec][1]);
     
  fprintf(out,"S3 sector time calibration parameters\n");
  fprintf(out,"  sec#  a0             a1 \n");
  for(sec=1;sec<NS3SEC;sec++)
    if(S3sec_cal_par->ctflag[sec]==1)
      fprintf(out,"   %02d  %12.5e   %12.5e\n",sec,S3sec_cal_par->ct[sec][0],S3sec_cal_par->ct[sec][1]);
  fclose(out);
    
}
/********************************************************************/
void calibrate_S3sec(raw_event* rev, S3sec_calibration_parameters *S3sec_cal_par, cS3 *cp)
{
  unsigned long long int one=1; 
  int sec;
  double ran,ren,e,t;
  memset(&cp->sh,0,sizeof(lheader));
  memset(&cp->sec,0,NS3SEC*sizeof(hit));
  /* energy calibration */
  if(rev->s3.sh.Efold>0)
    for(sec=1;sec<NS3SEC;sec++)
      if(S3sec_cal_par->ceflag[sec]==1)
	if((rev->s3.sh.EHP&(one<<sec))!=0)
	  if(rev->s3.sec[sec].charge>0)
  	    {
  	      ran=(double)rand()/(double)RAND_MAX-0.5;
  	      ren=rev->s3.sec[sec].charge+ran;
  	      e=S3sec_cal_par->ce[sec][0]+S3sec_cal_par->ce[sec][1]*ren;
	      if(e>0)
		if(e<S65K)
		  {
		    cp->sec[sec].E=e;
		    cp->sh.EHP|=(one<<sec);
		    cp->sh.FE++;
		  }
 	    }

  /* time calibration */
  if(rev->s3.sh.Tfold>0)
    for(sec=1;sec<NS3SEC;sec++)
      if(S3sec_cal_par->ctflag[sec]==1)
  	if((rev->s3.sh.THP&(one<<sec))!=0)
  	  if(rev->s3.sec[sec].cfd>0)
  	    {
  	      t=rev->s3.sec[sec].cfd&0x00ffffff;
  	      t-=(rev->s3.sec[sec].timestamp*16)&0x00ffffff;
  	      if((rev->h.setupHP&RF_BIT)!=0)
  		{
  		  t-=rev->rf.sin.t0;
  		  t+=S16K;
  		  ran=(double)rand()/(double)RAND_MAX-0.5;
  		  ren=t+ran;

  		  t=S3sec_cal_par->ct[sec][0]+S3sec_cal_par->ct[sec][1]*ren;
  		  if(t>0)
  		    if(t<S65K)
  		      {
  			cp->sec[sec].T=t;
  			cp->sh.THP|=(one<<sec);
  			cp->sh.FT++;
  		      }
  		}
  	  }

  if(cp->sh.FE>0)
    if(cp->sh.FT>0)
      for(sec=1;sec<NS3SEC;sec++)
  	if((cp->sh.EHP&(one<<sec))!=0)
  	  if((cp->sh.THP&(one<<sec))!=0)
  	    {
  	      cp->sh.HHP|=(one<<sec);
  	      cp->sh.FH++;
  	    }
}
