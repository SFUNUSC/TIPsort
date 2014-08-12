#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos,col,S3ring,TigRing;
  double ttg,tS3ring,eS3ring,etg;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_S3ring(data,&cal_par->s3ring,&cev->s3);

  if(cev->tg.h.FH>0)
    if(cev->s3.rh.FH>0)
      for(pos=1;pos<NPOS;pos++)
	if((cev->tg.h.HHP&(1<<(pos-1)))!=0)
	  if(cev->tg.det[pos].hge.FH>0)
	    if(cev->tg.det[pos].suppress==0)
	      {
		col=cev->tg.det[pos].addbackC;
		ttg=cev->tg.det[pos].addback.T;
		etg=cev->tg.det[pos].addback.E;
		TigRing=cev->tg.det[pos].ge[col].ring;

		if(ttg>cal_par->tg.ctlow[pos][col] && ttg<cal_par->tg.cthigh[pos][col])
		  if(etg>cal_par->tg.celow[pos][col] && etg<cal_par->tg.cehigh[pos][col])
		    for(S3ring=1;S3ring<NS3RING;S3ring++)
		      if(S3ring==1)
			if((cev->s3.rh.EHP&(one<<S3ring))!=0)
			  {
			    tS3ring=cev->s3.ring[S3ring].T;
			    eS3ring=cev->s3.ring[S3ring].E;
			    if(tS3ring>cal_par->s3ring.tlow[S3ring] && tS3ring<cal_par->s3ring.thigh[S3ring])
			      if(eS3ring>cal_par->s3ring.elow[S3ring] && eS3ring<cal_par->s3ring.ehigh[S3ring])
				{
				  etg/=cal_par->tg.contr_e;
				  if(etg>=0 && etg<S32K)
				    {
				      if(TigRing > 0 && TigRing<NRING)
					hist[TigRing][(int)rint(etg)]++;
				    }
				  else hist[TigRing][S32K-10]++;
				}}}	
  free(cev);
  return SEPARATOR_DISCARD;
}
/*==============================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  FILE* output,*cluster;
  char n[132];

  if(argc!=2)
    {
      printf("TIG-S3_ABandSuppRings_AllLimCluster master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts calibrated TIGRESS ring suppressed addback spectra gated on S3 ring and TIGRESS energies and times \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);

  if(name->flag.cluster_file==1)
    {
      printf("Sorting data from cluster %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("ERROR! I can't open %s\n",name->fname.cluster_file);
	  exit(-2);
	}}

  else
    {
      printf("ERROR! Cluster file not defined\n");
      exit(-1);
    }

  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calibration read from %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR! TIGRESS calibration parameters not defined\n");
      exit(-1);
    }

  if(name->flag.S3ring_cal_par==1)
    {
      printf("S3ring calibration read from %s\n",name->fname.S3ring_cal_par);
      initialize_S3ring_calibration(&cal_par->s3ring,name->fname.S3ring_cal_par);
    }
  else
    {
      printf("S3 ring calibration parameters not defined\n");
      exit(-1);
    }

  name->flag.inp_data=1; 
  while(fscanf(cluster,"%s",n)!=EOF)
    {
      strcpy(name->fname.inp_data,n);
      sort(name);
    }
  fclose(cluster);

  if((output=fopen("gated_rings.mca","w"))==NULL)
    {
      printf("ERROR! I can't open the mca file");
      exit(EXIT_FAILURE);
    }
  
  fwrite(hist,NRING*S32K*sizeof(int),1,output);
	 
  fclose(output);
}
