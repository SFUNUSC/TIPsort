#include "sort.h"

int analyze_data(raw_event *data)
{
	
	if((data->h.setupHP&TIGRESS_BIT)!=0)
    {
			//check if the position is in the hit pattern
			if((data->tg.h.GeHP&(1<<(pos-1)))!=0)
				//check if the position and color are in the hit pattern
				if((data->tg.det[pos].h.GeHP&(1<<col))!=0)
					{
						display_raw_event(data);
						getc(stdin);
					}
		}

  return SEPARATOR_DISCARD;
}
/*====================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;

  if(argc!=4)
    {
      printf("display_RawEventForTigressCore sfu_input_data pos col\n");
      printf("Program displays raw events containing the specified Tigress core.\n");
      exit(-1);
    }
  
  printf("Program displays raw events containing the specified Tigress core.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  pos=atoi(argv[2]);
  col=atoi(argv[3]);
  sort(name); 
}
