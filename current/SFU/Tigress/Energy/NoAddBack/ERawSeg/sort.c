#include "sort.h"

int analyze_data(raw_event *data)
{
  int pos,col,sgm,e;
  
  if(data->tg.h.Gefold>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((data->tg.h.GeHP&(1<<(pos-1)))!=0)
				if(data->tg.det[pos].h.Gefold>0)
					for(col=0;col<NCOL;col++)
						if((data->tg.det[pos].h.GeHP&(1<<col))!=0)
							if(data->tg.det[pos].ge[col].h.Efold>0)
								for(sgm=0;sgm<NSEGTIGR;sgm++)
									if((data->tg.det[pos].ge[col].h.EHP&(1<<sgm))!=0)
									//if((data->tg.det[pos].ge[col].h.EHP&1)!=0)
										{
											e=data->tg.det[pos].ge[col].seg[sgm].charge;
											if(e>=0 && e<S32K)
												hist[pos][col][sgm][e]++;
											else hist[pos][col][sgm][S32K-10]++;

										}
  return SEPARATOR_DISCARD;
}
/*=======================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  char n[132];
  int pos,col,sgm;

  if(argc!=2)
    {
      printf("Tigress_ERawSeg sfu_input_data_file_name\n");
      printf("Program sorts raw energy histogram for Tigress segments.\n");
      exit(-1);
    }
  
  printf("Program sorts raw energy histograms for Tigress segments.\n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  memset(hist,0,sizeof(hist));

  sort(name);

  for(pos=1;pos<NPOSTIGR;pos++)
  	for(col=0;col<NCOL;col++)
		  {
		    sprintf(n,"pos%1d%1d_col%1d_ERaw.mca",pos/10,pos-(pos/10)*10,col);
		    if((output=fopen(n,"w"))==NULL)
					{
						printf("ERROR! I cannot open %s\n",n);
						exit(EXIT_FAILURE);
					}
		    
		    for(sgm=0;sgm<NSEGTIGR;sgm++)
					fwrite(hist[pos][col][sgm],S32K*sizeof(int),1,output);
	
		    fclose(output);
		  }

}
