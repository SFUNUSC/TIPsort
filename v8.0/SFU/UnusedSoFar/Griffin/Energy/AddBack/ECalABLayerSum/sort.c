#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  double eAddBack;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_GRIFFIN(data,&cal_par->gr,&cev->gr);

  if(cev->gr.h.FH>0)
    for(pos=1;pos<NPOSGRIF;pos++)
      if(((cev->gr.h.HHP&(1<<(pos-1)))!=0) && (cev->gr.det[pos].hge.FE>0))
        if((cev->gr.h.AHP&(1<<(pos-1)))!=0)
	  {	
	    eAddBack = cev->gr.det[pos].addback.E/cal_par->gr.contr_e;
	    col = cev->gr.det[pos].addbackC;
	    ring=cev->gr.det[pos].ge[col].ring;
	    layer=(ring-1)/2;
	    
	    if((eAddBack>=0) && (eAddBack<S32K))
	      {
		if((ring>0) && (ring<NRING)) hist[layer][(int)rint(eAddBack)]++;
	      }
	    else hist[layer][S32K-1000]++;
	  }
  free(cev);
  return SEPARATOR_DISCARD;
}
/*=============================================================================*/
int main(int argc, char *argv[])
{
  FILE *output,*cluster;
  input_names_type* name;
  char DataFile[132];

  if(argc!=2)
    {
      printf("Griffin_ECalABLayerSum master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts ECalABLayerSum histograms for GRIFFIN.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);
  
  if(name->flag.cluster_file==1)
    {
      printf("Sorting ECalABLayerSum histograms for GRIFFIN clovers and cores based upon the cluster file: %s\n",name->fname.cluster_file);
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

  
  if(name->flag.GRIFFIN_cal_par==1)
    {
      printf("GRIFFIN calibration read from %s.\n",name->fname.GRIFFIN_cal_par);
      initialize_GRIFFIN_calibration(&cal_par->gr,name->fname.GRIFFIN_cal_par);
    }
  else
    {
      printf("ERROR!!! GRIFFIN calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting data from file %s\n", name);
      sort(name);
    }
  
  if((output=fopen("Layer_ECalABSum.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  
  fwrite(hist,NLAYER*S32K*sizeof(int),1,output);
  fclose(output);
}
