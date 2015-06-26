#include "sort.h"

int analyze_data(raw_event *data)
{

  int pos,col;
  double t;

  if(data->gr.h.Gefold>0)
    for(pos=1;pos<NPOSGRIF;pos++)
      if((data->gr.h.GeHP&(1<<(pos-1)))!=0)
	if(data->gr.det[pos].h.Gefold>0)
	  for(col=0;col<NCOL;col++)
	    if((data->gr.det[pos].h.GeHP&(1<<col))!=0)
	      if(data->gr.det[pos].ge[col].h.Tfold>0)
		if((data->gr.det[pos].ge[col].h.THP&1)!=0)
		  {
		    t=data->gr.det[pos].ge[col].t0[0]*16;

		    if((data->h.setupHP&RF_BIT)!=0)
		      {
		    	t-=data->rf.sin.t0;
			t/=16;
		    	t+=S16K;
		      }
		    else
		      t=S32K-4;

		    if(t>=0)
		      if(t<S32K)
			hist[pos][col][(int)rint(t)]++;
		      }
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
      printf("\n ./Griffin_TFitRaw sfu_input_data_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts raw time histogram for Griffin cores \n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  memset(hist,0,sizeof(hist));

  sort(name);

  for(pos=1;pos<NPOSGRIF;pos++)
    {
      sprintf(n,"pos%1d%1d_raw_core_time.mca",pos/10,pos-(pos/10)*10);
      if((output=fopen(n,"w"))==NULL)
	{
	  printf("\nI can't open file %s\n",n);
	  exit(EXIT_FAILURE);
	}

      for(col=0;col<NCOL;col++)
	{
	  fwrite(hist[pos][col],S32K*sizeof(int),1,output);
	}
      fclose(output);
    }



}
