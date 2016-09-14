#include "SpecBS.h"
/* SpecBS */

/* autobkgnd and bg2 extracted from radware source code and modified slightly.        */
/* background subtraction procedure from Starosta et al., NIMA 515 (2003) 771.        */
/*====================================================================================*/
int main(int argc, char *argv[])
{
  char *tproj_name,*gated_name,*ringGates_name;
  FILE *ftproj,*fgated,*sbg_output,*tbg_output,*final_output;
  char mcaFile[132];
  int i,j;
  double N,Nb,Np,Tp,Tg;
 
  memset(tproj,0,sizeof(tproj));
  memset(gated,0,sizeof(gated));
  memset(sbg,0,sizeof(sbg));
  memset(tbg,0,sizeof(tbg));
  memset(dtproj,0,sizeof(dtproj));
  memset(dgated,0,sizeof(dgated));
  memset(dbg,0,sizeof(dbg));
  memset(final,0,sizeof(final));
  memset(tmp_bg,0,sizeof(tmp_bg));

  //here is how you use the program
  if(argc!=6)
    {
      printf("\nProgram sorts a background subtracted spectra for DSAM analysis.\nAll files should be in .mca format, separated by ring, and have dim = 32K.\n\nINPUT:\n1) Total projection file name (.mca)\n2) Single energy gated file name (.mca)\n3) TIGRESS ring energy gate limits file\n4) Limits for background subtraction\n\nOUTPUT:\n1) Background subtracted spectra for DSAM lifetime analysis (.mca)\n2) Total background spectra calculated from the energy gated spectra, separated by ring (.mca)\n3) Smooth background spectra calculated from the total projection, separated by ring (.mca)\n\n");
      printf("usage:\nSpecBS total_proj_name single_gated_name ring_energy_gates bLow bHigh\n\n");
      exit(-1);
    }
  
  tproj_name=argv[1];
  gated_name=argv[2];
  ringGates_name=argv[3];
  bLow=atof(argv[4]);
  bHigh=atof(argv[5]);
  
  /* peak width estimates taken from radware
     fwhm = sqrt(F*F+G*G*x+H*H*x*x), x=ch/1000, swpars are 
     squared values of F, G, H for 0, 1, 2 respectively. */
  swpars[0]=9.;
  swpars[1]=4./1000.;
  swpars[2]=0.;
  
  //get the ring energy gate limits
  read_TIGRESS_ring_energy_gates(ringGates_name);
  
  //open the total projection file
  ftproj = fopen(tproj_name, "r");
  if (ftproj == NULL)
    {
      printf("\n*** Could not open total projection spectrum file for reading ***\n");
      exit(1);
    }
  fread(tproj,1,2*NRING*S32K*sizeof(int),ftproj);
  fclose(ftproj);
  
  //open the single gated file
  fgated = fopen(gated_name, "r");
  if (fgated == NULL)
    {
      printf("\n*** Could not open single energy gated spectrum file for reading ***\n");
      exit(1);
    }
  fread(gated,1,2*NRING*S32K*sizeof(int),fgated);
  fclose(fgated);
  
  /* calculate normalization factors */
  N=0.;
  Nb=0.;
  Np=0.;
  Tp=0.;
  Tg=0.;

  for(j=0;j<NRING;j++)
    {
      for(i=0;i<S32K;i++)
	{
	  tmp_bg[i]=(double)tproj[j][i];
	  dtproj[j][i]=(double)tproj[j][i];
	  dgated[j][i]=(double)gated[j][i];
	  Tp+=(double)tproj[j][i];
	  Tg+=(double)gated[j][i];
	}
      
      autobkgnd();
      
      for(i=0;i<S32K;i++)
	{
	  dbg[j][i]=tmp_bg[i];
	  sbg[j][i]=(int)rint(dbg[j][i]);
	}
      
      for(i=(int)relow[j];i<=(int)rehigh[j];i++)
	{
	  Nb+=dbg[j][i];
	  N+=dtproj[j][i];
	  //printf("T %.2f Np %.2f Nb %.2f\n",T,dtproj[j][i],dbg[j][i]);
	}
    }
  
  Np=N-Nb;
  printf("Tp %.2f Tg %.2f Np %.2f Nb %.2f N %.2f norm factor %.4f \n",Tp,Tg,Np,Nb,N,Tg/Tp);
   /* end calculation of normalization factors */
  
  /* do background subtraction */
  for(j=0;j<NRING;j++)
    for(i=0;i<S32K;i++)
      {
	final[j][i] = (int)rint(dgated[j][i]-(Tg/Tp)*(Nb/N)*dtproj[j][i]-(Tg/Tp)*(Np/N)*dbg[j][i]);
	tbg[j][i]=(int)rint((Tg/Tp)*(Nb/N)*dtproj[j][i]+(Tg/Tp)*(Np/N)*dbg[j][i]);
      }
  /* end background subtraction */

  /*OUTPUT SECTION*/
  //write the smooth background spectra
  sprintf(mcaFile,"smooth_background.mca");
  if((sbg_output=fopen(mcaFile,"w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(sbg,NRING*S32K*sizeof(int),1,sbg_output);
  fclose(sbg_output);
  
  //write the total background spectra
  sprintf(mcaFile,"total_background.mca");
  if((tbg_output=fopen(mcaFile,"w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(tbg,NRING*S32K*sizeof(int),1,tbg_output);
  fclose(tbg_output);
  
  //write the final spectra
  sprintf(mcaFile,"Ring_gatedBS.mca");
  if((final_output=fopen(mcaFile,"w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(final,NRING*S32K*sizeof(int),1,final_output);
  fclose(final_output);
  
  return 0;
}
/*====================================================================================*/
int autobkgnd(void)
{
  /* find background over +/- w2*FWHM */
  static double w2 = 6.;
  double fwhm, x, w1, x1, y1, x2, y2, bg, r1;
  int   i, hi, lo, ich, jch, ihi=0, ilo=0, ihi2, ilo2;

  w1 = 1.;
  ilo=bLow;
  ihi=bHigh;
  ilo2 = ihi;
  ihi2 = ilo;
  
  for (i = 1; i <= 3; ++i) 
    {
      for (ich = ilo + 2; ich <= ihi - 2; ich += 5) 
	{
	  x = (double) ich;
	  fwhm = sqrt(swpars[0] + swpars[1] * x + swpars[2] * x * x);
	  r1 = w1 * w2 * fwhm;
	  lo = ich - rint(r1);
	  r1 = w1 * w2 * fwhm; //why is this here twice?!?
	  hi = ich + rint(r1);

	  if (lo < ilo)
	    lo = ilo;
	  if (hi > ihi)
	    hi = ihi;

	  find_bg2(lo, hi, &x1, &y1, &x2, &y2);

	  if (ilo2 > rint(x1))
	    ilo2 = rint(x1);
	  if (ihi2 < rint(x2))
	    ihi2 = rint(x2);
  
	  for (jch = rint(x1); jch <= rint(x2); ++jch) 
	    {
	      x = (double) jch;
	      bg = y1 - (x1 - x) * (y1 - y2) / (x1 - x2);
	      tmp_bg[jch] = bg;
	    }
	}
    }
  for (ich = ilo2; ich <= ihi2; ++ich)
    {
      tmp_bg[ich] += sqrt(tmp_bg[ich]) * 1.6;
    }
  return 0;
}
/*====================================================================================*/
int find_bg2(int loch, int hich, double *x1, double *y1, double *x2, double *y2)
{
  double a,y;
  int   i, mid;

  mid = (loch + hich) / 2;

  /* find channel with least counts on each side of middle */
  *y1 = (tmp_bg[loch] + tmp_bg[loch + 1] + tmp_bg[loch + 2]) / 3.;
  *x1 = (double) (loch + 1);
  for (i = loch + 2; i <= mid - 2; ++i) 
    {
      a = (tmp_bg[i-1] + tmp_bg[i] + tmp_bg[i+1]) / 3.;
      if (*y1 > a) 
	{
	  *y1 = a;
	  *x1 = (double) i;
	}
    }
  *y2 = (tmp_bg[mid + 2] + tmp_bg[mid + 3] + tmp_bg[mid + 4]) / 3.;
  *x2 = (double) (mid + 3);
  for (i = mid + 4; i <= hich - 1; ++i) 
    {
      a = (tmp_bg[i-1] + tmp_bg[i] + tmp_bg[i+1]) / 3.;
      if (*y2 > a) 
	{
	  *y2 = a;
	  *x2 = (double) i;
	}
    }

  /* check that there are no channels between loch and hich
     that are below the background. */
  if (*y2 > *y1) 
    {
      for (i = rint(*x1) + 1; i <= mid - 2; ++i) 
	{
	  y = *y1 - (*x1 - (double) i) * (*y1 - *y2) / (*x1 - *x2);
	  a = (tmp_bg[i-1] + tmp_bg[i] + tmp_bg[i+1]) / 3.;
	  if (y > a) 
	    {
	      *y1 = a;
	      *x1 = (double) i;
	    }
	}
      for (i = rint(*x2) + 1; i <= hich - 1; ++i) 
	{
	  y = *y1 - (*x1 - (double) i) * (*y1 - *y2) / (*x1 - *x2);
	  a = (tmp_bg[i-1] + tmp_bg[i] + tmp_bg[i+1]) / 3.;
	  if (y > a) 
	    {
	      *y2 = a;
	      *x2 = (double) i;
	    }
	}
    }
  else 
    {
      for (i = rint(*x1) - 1; i >= loch + 1; --i) 
	{
	  y = *y1 - (*x1 - (double) i) * (*y1 - *y2) / (*x1 - *x2);
	  a = (tmp_bg[i-1] + tmp_bg[i] + tmp_bg[i+1]) / 3.;
	  if (y > a) 
	    {
	      *y1 = a;
	      *x1 = (double) i;

	    }
	}
      for (i = rint(*x2) - 1; i >= mid + 3; --i) 
	{
	  y = *y1 - (*x1 - (double) i) * (*y1 - *y2) / (*x1 - *x2);
	  a = (tmp_bg[i-1] + tmp_bg[i] + tmp_bg[i+1]) / 3.;
	  if (y > a) 
	    {
	      *y2 = a;
	      *x2 = (double) i;
	    }
	}
    }
  if (*y1 < 1.) 
    *y1 = 1.;
  if (*y2 < 1.) 
    *y2 = 1.;
  
  return 0;
}
/*====================================================================================*/
void read_TIGRESS_ring_energy_gates(char *filename)
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
  printf("\nTIGRESS ring energy gates for DSAM analysis read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %f %f",&ring,&low,&high)!=EOF)
	  if(ring>0 && ring<NRING)
	      {
		relow[ring]=low;
		rehigh[ring]=high;
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

  

