#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos1,pos2,ring;
  double u;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);

  if(cev->csiarray.h.FE==2)
    if(cev->csiarray.h.FT==2)
      for(pos1=1;pos1<NCSI;pos1++)
        if((cev->csiarray.h.EHP&(one<<pos1))!=0)
          {
            ring=cev->csiarray.ring[pos1];//check the ring position of the first hit
            for(pos2=pos1+1;pos2<NCSI;pos2++)
	      if((cev->csiarray.h.THP&(one<<pos2))!=0)
                if(ring==cev->csiarray.ring[pos2])//check that the 2 hits are in the same ring
                  {
	            u=cev->csiarray.U;
	            //printf("CsIArray ene = %f\n",u);
                    //getc(stdin);
	            if(u>=0 && u<S32K)
	              hist[ring][(int)rint(u)]++;
                  }
          }
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *cluster;
  input_names_type* name;
  char DataFile[132];


  if(argc!=2)
    {
      printf("CsIArray_DeltaUSameRingSum master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts deltaU histograms for the energy calibrated CsI array.  Only sorts events containing 2 CsIArray hits in the same ring.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);

  if(name->flag.cluster_file==1)
    {
      printf("Sorting deltaU histograms for the energy calibrated CsI array based upon the cluster file: %s\n",name->fname.cluster_file);
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

  if(name->flag.CSIARRAY_cal_par==1)
    {
      printf("CsIArray calpar read from: %s\n",name->fname.CSIARRAY_cal_par);
      initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
    }
  else
    {
      printf("ERROR! CsIArray calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }

  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting CsI Delta U data from file %s\n", name);
      sort(name);
    }  
  
  if((output=fopen("CsIArray_deltaUSameRingSum.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!");
      exit(EXIT_FAILURE);
    }
 
  for(int pos=0;pos<NCSI;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);
  
  fclose(output);
}