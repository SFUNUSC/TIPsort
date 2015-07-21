#include "SFU-common.h"
#include "SFU-format.h"
/*===============================================================*/
int verify_ts(raw_event *d,unsigned long long int *min, unsigned long long int *max )
{
  unsigned long long int one=1,ts_min=one<<63,ts_max=0,x,y;
  
  for(int i=0;i<MAX_DET_BITS;i++)
    switch(d->h.setupHP&(one<<i))
      {
      case TIGRESS_BIT:
	verify_ts_Tigress(&d->tg,&x,&y);
	if(x<ts_min) ts_min=x;
	if(y>ts_max) ts_max=y;	
	break;
      case GRIFFIN_BIT:
	verify_ts_Griffin(&d->gr,&x,&y);
	if(x<ts_min) ts_min=x;
	if(y>ts_max) ts_max=y;	
	break;
      case PINArray_BIT:
	verify_ts_pinarray(&d->pinarray,&x,&y);
	if(x<ts_min) ts_min=x;
	if(y>ts_max) ts_max=y;
	break;
      case CsIArray_BIT:
	verify_ts_csiarray(&d->csiarray,&x,&y);
	if(x<ts_min) ts_min=x;
	if(y>ts_max) ts_max=y;
	break;
      case BDPIN_BIT:
	verify_ts_PINBD(&d->pinbd,&x,&y);
	if(x<ts_min) ts_min=x;
	if(y>ts_max) ts_max=y;
	break;
      case RF_BIT:
	verify_ts_RF(&d->rf,&x,&y);
	if(x<ts_min) ts_min=x;
	if(y>ts_max) ts_max=y;
	break;
      case S3SEC_BIT ... S3RING_BIT:
	verify_ts_S3(&d->s3,&x,&y);
	if(x<ts_min) ts_min=x;
	if(y>ts_max) ts_max=y;
	break;
      default:
	break;
      }
  
  *min=ts_min;
  *max=ts_max;
  return (int)(*max-*min);
}
/*===============================================================*/
int verify_ts_PINBD(PINBD*d,unsigned long long int *min, unsigned long long int *max )
{
  unsigned long long int ts;
  
  ts=((unsigned long long)d->pin.timestamp_up&0x00ffffff)<<24;
  ts|=((unsigned long long)d->pin.timestamp&0x00ffffff);
  *min=ts;
  *max=ts;
  return (int)(*max-*min);
}
/*===============================================================*/
int verify_ts_RF(RF*d,unsigned long long int *min, unsigned long long int *max )
{
  unsigned long long int ts;

  ts=((unsigned long long)d->ch.timestamp_up&0x00ffffff)<<24;
  ts|=((unsigned long long)d->ch.timestamp&0x00ffffff);
  *min=ts;
  *max=ts;
  return (int)(*max-*min);
}
/*===============================================================*/
int verify_ts_pinarray(PINArray*d,unsigned long long int *min, unsigned long long int *max )
{
  unsigned long long int one=1;
  unsigned long long int ts;

  *min=(one<<63);
  *max=0;
  if(d->h.TSfold>0)
    for(int pos=1;pos<NPIN;pos++)
      if((d->h.TSHP&(one<<pos))!=0)
	{
	  ts=((unsigned long long)d->pin[pos].timestamp_up&0x00ffffff)<<24;
	  ts|=((unsigned long long)d->pin[pos].timestamp&0x00ffffff);
	  if(ts>*max) *max=ts;
	  if(ts<*min) *min=ts;
	 }
  
  return (int)(*max-*min);
}
/*===============================================================*/
int verify_ts_csiarray(CsIArray*d,unsigned long long int *min, unsigned long long int *max )
{
  unsigned long long int one=1;
  unsigned long long int ts;

  *min=(one<<63);
  *max=0;
  if(d->h.TSfold>0)
    for(int pos=1;pos<NCSI;pos++)
      if((d->h.TSHP&(one<<pos))!=0)
	{
	  ts=((unsigned long long)d->csi[pos].timestamp_up&0x00ffffff)<<24;
	  ts|=((unsigned long long)d->csi[pos].timestamp&0x00ffffff);
	  if(ts>*max) *max=ts;
	  if(ts<*min) *min=ts;
	 }
  
  return (int)(*max-*min);
}
/*===============================================================*/
int verify_ts_S3(S3*d,unsigned long long int *min, unsigned long long int *max )
{
  unsigned long long int one=1;
  unsigned long long int ts;

  *min=(one<<63);
  *max=0;
  if(d->sh.TSfold>0)
    for(int pos=1;pos<NS3SEC;pos++)
      if((d->sh.TSHP&(one<<pos))!=0)
	{
	  ts=((unsigned long long)d->sec[pos].timestamp_up&0x00ffffff)<<24;
	  ts|=((unsigned long long)d->sec[pos].timestamp&0x00ffffff);
	  if(ts>*max) *max=ts;
	  if(ts<*min) *min=ts;
	 }

  if(d->rh.TSfold>0)
    for(int pos=1;pos<NS3RING;pos++)
      if((d->rh.TSHP&(one<<pos))!=0)
	{
	  ts=((unsigned long long)d->ring[pos].timestamp_up&0x00ffffff)<<24;
	  ts|=((unsigned long long)d->ring[pos].timestamp&0x00ffffff);
	  if(ts>*max) *max=ts;
	  if(ts<*min) *min=ts;
	 }
  
  return (int)(*max-*min);
}
/*===============================================================*/
int verify_ts_Tigress(Tigress *d,unsigned long long int *min, unsigned long long int *max )
{
  unsigned long long int ts,one=1;
  int pos,col,seg,sup;

  *min=(one<<63);
  *max=0;

  if(d->g.Posfold>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((d->h.GeHP&(1<<(pos-1)))!=0)
	{
	  /*check Ge */
	  if(d->det[pos].h.Gefold>0)
	    for(col=0;col<NCOL;col++)
	      if((d->det[pos].h.GeHP&(1<<col))!=0)
		if(d->det[pos].ge[col].h.TSfold>0)
		  for(seg=0;seg<NSEGTIGR;seg++)
		    if((d->det[pos].ge[col].h.TSHP&(1<<seg))!=0)
		      {
			ts=((unsigned long long)d->det[pos].ge[col].seg[seg].timestamp_up&0x00ffffff)<<24;
			ts|=((unsigned long long)d->det[pos].ge[col].seg[seg].timestamp&0x00ffffff);
			if(ts>*max) *max=ts;
			if(ts<*min) *min=ts;
		      }
	  /* check BGO */
	  if(d->det[pos].h.BGOfold>0)
	    for(col=0;col<NCOL;col++)
	      if((d->det[pos].h.BGOHP&(1<<col))!=0)
		if(d->det[pos].bgo[col].h.TSfold>0)
		  for(sup=0;sup<NSUP;sup++)
		    if((d->det[pos].bgo[col].h.TSHP&(1<<sup))!=0)
		      {
			ts=((unsigned long long)d->det[pos].bgo[col].sup[sup].timestamp_up&0x00ffffff)<<24;
			ts|=((unsigned long long)d->det[pos].bgo[col].sup[sup].timestamp&0x00ffffff);
			if(ts>*max) *max=ts;
			if(ts<*min) *min=ts;
		      }
	}
  
  return (int)(*max-*min);

}
/*===============================================================*/
int verify_ts_Griffin(Griffin *d,unsigned long long int *min, unsigned long long int *max )
{
  unsigned long long int ts,one=1;
  int pos,col,seg;

  *min=(one<<63);
  *max=0;

  if(d->g.Posfold>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((d->h.GeHP&(1<<(pos-1)))!=0)
	{
	  /*check Ge */
	  if(d->det[pos].h.Gefold>0)
	    for(col=0;col<NCOL;col++)
	      if((d->det[pos].h.GeHP&(1<<col))!=0)
		if(d->det[pos].ge[col].h.TSfold>0)
		  for(seg=0;seg<NSEGGRIF;seg++)
		    if((d->det[pos].ge[col].h.TSHP&(1<<seg))!=0)
		      {
			ts=((unsigned long long)d->det[pos].ge[col].seg[seg].timestamp_up&0x00ffffff)<<24;
			ts|=((unsigned long long)d->det[pos].ge[col].seg[seg].timestamp&0x00ffffff);
			if(ts>*max) *max=ts;
			if(ts<*min) *min=ts;
		      }

	}
  
  return (int)(*max-*min);

}
/*===============================================================*/
void display_raw_event(raw_event* d)
{
  display_rheader(&d->h);
  for(int i=0;i<MAX_DET_BITS;i++)
    switch(d->h.setupHP&(1<<i))
      {
      case TIGRESS_BIT:
  	display_Tigress(&d->tg);
  	break;
      case GRIFFIN_BIT:
  	display_Griffin(&d->gr);
  	break;
      case PINArray_BIT:
  	display_pinarray(&d->pinarray);
  	break;
      case CsIArray_BIT:
  	display_csiarray(&d->csiarray);
  	break;
      case BDPIN_BIT:
  	display_pinbd(&d->pinbd);
  	break;
      case RF_BIT:
  	display_RF(&d->rf);
  	break;
      case S3SEC_BIT...S3RING_BIT:
  	display_S3(&d->s3);
  	break;
      default:
  	break;
      }
}
/*===============================================================*/
void display_rheader(rheader* d)
{

  printf("================================================================\n");
  printf("Event number                      %8d\n",d->trig_num&0x7fffffff);
  printf("Fragment fold                     %8d\n",d->frag_fold);
  printf("Trigger bits                      %8d 0x%8.8x\n",d->setupHP,d->setupHP);
  printf("Time Stamp High Min               %8.8x\n",d->timestamp_up_min&0x00ffffff);
  printf("Time Stamp High Max               %8.8x\n",d->timestamp_up_max&0x00ffffff);
  printf("Time Stamp Low Min                %8.8x\n",d->timestamp_min&0x00ffffff);
  printf("Time Stamp Low Max                %8.8x\n",d->timestamp_max&0x00ffffff);  ;
  printf("Data for: ");
  for(int i=0;i<MAX_DET_BITS;i++)
    switch(d->setupHP&(1<<i))
      {
      case TIGRESS_BIT:
	printf(" TIGRESS ");
	break;
      case GRIFFIN_BIT:
	printf(" GRIFFIN ");
	break;
      case PINArray_BIT:
	printf(" PINArray ");
	break;
      case RF_BIT:
	printf(" RF ");
	break;
      case BDPIN_BIT:
	printf(" Beam Dump PIN ");
	break;
      case S3SEC_BIT:
	printf(" S3 sectors ");
	break;
      case S3RING_BIT:
	printf(" S3 rings ");
	break;
      case CsIArray_BIT:
	printf(" CsIArray ");
	break;
      default:
	break;
      }
  printf("\n");
}
/*===============================================================*/
void display_channel(int i,channel* d)
{
  printf(" %4d     %8.8x  %8.8x  %8.8x  %8.8x\n",i,d->timestamp_up&0x00ffffff,d->timestamp&0x00ffffff,d->cfd,d->charge);  
}
/*===============================================================*/
void display_RF(RF* d)
{
  printf("--->      RF HIT          <--------------------------------------\n");
  printf("Sine wave parameters:  Phase     Amplitude     Offset\n");
  printf("                  %10.3f   %10.3f    %10.3f\n",d->sin.t0,d->sin.A,d->sin.C);
  display_channel_legend();
  display_channel(1,&d->ch);
}
/*===============================================================*/
void display_channel_legend()
{
  printf("Channel    TS_high   TS low     CFD      Charge\n");
}
/*===============================================================*/
void display_segment_legend()
{
  printf("Segment    TS_high   TS low     CFD      Charge\n");
}
/*===============================================================*/
void display_suppressor_legend()
{
  printf("Suppressor TS_high   TS low     CFD      Charge\n");
}
/*===============================================================*/
void display_pheader(pheader* d)
{
  printf("Energy     fold                   %4.4x\n",d->Efold);
  printf("CFD        fold                   %4.4x\n",d->Tfold);
  printf("Time Stamp fold                   %4.4x\n",d->TSfold);  
  printf("Energy hit pattern                %12.12llx\n",d->EHP);
  printf("CFD hit pattern                   %12.12llx\n",d->THP);
  printf("Time Stamp hit pattern            %12.12llx\n",d->TSHP);  
 
}
/*===============================================================*/
void display_pinarray(PINArray* d)
{									
  unsigned long one=1;
  printf("--->   PINArray HIT       <--------------------------------------\n");
  display_pheader(&d->h);
  display_channel_legend();
  for(int i=0;i<NPIN;i++)
    if((d->h.TSHP&(one<<i))!=0)
       display_channel(i,&d->pin[i]);
}
/*===============================================================*/
void display_csiarray(CsIArray* d)
{									
  unsigned long one=1;
  printf("--->   CsIArray HIT       <--------------------------------------\n");
  display_pheader(&d->h);
  display_channel_legend();
  for(int i=0;i<NCSI;i++)
    if((d->h.TSHP&(one<<i))!=0)
      {
       display_channel(i,&d->csi[i]);
       printf("T0 for channel %d is %f\n",i,d->t0[i]);
      }
}
/*===============================================================*/
void display_Tigress(Tigress* d)
{
  int id,pos;
  printf("--->   TIGRESS HIT      <----------------------------------------\n");
  display_theader(&d->h);
  display_gheader(&d->g);

  for(id=0;id<NPOSTIGR-1;id++)
    {
      pos=id+1;
      if((d->g.PosHP&(1<<id))!=0)
	{
  printf("___ Position #%2d has data _____________________________________\n",pos);
	  display_Css(&d->det[pos]);
	}
    }
}
/*===============================================================*/
void display_Griffin(Griffin* d)
{
  int id,pos;
  printf("--->   GRIFFIN HIT      <----------------------------------------\n");
  display_GRIF_theader(&d->h);
  display_GRIF_gheader(&d->g);

  for(id=0;id<NPOSGRIF-1;id++)
    {
      pos=id+1;
      if((d->g.PosHP&(1<<id))!=0)
	{
  printf("___ Position #%2d has data _____________________________________\n",pos);
	  display_GCss(&d->det[pos]);
	}
    }
}
/*===============================================================*/
void display_theader(theader* d)
{
  printf("Ge  position fold                 %4.4x\n",d->Gefold);
  printf("BGO position fold                 %4.4x\n",d->BGOfold); 
  printf("Ge  position hit pattern          %4.4x\n",d->GeHP);
  printf("BGO position hit pattern          %4.4x\n",d->BGOHP);
}
/*===============================================================*/
void display_GRIF_theader(theader* d)
{
  printf("Ge  position fold                 %4.4x\n",d->Gefold);
  printf("Ge  position hit pattern          %4.4x\n",d->GeHP);
}
/*===============================================================*/
void display_cheader(theader* d)
{
  printf("Ge  color fold                    %4.4x\n",d->Gefold);
  printf("BGO color fold                    %4.4x\n",d->BGOfold); 
  printf("Ge  color hit pattern             %4.4x\n",d->GeHP);
  printf("BGO color hit pattern             %4.4x\n",d->BGOHP);
}
/*===============================================================*/
void display_GRIF_cheader(theader* d)
{
  printf("Ge  color fold                    %4.4x\n",d->Gefold);
  printf("Ge  color hit pattern             %4.4x\n",d->GeHP);
}
/*===============================================================*/
void display_gheader(gheader* d)
{
  printf("Tigress position fold             %4.4x\n",d->Posfold);
  printf("Tigress crystal  fold             %4.4x\n",d->Tfold); 
  printf("Ge      crystal  fold             %4.4x\n",d->Gefold);
  printf("BGO     crystal  fold             %4.4x\n",d->BGOfold);
  printf("Tigress position hit pattern      %4.4x\n",d->PosHP); 
  printf("Tigress crystal  hit pattern      %16.16llx\n",d->THP); 
  printf("Ge      crystal  hit pattern      %16.16llx\n",d->GeHP);
  printf("BGO     crystal  hit pattern      %16.16llx\n",d->BGOHP);
}
/*===============================================================*/
void display_GRIF_gheader(gheader* d)
{
  printf("Griffin position fold             %4.4x\n",d->Posfold);
  printf("Griffin crystal  fold             %4.4x\n",d->Tfold); 
  printf("Ge      crystal  fold             %4.4x\n",d->Gefold);
  printf("Griffin position hit pattern      %4.4x\n",d->PosHP); 
  printf("Griffin crystal  hit pattern      %16.16llx\n",d->THP); 
  printf("Ge      crystal  hit pattern      %16.16llx\n",d->GeHP);

}
/*===============================================================*/
void display_Css(CssTIGR* d)
{
  char colnames[4][6]={"Blue","Green","Red","White"};
  int col,seg,sup;
  display_cheader(&d->h);
  if(&d->h.Gefold>0)
    for(col=0;col<NCOL;col++)
      if((d->h.GeHP&(1<<col))!=0)
	{
	  printf("~~~~ %6s  Ge has data  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n",colnames[col]);
	  display_header(&d->ge[col].h);
	  display_segment_legend();
	  for(seg=0;seg<NSEGTIGR;seg++)
	    if((d->ge[col].h.TSHP&(1<<seg))!=0)
	      {
		display_channel(seg,&d->ge[col].seg[seg]);
		printf("t0: %f \n",d->ge[col].t0[seg]);
	      }     	    
	}

  if(&d->h.BGOfold>0)
    for(col=0;col<NCOL;col++)
      if((d->h.BGOHP&(1<<col))!=0)
	{
	  printf("~~~~ %6s BGO has data  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n",colnames[col]);
	  display_bheader(&d->bgo[col].h);
	  display_suppressor_legend();
	  for(sup=0;sup<NSUP;sup++)
	    if((d->bgo[col].h.TSHP&(1<<sup))!=0)
	      {
	      display_channel(sup,&d->bgo[col].sup[sup]);
	      printf("t0: %f \n",d->bgo[col].t0[sup]);
	      }
			
	}
}
/*===============================================================*/
void display_GCss(CssGRIF* d)
{
  char colnames[4][6]={"Blue","Green","Red","White"};
  int col,seg;
  display_GRIF_cheader(&d->h);
  if(&d->h.Gefold>0)
    for(col=0;col<NCOL;col++)
      if((d->h.GeHP&(1<<col))!=0)
	{
	  printf("~~~~ %6s  Ge has data  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n",colnames[col]);
	  display_header(&d->ge[col].h);
	  display_segment_legend();
	  for(seg=0;seg<NSEGGRIF;seg++)
	    if((d->ge[col].h.TSHP&(1<<seg))!=0)
	      {
		display_channel(seg,&d->ge[col].seg[seg]);	    
		printf("T0 for segment %d is %f\n",seg,d->ge[col].t0[seg]);
	      }
	}
 
}
/*===============================================================*/
void display_header(header* d)
{
  printf("Segment Energy fold               %4.4x\n",d->Efold);
  printf("Segment CFD fold                  %4.4x\n",d->Tfold);
  printf("Segment Time Stamp fold           %4.4x\n",d->TSfold);  
  printf("Segment Energy hit pattern        %8.8x\n",d->EHP);
  printf("Segment CFD hit pattern           %8.8x\n",d->THP);
  printf("Segment Time Stamp hit pattern    %8.8x\n",d->TSHP);  
}
/*===============================================================*/
void display_pbdheader(header* d)
{
  printf("PIN Energy fold                   %4.4x\n",d->Efold);
  printf("PIN CFD fold                      %4.4x\n",d->Tfold);
  printf("PIN Time Stamp fold               %4.4x\n",d->TSfold);  
  printf("PIN Energy hit pattern            %8.8x\n",d->EHP);
  printf("PIN CFD hit pattern               %8.8x\n",d->THP);
  printf("PIN Time Stamp hit pattern        %8.8x\n",d->TSHP);  
}
/*===============================================================*/
void display_bheader(header* d)
{
  printf("Suppressor Energy fold            %4.4x\n",d->Efold);
  printf("Suppressor CFD fold               %4.4x\n",d->Tfold);
  printf("Suppressor Time Stamp fold        %4.4x\n",d->TSfold);  
  printf("Suppressor Energy hit pattern     %8.8x\n",d->EHP);
  printf("Suppressor CFD hit pattern        %8.8x\n",d->THP);
  printf("Suppressor Time Stamp hit pattern %8.8x\n",d->TSHP);  
}
/*===============================================================*/
void display_pinbd(PINBD* d)
{									
  printf("--->   BEAM DUMP PIN HIT     <-----------------------------------\n");
  display_pbdheader(&d->h);
  display_channel_legend();
  display_channel(1,&d->pin);
}
/*===============================================================*/
void display_S3(S3* d)
{									
  unsigned long one=1;
  printf("--->   S3 HIT       <--------------------------------------\n");
  if(d->sh.TSfold>0)
    {
      printf("_______Sector data\n");
      display_pheader(&d->sh);
      display_channel_legend();
      for(int i=1;i<NS3SEC;i++)
	if((d->sh.TSHP&(one<<i))!=0)
	  display_channel(i,&d->sec[i]);
    }

  if(d->rh.TSfold>0)
    {
      printf("________Ring data\n");
      display_pheader(&d->rh);
      display_channel_legend();
      for(int i=1;i<NS3RING;i++)
	if((d->rh.TSHP&(one<<i))!=0)
	  display_channel(i,&d->ring[i]);
    }
}
