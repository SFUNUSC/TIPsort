#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  uint64_t one=1;
  int pos,ring;
  double e;
  
 //if((data->h.setupHP&RF_BIT)==0)
 //   return SEPARATOR_DISCARD;

 if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);

  if(data->csiarray.h.Efold>0)
    for(pos=1;pos<NCSI;pos++)
      if((data->csiarray.h.THP[pos/64]&(one<<pos%64))!=0)
				{
          e=data->csiarray.csi[pos].charge;
          ring=cev->csiarray.ring[pos];
          /* printf("CsIArray pos %d ene = %f ring %d\n",pos,e,ring); */
          /* getc(stdin); */
          if(e>=0 && e<S32K)
            hist[ring][(int)(e)]++;
        }
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;

  if(argc!=2)
    {
      printf("CsIArray_RawRing master_file_name\n");
      printf("Program sorts ERaw histograms separated by ring for the CsIArray.\n");
      exit(-1);
    }
  
  printf("Program sorts ERaw histograms separated by ring for the CsIArray.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);

  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined!\n");
      exit(EXIT_FAILURE);
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
  
  sort(name);
  
  if((output=fopen("CsIArray_ERawRing.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!");
      exit(EXIT_FAILURE);
    }

  fwrite(hist,NCSIRING*S32K*sizeof(int),1,output);
  fclose(output);
  }
