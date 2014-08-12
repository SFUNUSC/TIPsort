#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
 int thp;
 double t;
 
 for(pos=1;pos<NPOSTIGR;pos++)
   for(col=0;col<NCOL;col++)
     {
       thp=data->tg.det[pos].ge[col].h.THP;
       for(seg=0;seg<NSEGTIGR;seg++) 
	 if((thp&(1<<seg))!=0)
	   {
	     t=data->tg.det[pos].ge[col].seg[seg].cfd;
	     t-=data->rf.sin.t0;
	     t+=S32K/2.;
	     if(t>=0)
	       if(t<S32K)
		 hist[pos][col][seg][(int)rint(t)]++;
	   }
     }


  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  FILE *output;
  int pos,col;
  char name[132];

 if(argc!=3)
    {
      printf("\n ./assemble_TigressSegT midas_input_data_file_name map\n");
      exit(-1);
    }

 /* initialize histograms */
  memset(&hist,0,sizeof(hist));

/* do sorting */
  sort_and_assemble(argv[1],argv[2]);
 /* display results */
  printf("\n"); 

 for(pos=1;pos<NPOSTIGR;pos++)
    for(col=0;col<NCOL;col++) 
      {
	sprintf(name,"pos%1d%1d_col%d_seg_time.mca",pos/10,pos-(pos/10)*10,col);
	if((output=fopen(name,"w"))==NULL)
	  {
	    printf("\nI can't open file %s\n",name);
	    exit(EXIT_FAILURE);
	  }
	for(seg=0;seg<NSEGTIGR;seg++)
	  fwrite(hist[pos][col][seg],S32K*sizeof(int),1,output);
	fclose(output);

    }



  
}
