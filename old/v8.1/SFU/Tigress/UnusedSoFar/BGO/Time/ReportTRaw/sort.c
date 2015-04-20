#include "sort.h"

int analyze_data(raw_event *data)
{

  double t,tb;
  if(data->tg.h.BGOfold>0)
    if((data->tg.h.BGOHP&(1<<(pos-1)))!=0)
      if(data->tg.det[pos].h.BGOfold>0)
	if((data->tg.det[pos].h.BGOHP&(1<<col))!=0)
	  if(data->tg.det[pos].bgo[col].h.Tfold>0)
	    if((data->tg.det[pos].bgo[col].h.THP&(1<<sup))!=0)
	      {
		t=data->tg.det[pos].bgo[col].sup[sup].cfd&0x00ffffff;
		t-=(data->tg.det[pos].bgo[col].sup[sup].timestamp*16)&0x00ffffff;
		if((data->h.setupHP&RF_BIT)!=0)
		  {
		    t-=(int)data->rf.sin.t0;
			t+=S16K;
			if(t<0)t=S32K-2;
			if(t>S32K) t=S32K-3;
		  }
		else
		  t=S32K-4;
		if(t>=tlow)
		  if(t<=thigh)
		    {
		      printf("Match in trigger number %8d\n",data->h.trig_num);
		      printf("T                       %8d\n",(int)t);
		      tb=data->tg.det[pos].bgo[col].sup[sup].cfd&0x00ffffff;
		      tb-=(data->tg.det[pos].bgo[col].sup[sup].timestamp*16)&0x00ffffff;
		      printf("TBGO                    %8d\n",(int)tb);
		      if((data->h.setupHP&RF_BIT)!=0)
			{
			  printf("TRF                     %8d\n",(int)data->rf.sin.t0);
			}
		      else
			printf("TRF failed fit\n");
		    }
	      }
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;

 
  if(argc!=7)
    {
      printf("\n ./Tigress_ReportTBGORaw sfu_input_data_file_name pos col sup tlow thigh\n");
      exit(-1);
    }
  
  printf("Program reports trigger number for a BGO time gate \n");

  pos=atoi(argv[2]);
  col=atoi(argv[3]);
  sup=atoi(argv[4]);
  tlow=atoi(argv[5]);
  thigh=atoi(argv[6]);
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);

  sort(name);

 

}

