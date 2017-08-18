#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos;
  double eTotal=0.;

  //Need to ignore RF bit for analysis of triple alpha calibration sources  
  /*if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;
*/
 if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);

  if(cev->csiarray.h.FE>0)
    for(pos=1;pos<NCSI;pos++)
      if((cev->csiarray.h.EHP&(one<<pos))!=0)
				{
					eTotal+=cev->csiarray.csi[pos].E/cal_par->csiarray.contr_e;
					/* printf("CsIArray pos %d ene = %f\n",pos,e); */
					/* getc(stdin); */
					
				}
	eTotal=eTotal/10.; //contract data so it will fit
	//printf("eTotal: %f\n",eTotal);
	if(eTotal>=0 && eTotal<S32K)
		{
			hist[(int)(eTotal)]++;
		}
	else
		{
			hist[S32K-1000]++;
		}
		
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *cluster;
  input_names_type* name;
  //TCanvas *canvas;
  //TApplication *theApp;
  char DataFile[132];

  if(argc!=2)
    {
      printf("CsIArray_ECalTotalEventSum master_file_name\n");
      printf("Program sorts a summed ECal histogram for the CsIArray, containing the total energy deposited in each event from all CsIArray hits, from a cluster file.\n");
      exit(-1);
    }
  
  printf("Program sorts a summed ECal histogram for the CsIArray, containing the total energy deposited in each event from all CsIArray hits, from a cluster file.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);

  if(name->flag.cluster_file==1)
    {
      printf("Sorting calibrated energy histograms for CsIArray based upon the cluster file: %s\n",name->fname.cluster_file);
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
      
      printf("Sorting particle energy data from file %s\n", name->fname.inp_data);
      sort(name);
    }
  
  if((output=fopen("CsIArray_ECalTotalEventSum.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!");
      exit(EXIT_FAILURE);
    }
  fwrite(hist,S32K*sizeof(int),1,output);
  fclose(output);
  printf("Histogram written to file: CsIArray_ECalTotalEventSum.mca\n");
  printf("Data is contracted by a factor of 10.\n");
}
