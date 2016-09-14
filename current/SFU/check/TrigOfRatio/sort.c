#include "sort.h"

int analyze_data(raw_event *data)
{
  int trig;
  
  unsigned long long int one=1;
  int type;
  double chi,s,f,r,t;
  
  if((data->h.setupHP&RF_BIT)==0) return SEPARATOR_DISCARD;
  if((data->h.setupHP&CsIArray_BIT)==0) return SEPARATOR_DISCARD;
  
  for(pos=1;pos<NCSI;pos++)
    if((data->csiarray.h.THP&(one<<pos))!=0)
      //if((data->csiarray.h.TSfold)==1)
      {
	type=data->csiarray.wfit[pos].type;
	chi=data->csiarray.wfit[pos].chisq;
	chi/=data->csiarray.wfit[pos].ndf;
	
	if((type>=idmin) && (type<=idmax))
	  if((chi>=chimin) && (chi<=chimax))
	    {
	      s=data->csiarray.wfit[pos].am[3];
	      f=data->csiarray.wfit[pos].am[2];
	      r=s/f*100;

	      trig=data->h.trig_num&0x7fffffff;
	      printf("Trigger number and ratio are: %d %f \n",trig,r);
	    }}
  return SEPARATOR_DISCARD;
}
/*==============================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  
  if(argc!=6)
    {
      printf("TrigOfRatio sfu_data_file idmin idmax chimin chimax ratioLow ratioHigh\n");
      exit(-1);
    }
  
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  ratioLow=atof(argv[6]);
  ratioHigh=atof(argv[7]);
  
  printf("Program prints trigger number for events with PID ratios in specified range\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  
  sort(name);
}
