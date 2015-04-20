#include "sort.h"

int analyze_data(raw_event *data)
{

  unsigned long long int one=1;
  int pos;
  double chi;

  for(pos=1;pos<NCSI;pos++)
    if((data->csiarray.h.THP&(one<<pos))!=0)
      if(data->csiarray.wfit[pos].type==1)
	{
	  chi=data->csiarray.wfit[pos].chisq;
	  chi/=data->csiarray.wfit[pos].ndf;
	  if(chi>=chimin)
	    if(chi<=chimax)
	      return SEPARATOR_KEEP;
	}

  return SEPARATOR_DISCARD;
}
/*====================================================================================*/

int main(int argc, char *argv[])
{

  input_names_type* name;
  

  if(argc!=5)
    {
      printf("\n ./separate_CSI_Type1_Fit sfu_input_data_file_name sfu_output_data_file_name chimin chimax\n");
      exit(-1);
    }

  printf("Program separates events with CsI array type 1 fits\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  strcpy(name->fname.out_data,argv[2]);
  chimin=atof(argv[3]);
  chimax=atof(argv[4]);
  name->flag.separate=1;

  sort(name); 
   
}
