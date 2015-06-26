#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double chi,s,f,r,e;


  for(pos=1;pos<NCSI;pos++)
    if((data->csiarray.h.THP&(one<<pos))!=0)
      {
	type=data->csiarray.wfit[pos].type;
	chi=data->csiarray.wfit[pos].chisq;
	chi/=data->csiarray.wfit[pos].ndf;
	if(type>=idmin)
	  if(type<=idmax)
	    if(chi>=chimin)
	      if(chi<=chimax)
		{
		  e=data->csiarray.wfit[pos].am[1];
		  s=data->csiarray.wfit[pos].am[3];
		  f=data->csiarray.wfit[pos].am[2];
		  r=s/f*100+100;
		  if(r>=rmin)
		    if(r<=rmax)
		      if(e>=amin)
			if(e<=amax)
			  {
			    printf("CSI detector No. %12d\n",pos);
			    printf("Trigger Number   %12d\n",data->h.trig_num);
			    printf("Time Stamp Low   %12d\n",data->csiarray.csi[pos].timestamp);
			    printf("Time Stamp Up    %12d\n",data->csiarray.csi[pos].timestamp_up);
			    printf("Fit Type         %12d\n",data->csiarray.wfit[pos].type);
			    printf("ChiSq/NDF        %12.4f\n",chi);
			    printf("Amplitude        %12.4f\n",e);
			    printf("Ratio            %12.4f\n",r);
			    for(int i=0;i<NSHAPE;i++)
			      printf("amp. %12.4f   tau  %12.4f\n",data->csiarray.wfit[pos].am[i],data->csiarray.wfit[pos].t[i]);
			    getc(stdin);

			  }
		}
      }
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{

  input_names_type* name;


  if(argc!=10)
    {
      printf("\n ./CSIARRAY_FindFit sfu_input_data_file_name idmin idmax chimin chimax rmin rmax amin amax\n");
      exit(-1);
    }
 
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  rmin=atof(argv[6]);
  rmax=atof(argv[7]);  
  amin=atof(argv[8]);
  amax=atof(argv[9]);
  printf("Program finds CSIARRAY fits \n");

 
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);

 
}
