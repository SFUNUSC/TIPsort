#include "sort.h"

int analyze_data(raw_event *data)
{

  int e;
  double t;
  if(data->tg.h.BGOfold>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((data->tg.h.BGOHP&(1<<(pos-1)))!=0)
	if(data->tg.det[pos].h.BGOfold>0)
	  for(col=0;col<NCOL;col++)
	    if((data->tg.det[pos].h.BGOHP&(1<<col))!=0)
	      if(data->tg.det[pos].bgo[col].h.Efold>0)
		for(sup=0;sup<NSUP;sup++)
		  if((data->tg.det[pos].bgo[col].h.EHP&(1<<sup))!=0)
		    {
		      e=data->tg.det[pos].bgo[col].sup[sup].charge;
		      t=data->tg.det[pos].bgo[col].sup[sup].cfd&0x00ffffff;
		      t-=(data->tg.det[pos].bgo[col].sup[sup].timestamp*16)&0x00ffffff;
		      if((data->h.setupHP&RF_BIT)!=0)
			{
			  t-=(int)data->rf.sin.t0;
			  t+=S16K;
			  if(t<0)t=S32K-2;
			  if(t>S32K) t=S32K-3;
			}
		      else
			t=S32K-4;

		      if(e>=0)
			if(e<S32K)
			  if(t>tlow)
			    if(t<thigh)
			  hist[pos][col][sup][e]++;
		    }
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  char n[132];
 
  if(argc!=4)
    {
      printf("\n ./Tigress_EBGORawLim sfu_input_data_file_name tlow thigh\n");
      exit(-1);
    }
  
  printf("Program sorts raw energy histogram for Tigress suppressors \n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  memset(hist,0,sizeof(hist));
  tlow=atof(argv[2]);
  thigh=atof(argv[3]);
  sort(name);

  for(pos=1;pos<NPOSTIGR;pos++)
    for(col=0;col<NCOL;col++)
      {
	sprintf(n,"pos%1d%1d_col%d_raw_bgo_lim_energy.mca",pos/10,pos-(pos/10)*10,col);
	if((output=fopen(n,"w"))==NULL)
	  {
	    printf("\nI can't open file %s\n",n);
	    exit(EXIT_FAILURE);
	  }
	for(sup=0;sup<NSUP;sup++)
	  fwrite(hist[pos][col][sup],S32K*sizeof(int),1,output);
	fclose(output);
    }



}

