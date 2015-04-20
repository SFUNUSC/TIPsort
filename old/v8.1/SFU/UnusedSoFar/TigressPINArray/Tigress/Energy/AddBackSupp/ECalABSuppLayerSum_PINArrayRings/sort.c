#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  cal_event* cev;
  double eAddBack=0;
  int take=0,suppFlag=0;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_PINARRAY(data,&cal_par->pinarray,&cev->pinarray);
  
  if((cev->tg.h.FH>0) && (cev->pinarray.h.FH>0))
    for(pos=1;pos<NPOSTIGR;pos++)
      if(((cev->tg.h.HHP&(one<<(pos-1)))!=0) && (cev->tg.det[pos].hge.FE>0))
	if((cev->tg.h.AHP&(one<<(pos-1)))!=0)
	  {
	    take=0;
	    eAddBack = cev->tg.det[pos].addback.E/cal_par->tg.contr_e;  
	    suppFlag = cev->tg.det[pos].suppress;
	    col = cev->tg.det[pos].addbackC;
	    ring = cev->tg.det[pos].ge[col].ring;

	    if((suppFlag==0) && ((eAddBack>=0) && (eAddBack<S32K)))
	      if((ring>0) && (ring<NRING))
		for(pin=1;pin<NPIN;pin++)
		  if(((cev->pinarray.h.HHP&(one<<pin))!=0) && (take==0))
		    {
		      take=1;
		      pinRing = (cal_par->pinarray.ring_map[pin] - 1);
		      
		      //layer must be defined here so it resets with a new pin
		      //for fold 2 events!!!! (if above take check is commented out)
		      layer = (ring-1)/2;
		      
		      if((pinRing>=0) && (pinRing<2))
			//if((pinRing>=0) && (pinRing<NPINRING))
			{
			  //layer *= NPINRING;
			  //layer += pinRing;
			  hist[layer][(int)rint(eAddBack)]++; 
			}}}
  free(cev);
  return SEPARATOR_DISCARD;
}
/*=========================================================================*/
int main(int argc, char *argv[])
{
  FILE *output,*cluster;
  input_names_type* name;
  char DataFile[132];
  
  if(argc!=2)
    {
      printf("Tigress_PINArrayRingsECalABSuppLayerSum master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts ECalABSuppLayerSum histograms for TIGRESS gated on PINArrayRings.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);
  
  if(name->flag.cluster_file==1)
    {
      printf("Sorting based upon the cluster file: %s\n",name->fname.cluster_file);
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
  
  if(name->flag.PINARRAY_cal_par==1)
    {
      printf("PINARRAY calibration read from %s\n",name->fname.PINARRAY_cal_par);
      initialize_PINARRAY_calibration(&cal_par->pinarray,name->fname.PINARRAY_cal_par);
    }
  else
    {
      printf("ERROR!!! PINARRAY calibration parameters not defined!\n");
      exit(-1);
    }
  
  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting data from file %s\n", name);
      sort(name);
    }
  
  if((output=fopen("Layer_PINArrayRingsECalABSuppSum.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(hist,NLAYER*NPINRING*S32K*sizeof(int),1,output);
  fclose(output);
}
