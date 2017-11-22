#include "sort.h"

int analyze_data(raw_event *data)
{

  uint64_t one=1;
  int pos;

	if((data->h.setupHP&CsIArray_BIT)==0) 
    return SEPARATOR_DISCARD;

	for(pos=1;pos<NCSI;pos++)
    if((data->csiarray.h.THP[pos/64]&(one<<pos%64))!=0)
    	{
    		if(pos<posLow || pos>posHigh)
					return SEPARATOR_DISCARD;
    	}

	return SEPARATOR_KEEP;
}
/*====================================================================================*/

int main(int argc, char *argv[])
{
  input_names_type* name;
  
  if(argc!=5)
    {
      printf("separate_CsIPos SFU_input_data SFU_output_data posLow posHigh\n");
      printf("Program separates data with Csi hits only in the position range specified.\n");
      exit(-1);
    }
   
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  strcpy(name->fname.out_data,argv[2]);
  name->flag.separate=1;

  posLow=atoi(argv[3]);
  posHigh=atoi(argv[4]);
  
  sort(name); 
}
