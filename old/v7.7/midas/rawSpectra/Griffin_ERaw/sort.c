#include "sort.h"

int analyze_assembled_event(raw_event* data)
{
  int ehp,e;
  
  for(pos=1;pos<NPOSGRIF;pos++)
    for(col=0;col<NCOL;col++)
      {
	ehp=data->gr.det[pos].ge[col].h.EHP;
	for(seg=0;seg<NSEGGRIF;seg++) 
	  if((ehp&(1<<seg))!=0)
	    {
	      e=data->gr.det[pos].ge[col].seg[seg].charge;
	      if(e>=0 && e<S32K) hist[pos][col][seg][e]++;
	      else 
		{
		  hist[pos][col][seg][S32K-1000]++;
		  printf("Bad energy value of %d!\n",e);
		  if(e<2*S32K) count_low++;
		  else count_high++;
		}}}
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  FILE* output;
  char name[132];
  
  if(argc!=3)
    {
      printf("rawSpectra_GriffinERaw midas_input_data map\n");
      exit(-1);
    }
  
  memset(&hist,0,sizeof(hist));
  sort_and_assemble(argv[1],argv[2]);
  printf("Program sorts raw energy histograms for GRIFFIN Ge from midas file.\n");

  for(pos=1;pos<NPOSGRIF;pos++)
    for(col=0;col<NCOL;col++) 
      {
	sprintf(name,"pos%1d%1d_col%d_ERaw.mca",pos/10,pos-(pos/10)*10,col);
	if((output=fopen(name,"w"))==NULL)
	  {
	    printf("ERROR!!! I can't open the mca file!\n");
	    exit(EXIT_FAILURE);
	  }
	for(seg=0;seg<NSEGGRIF;seg++)
	  fwrite(hist[pos][col][seg],S32K*sizeof(int),1,output);
	fclose(output);
      }
  printf("The number of bad energies was %d.\n Low and High were %d %d.\n",
	 count_low+count_high,count_low,count_high);
}
