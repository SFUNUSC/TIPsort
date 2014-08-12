#include "sort.h"

int analyze_data(raw_event *data)
{
 
  unsigned long long int one=1,none=-1;
  int csi,type;
  double chi;
  unsigned long long int flag=0,kill;


  if((data->h.setupHP&CsIArray_BIT)==0)
    {
      encode(data,output,enb);
      return SEPARATOR_DISCARD;
    }

  if((data->h.setupHP&RF_BIT)==0)
    {
      encode(data,output,enb);
      return SEPARATOR_DISCARD;
    }
 
  
 
  for(csi=1;csi<NCSI;csi++)
    if((data->csiarray.h.TSHP&(one<<csi))!=0)
      {
	type=data->csiarray.wfit[csi].type;
	chi=data->csiarray.wfit[csi].chisq;
	chi/=data->csiarray.wfit[csi].ndf;
	if(type>=idmin)
	  if(type<=idmax)
	    if(chi>=chimin)
	      if(chi<=chimax)
		flag|=(one<<csi);						     
      }
			  
	 
  
  //drop csi with the flag of zero
  for(csi=1;csi<NCSI;csi++)
    if((data->csiarray.h.TSHP&(one<<csi))!=0)
      if((flag&(one<<csi))==0)
	{
	  memset(&data->csiarray.csi[csi],0,sizeof(channel));
	  memset(&data->csiarray.wfit[csi],0,sizeof(ShapePar));
	  memset(&data->csiarray.t0[csi],0,sizeof(double));
	  data->csiarray.h.Efold--;
	  data->csiarray.h.Tfold--;	  
	  data->csiarray.h.TSfold--;
	  kill=none-(one<<csi);
	  data->csiarray.h.TSHP&=kill;
	  data->csiarray.h.EHP&=kill;
	  data->csiarray.h.THP&=kill;
	}
  
  if(data->csiarray.h.TSfold<=0)
    {
      kill=none-CsIArray_BIT; 
      data->h.setupHP&=kill;
      memset(&data->csiarray,0,sizeof(CsIArray));
    }

    encode(data,output,enb);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/

int main(int argc, char *argv[])
{

  input_names_type* name;
 
  if(argc!=7)
    {
      printf("\n ./separate_CSIARRAY_FitType input_file_name output_file_name idmin idmax chimin chimax\n");
      exit(-1);
    }

  printf("Program separates events based on CsI fit type\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  strcpy(name->fname.out_data,argv[2]);
  idmin=atoi(argv[3]);
  idmax=atoi(argv[4]);
  chimin=atof(argv[5]);
  chimax=atof(argv[6]);
 
 
 
  if((output=fopen(name->fname.out_data,"w"))==NULL)
    {
      printf("\nI can't open output file %s for writing\n",name->fname.out_data);
      exit(-2);
    }
  memset(enb,0,sizeof(enb));
  enb[0]=BUFFER_TAG;
  enb[1]++;
  enb[1]++;


  sort(name); 
  // save the last buffer
  fwrite(enb,sizeof(int),BUFFSIZE,output);
 
}
