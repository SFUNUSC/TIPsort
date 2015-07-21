#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
 int thp,t;

 for(pos=1;pos<NPOSTIGR;pos++)
   for(col=0;col<NCOL;col++)
     if((thp=data->tg.det[pos].ge[col].h.THP)!=0)
       if((thp&1)!=0)
	 {
	   //gets time as cfd-timestamp with proper masking
	   t=data->tg.det[pos].ge[col].seg[0].cfd&0x00ffffff;
	   t-=(data->tg.det[pos].ge[col].seg[0].timestamp*16)&0x00ffffff;

	   //offset by 16K
	   t+=S16K;

	   //convert to ns
	   //t*=10;
	   //t/=16;

	   if(t<0)
	     t=10;
	   if(t>=S32K)
	     t=S32K-10;

	   if(t>=0)
	     if(t<S32K)
	       hist[pos][col][t]++;
	 }

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

  FILE* output;
  char name[132];

if(argc!=3)
    {
      printf("\n ./rawSpectra_TigressCoreTime midas_input_data_file_name map\n");
      exit(-1);
    }

 /* initialize histograms */
  memset(&hist,0,sizeof(hist));

/* do sorting */
  sort_and_assemble(argv[1],argv[2]);
 /* display results */
  printf("\n"); 
  printf("Program sorts raw CFD time histograms for Tigress Ge cores\n");
  
   for(pos=1;pos<NPOSTIGR;pos++)
    {
      sprintf(name,"pos%1d%1d_raw_core_time.mca",pos/10,pos-(pos/10)*10);
      if((output=fopen(name,"w"))==NULL)
	{
	  printf("\nI can't open file %s\n",name);
	  exit(EXIT_FAILURE);
	}

      for(col=0;col<NCOL;col++)
	{
	  fwrite(hist[pos][col],S32K*sizeof(int),1,output);
	}
      fclose(output);

    }

}
