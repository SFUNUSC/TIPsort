#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  uint64_t one=1;
  int pos,col,ring,csi;
  double etig,ecsi;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);

 if((data->h.setupHP&TIGRESS_BIT)==0)
    return SEPARATOR_DISCARD;

  if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;

 if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;

 if(cev->tg.h.FH>0)
   if(cev->csiarray.h.FH>0)
     for(pos=1;pos<NPOSTIGR;pos++)
       if(((cev->tg.h.HHP&(1<<(pos-1)))!=0) && (cev->tg.det[pos].hge.FE>0))
	 if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
	   {	
	     etig = cev->tg.det[pos].addback.E; /* TIGRESS energy in keV */
	     col = cev->tg.det[pos].addbackC;
	     ring=cev->tg.det[pos].addbackR;
	     
	     if(cev->csiarray.h.FH>0)
	       for(csi=1;csi<NCSI;csi++)
			 if((cev->csiarray.h.HHP[csi/64]&(one<<csi%64))!=0)
				 {
				   ecsi=cev->csiarray.csi[csi].E/1000.; /* CsI energy in MeV */
				   
				   if(ecsi>=eLow && ecsi<=eHigh)
				     {
							 if(etig<0 || etig>S32K-10) 
								 etig=S32K-10;
							 
							 hist[ring][(int)(etig)]++;
				     }
				 }
	   }
 
 free(cev);
 return SEPARATOR_DISCARD;
}
/*=============================================================================*/
int main(int argc, char *argv[])
{
  FILE *cluster,*output;
  input_names_type* name;
  char DataFile[132];
 
  if(argc!=4)
    {
      printf("Tigress_ECalABRingSum_CsIEGate master_file_name Elow [MeV] Ehigh [MeV] \n");
      exit(-1);
    }

  //CsI energies for gating
  eLow=atof(argv[2]);
  eHigh=atof(argv[3]);
  
  printf("Program sorts ECalABRing_CsIEGate histograms for TIGRESS.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  memset(hist,0,sizeof(hist));

  read_master(argv[1],name);
  
  if(name->flag.cluster_file==1)
    {
      printf("Sorting ECalABSuppRingSum_PIDr energy histograms for TIGRESS cores based upon the cluster file: %s\n",name->fname.cluster_file);
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
      printf("TIGRESS calibration read from %s.\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  if(name->flag.CSIARRAY_cal_par==1)
    {
      printf("CsIArray calpar read from: %s\n",name->fname.CSIARRAY_cal_par);
      initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
    }
  else
    {
      printf("ERROR!!! TIGRESS calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting data from %s\n", name);
      sort(name); 
    }

  if((output=fopen("Ring_ECalABSum_CsIEGate.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  
  fwrite(hist,NRING*S32K*sizeof(int),1,output);

  fclose(output);

  return 0;
}
