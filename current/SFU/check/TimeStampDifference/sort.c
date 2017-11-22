#include "sort.h"

int analyze_data(raw_event *data)
{
  int trigger;
  unsigned long long tsRF,tsCsI,tsTIG; //64 bit int
  int pos,col;
  uint64_t one=1;
 
  tsRF=0x0;
  tsCsI=0x0;
  tsTIG=0x0;

  trigger=data->h.trig_num&0x7fffffff;
  printf("==================================\n");
  printf("trigger %d\n",trigger);

  /* get RF timestamp */
  tsRF=((unsigned long long)data->rf.ch.timestamp_up&0x00ffffff)<<24;
  tsRF|=((unsigned long long)data->rf.ch.timestamp&0x00ffffff);

  //printf("RF timestamp is  %16.16llx\n",tsRF);
  printf("RF timestamp is  %6.6x %6.6x\n",data->rf.ch.timestamp_up&0x00ffffff,data->rf.ch.timestamp&0x00ffffff);
  //getc(stdin);

  /* get CsI timestamps */
  if(data->csiarray.h.TSfold>0)
    for(pos=1;pos<NCSI;pos++)
      if((data->csiarray.h.TSHP[pos/64]&(one<<pos%64))!=0)
	{
	  tsCsI=((unsigned long long)data->csiarray.csi[pos].timestamp_up&0x00ffffff)<<24;
	  tsCsI|=((unsigned long long)data->csiarray.csi[pos].timestamp&0x00ffffff);
	  //printf("CsI timestamp is %16.16llx for pos %2d\n",tsCsI,pos);
	  printf("CsI timestamp is %6.6x %6.6x for pos %2d\n",data->csiarray.csi[pos].timestamp_up&0x00ffffff,data->csiarray.csi[pos].timestamp&0x00ffffff);
	  //getc(stdin);
	}

  /* get TIGRESS core timestamps */
  if(data->tg.g.Posfold>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((data->tg.h.GeHP&(1<<(pos-1)))!=0)
	if(data->tg.det[pos].h.Gefold>0)
	  for(col=0;col<NCOL;col++)
	    if((data->tg.det[pos].h.GeHP&(1<<col))!=0)
	      if(data->tg.det[pos].ge[col].h.TSfold>0)
		{
		  tsTIG=((unsigned long long)data->tg.det[pos].ge[col].seg[0].timestamp_up&0x00ffffff)<<24;
		  tsTIG|=((unsigned long long)data->tg.det[pos].ge[col].seg[0].timestamp&0x00ffffff);
		  //printf("TIG timestamp is %16.16llx for pos %2d col %1d\n",tsTIG,pos,col);
		  printf("TIG timestamp is %6.6x %6.6x for pos %2d col %1d\n",data->tg.det[pos].ge[col].seg[0].timestamp_up&0x00ffffff,data->tg.det[pos].ge[col].seg[0].timestamp&0x00ffffff,pos,col);
		}
    
  printf("==================================\n");
  getc(stdin);
  
  return SEPARATOR_DISCARD;
}
/*========================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  
  if(argc!=2)
    {
      printf("check_TimeStampDifference sfu_input_data\n");
      exit(-1);
    }
  
  printf("Program checks time stamp consistency between CsI Array, TIGRESS, and RF.\n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);

  sort(name); 
}
