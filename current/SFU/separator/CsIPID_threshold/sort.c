#include "sort.h"

int analyze_data(raw_event *data)
{
  uint64_t one=1;
  double s,f;
  double r=0;
  int type;
  
  if((data->h.setupHP&RF_BIT)==0) 
    return SEPARATOR_DISCARD;

  if((data->h.setupHP&CsIArray_BIT)==0) 
    return SEPARATOR_DISCARD;

  for(pos=1;pos<NCSI;pos++)
    if((data->csiarray.h.THP[pos/64]&(one<<pos%64))!=0)
      {
	type=data->csiarray.wfit[pos].type;
	if(type==1)
	  {
	    //e=data->csiarray.wfit[pos].am[1];
	    s=data->csiarray.wfit[pos].am[3];
	    f=data->csiarray.wfit[pos].am[2];
	    
	    /* for(int i=1;i<=4;i++) */
	    /* printf("amp[%d] %Lf\n",i,data->csiarray.wfit[pos].am[i]); */
	    
	    if(f==0)
	      r=0;
	    else
	      r=s/f*100;
	    
	    r+=100;

	  }
      }
  
	if(thrtype==0)
		if(r<=threshold)
			{
				encode(data,output,enb);
				//printf("Event encoded with np %d na %d\n\nEND OF EVENT\n",np,na);
			}
	if(thrtype==1)
		if(r>=threshold)
			{
				encode(data,output,enb);
				//printf("Event encoded with np %d na %d\n\nEND OF EVENT\n",np,na);
			}    
	return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;

  if(argc!=4)
    {
      printf("\n ./separate_CsIArray_PID_threshold master_file_name threshold threshold_type\n");
      exit(-1);
    }
 
  printf("Program separates events according to the specified PID threshold.\n");
  printf("If threshold_type is 0, then only event below the PID theshold will be taken.  Otherwise events above the threshold will be taken.\n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));

  read_master(argv[1],name);
  threshold=atof(argv[2]);
  thrtype=atoi(argv[3]);

  if(name->flag.inp_data!=1)
    {
      printf("\nInput data file not defined\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.out_data!=1)
    {
      printf("\nOutput data file not defined\n");
      exit(EXIT_FAILURE);
    }

  if((output=fopen(name->fname.out_data,"w"))==NULL)
    {
      printf("\nI can't open output file %s for writing\n",name->fname.out_data);
      exit(-2);
    }

  memset(enb,0,sizeof(enb));
  enb[0]=BUFFER_TAG;
  enb[1]++;
  enb[1]++;

  sort(name);
  
	//save the last buffer which will be dropped otherwise
	//if enb[1]==2 then the buffer contains no data, only the header
	if(enb[1]>2)
		fwrite(enb,sizeof(int),BUFFSIZE,output);
 
}

