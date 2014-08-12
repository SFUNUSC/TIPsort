#include "sort.h"

int analyze_data(raw_event *data)
{
  double t,tb,tg;
  if(data->tg.h.BGOfold>0) /* BGO in the event */
    if((data->tg.h.BGOHP&(1<<(pos-1)))!=0) /* BGO at the right position in the event */
      if(data->tg.h.Gefold>0) /*Ge in the event */
	if((data->tg.h.GeHP&(1<<(pos-1)))!=0) /*Ge in the right position in the event */
	  if(data->tg.det[pos].ge[col].h.Tfold>0)  /* Ge time information  at the right position and color in the event */
	    if((data->tg.det[pos].ge[col].h.THP&1)!=0) /* Ge core time information  at the right position and color in the event */
	      if(data->tg.det[pos].h.BGOfold>0) /* BGO at the right position in the event */
		if((data->tg.det[pos].h.BGOHP&(1<<col))!=0)  /* BGO at the right position and color in the event */
		  if(data->tg.det[pos].bgo[col].h.Tfold>0)   /* BGO time information at the right position and color in the event */
		    for(sup=0;sup<NSUP;sup++) /* loop over BGO suppressors */
		      if((data->tg.det[pos].bgo[col].h.THP&(1<<sup))!=0) 
			{
			  /* BGO time */
			  tb=data->tg.det[pos].bgo[col].sup[sup].cfd&0x00ffffff;
			  tb-=(data->tg.det[pos].bgo[col].sup[sup].timestamp*16)&0x00ffffff;
			  /* Ge time */
			  tg=data->tg.det[pos].ge[col].seg[0].cfd&0x00ffffff;
			  tg-=(data->tg.det[pos].ge[col].seg[0].timestamp*16)&0x00ffffff;
			  /* relative time */
			  t=tb-tg+S2K;
			  if(t>=0 && t<S4K)
			    hist[sup][(int)rint(t)]++;
			}
  return SEPARATOR_DISCARD;
}
/*=========================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  char n[132];
 
  if(argc!=4)
    {
      printf("Tigress_TGeBGO data_file_name position color\n");
      exit(-1);
    }
  
  printf("Program sorts relative TRaw histograms for Ge and BGO.\n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  memset(hist,0,sizeof(hist));
  pos=atoi(argv[2]);
  col=atoi(argv[3]);
  sort(name);

  sprintf(n,"pos%1d%1d_col%d_GeBGO_time.spn",pos/10,pos-(pos/10)*10,col);

  if((output=fopen(n,"w"))==NULL)
    {
      printf("ERROR! I can't open: %s\n",n);
      exit(EXIT_FAILURE);
    }
  for(sup=0;sup<NSUP;sup++)
    fwrite(hist[sup],S4K*sizeof(int),1,output);
  fclose(output);
}
