#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
  int thp;
  double t;

  for(pos=1;pos<NPIN;pos++)
    if((thp=data->pinarray.h.THP)!=0)
      if(((thp>>pos)&1)!=0)
	{
	  t=data->pinarray.pin[pos].cfd;
	  t-=data->rf.sin.t0;
	  t+=16384.;
	  if(t>=0)
	    if(t<S32K)
	      hist[pos][(int)rint(t)]++;
	}
   
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
 FILE  *output;
 char name[132];

 if(argc!=4)
    {
      printf("\n ./assemble_PINArrayT midas_input_data_file_name  map\n");
      exit(-1);
    }

 /* initialize histograms */
  memset(&hist,0,sizeof(hist));
/* do sorting */
  sort_and_assemble(argv[1],argv[2]);
 /* display results */
  printf("\n"); 

  sprintf(name,"PINArray_raw_time.mca");
  if((output=fopen(name,"w"))==NULL)
    {
      printf("\nI can't open file %s\n",name);
      exit(EXIT_FAILURE);
    }
  for(pos=0;pos<NPIN;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);

  fclose(output);

  

  
}
