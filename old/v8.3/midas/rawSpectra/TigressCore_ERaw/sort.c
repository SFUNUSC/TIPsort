#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
 int ehp,e;

 for(pos=1;pos<NPOSTIGR;pos++)
   for(col=0;col<NCOL;col++)
     if((ehp=data->tg.det[pos].ge[col].h.EHP)!=0)
       if((ehp&1)!=0)
	 {
	   e=data[0].tg.det[pos].ge[col].seg[0].charge;
	   if(e>=0)
	     if(e<S32K)
	       hist[pos][col][e]++;
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
      printf("\n ./rawSpectra_TigressCoreE midas_input_data_file_name map\n");
      exit(-1);
    }

 /* initialize histograms */
  memset(&hist,0,sizeof(hist));

/* do sorting */
  sort_and_assemble(argv[1],argv[2]);
 /* display results */
  printf("\n"); 
  printf("Program sorts raw energy histograms for Tigress Ge cores\n");
  
   for(pos=1;pos<NPOSTIGR;pos++)
    {
      sprintf(name,"pos%1d%1d_raw_core_energy.mca",pos/10,pos-(pos/10)*10);
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
