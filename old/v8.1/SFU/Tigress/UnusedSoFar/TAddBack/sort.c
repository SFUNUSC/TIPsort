#include "sort.h"

int analyze_data(raw_event *data)
{
  int col1,col2;
  double t,t1,t2;

  if(data->tg.h.Gefold>0) /*Ge in the event */
    if((data->tg.h.GeHP&(1<<(pos-1)))!=0) /*Ge in the right position in the event */
      for(col1=0;col1<NCOL;col1++) /*loop through the first color */
	if(data->tg.det[pos].ge[col1].h.Tfold>0)  /* Ge time information  at the right position and color in the event */
	  if((data->tg.det[pos].ge[col1].h.THP&1)!=0) /* Ge core time information  at the right position and color in the event */
	    {
	      /* Ge time 1*/
	      t1=data->tg.det[pos].ge[col1].seg[0].cfd&0x00ffffff;
	      t1-=(data->tg.det[pos].ge[col1].seg[0].timestamp*16)&0x00ffffff;

	      for(col2=col1+1;col2<NCOL;col2++) /*loop through the second color */
		if(data->tg.det[pos].ge[col2].h.Tfold>0)  /* Ge time information  at the right position and color in the event */
		  if((data->tg.det[pos].ge[col2].h.THP&1)!=0) /* Ge core time information  at the right position and color in the event */
		    {
		      /* Ge time */
		      t2=data->tg.det[pos].ge[col2].seg[0].cfd&0x00ffffff;
		      t2-=(data->tg.det[pos].ge[col2].seg[0].timestamp*16)&0x00ffffff;

		      /* relative time */
		      t=t1-t2+S2K;

		      if((t>=0)&&(t<S4K))				      
			{
			  hist[ind[col1][col2]][(int)rint(t)]++;
			}

		    }
	    }
		  
	      
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  char n[132];
  int k;

  if(argc!=3)
    {
      printf("\n ./Tigress_TAddBack sfu_input_data_file_name position\n");
      exit(-1);
    }
  
  printf("Program sorts raw time histogram for Tigress add back \n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  memset(hist,0,sizeof(hist));
  pos=atoi(argv[2]);

  sort(name);


  sprintf(n,"pos%1d%1d_TAddBack.spn",pos/10,pos-(pos/10)*10);

  if((output=fopen(n,"w"))==NULL)
    {
      printf("\nI can't open file %s\n",n);
      exit(EXIT_FAILURE);
    }
  for(k=0;k<NPAIR;k++)
    fwrite(hist[k],S4K*sizeof(int),1,output);
  fclose(output);




}

