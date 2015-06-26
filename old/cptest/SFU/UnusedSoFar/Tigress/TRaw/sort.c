#include "sort.h"

int analyze_data(raw_event *data)
{

  int pos,col;
  double t;

  if(data->tg.h.Gefold>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((data->tg.h.GeHP&(1<<(pos-1)))!=0)
	if(data->tg.det[pos].h.Gefold>0)
	  for(col=0;col<NCOL;col++)
	    if((data->tg.det[pos].h.GeHP&(1<<col))!=0)
	      if(data->tg.det[pos].ge[col].h.Tfold>0)
		if((data->tg.det[pos].ge[col].h.THP&1)!=0)
		  {
		    t=data->tg.det[pos].ge[col].seg[0].cfd&0x00ffffff;
		    t-=(data->tg.det[pos].ge[col].seg[0].timestamp*16)&0x00ffffff;
		    if((data->h.setupHP&RF_BIT)!=0)
		      {
			//			t-=data->rf.sin.t0;
			t+=S16K;
			if(t<0)t=S32K-2;
			if(t>S32K) t=S32K-3;
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
      printf("\n ./Tigress_ERaw sfu_input_data_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts raw time histogram for Tigress cores \n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  memset(hist,0,sizeof(hist));

  sort(name);

  for(pos=1;pos<NPOSTIGR;pos++)
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
