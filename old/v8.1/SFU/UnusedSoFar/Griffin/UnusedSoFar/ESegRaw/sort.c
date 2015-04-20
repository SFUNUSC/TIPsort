#include "sort.h"

int analyze_data(raw_event *data)
{

  int e;
  if(data->gr.h.Gefold>0)
    for(pos=1;pos<NPOSGRIF;pos++)
      if((data->gr.h.GeHP&(1<<(pos-1)))!=0)
	if(data->gr.det[pos].h.Gefold>0)
	  for(col=0;col<NCOL;col++)
	    if((data->gr.det[pos].h.GeHP&(1<<col))!=0)
	      if(data->gr.det[pos].ge[col].h.Efold>0)
		for(seg=0;seg<NSEGGRIF;seg++)
		  if((data->gr.det[pos].ge[col].h.EHP&(1<<seg))!=0)
		    {
		      e=data->gr.det[pos].ge[col].seg[seg].charge;
		      if(e>=0)
			if(e<S32K)
			  hist[pos][col][seg][e]++;
		    }
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  char n[132];
 
  if(argc!=2)
    {
      printf("\n ./Griffin_ESegRaw sfu_input_data_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts raw energy histogram for Griffin cores \n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  memset(hist,0,sizeof(hist));

  sort(name);

  for(pos=1;pos<NPOSGRIF;pos++)
    for(col=0;col<NCOL;col++)
      {
	sprintf(n,"pos%1d%1d_col%d_raw_seg_energy.mca",pos/10,pos-(pos/10)*10,col);
	if((output=fopen(n,"w"))==NULL)
	  {
	    printf("\nI can't open file %s\n",n);
	    exit(EXIT_FAILURE);
	  }
	for(seg=0;seg<NSEGGRIF;seg++)
	  fwrite(hist[pos][col][seg],S32K*sizeof(int),1,output);
	fclose(output);
    }



}

