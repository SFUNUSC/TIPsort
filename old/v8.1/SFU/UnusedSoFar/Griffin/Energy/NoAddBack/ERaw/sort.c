#include "sort.h"

int analyze_data(raw_event *data)
{
  int pos,col,e;
  
  if(data->gr.h.Gefold>0)
    for(pos=1;pos<NPOSGRIF;pos++)
      if((data->gr.h.GeHP&(1<<(pos-1)))!=0)
	if(data->gr.det[pos].h.Gefold>0)
	  for(col=0;col<NCOL;col++)
	    if((data->gr.det[pos].h.GeHP&(1<<col))!=0)
	      if(data->gr.det[pos].ge[col].h.Efold>0)
		if((data->gr.det[pos].ge[col].h.EHP&1)!=0)
		  {
		    e=data->gr.det[pos].ge[col].seg[0].charge;
		    if(e>=0 && e<S32K)
		      hist[pos][col][e]++;
		    else 
		      {
			//printf("Bad energy is %d\n",e);
			hist[pos][col][S32K-1000]++;
		      }}
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  char n[132];
  int pos,col;
  
  if(argc!=2)
    {
      printf("Griffin_ERaw sfu_input_data\n");
      exit(-1);
    }
  
  printf("Program sorts ERaw histograms for Griffin cores \n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  memset(hist,0,sizeof(hist));
  
  sort(name);
  
  for(pos=1;pos<NPOSGRIF;pos++)
    {
      sprintf(n,"pos%1d%1d_raw_core_energy.mca",pos/10,pos-(pos/10)*10);
      if((output=fopen(n,"w"))==NULL)
	{
	  printf("ERROR!!! I can't open the mca file!\n");
	  exit(EXIT_FAILURE);
	}
      
      for(col=0;col<NCOL;col++)
	{
	  fwrite(hist[pos][col],S32K*sizeof(int),1,output);
	}
      fclose(output);
    }}
