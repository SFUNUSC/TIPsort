#include "sort.h"

int analyze_data(raw_event *data)
{
 
  int pos,col,csi;
  
  long long int one=1,none=-1,kill;
  int id,id_ge;
  long long int flag_ge,flag_csi,drop;
  int    flag_pos;
  int rf_ts,rf_ts_up,ts,ts_up;

  if((data->h.setupHP&TIGRESS_BIT)==0)
    return SEPARATOR_DISCARD;

  if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;

 if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;
 
 rf_ts=data->rf.ch.timestamp;
 rf_ts_up=data->rf.ch.timestamp_up;

  flag_pos=0;
  flag_ge=0;
  flag_csi=0;

  /*verify TIGRESS */
  for(pos=1;pos<NPOSTIGR;pos++)
    if((data->tg.h.GeHP&(1<<(pos-1)))!=0)
      if(data->tg.det[pos].h.Gefold>0)
	for(col=0;col<NCOL;col++)
	  if((data->tg.det[pos].h.GeHP&(1<<col))!=0)
	    {
	      ts=data->tg.det[pos].ge[col].seg[0].timestamp;
	      ts_up=data->tg.det[pos].ge[col].seg[0].timestamp_up;
	      if(ts_up==rf_ts_up)
		if(ts==rf_ts)
		  {
		    id=pos-1;
		    flag_pos|=(1<<id);
		    id_ge=id*NCOL+col;
		    flag_ge|=(one<<id_ge);
		  }
	    }

  if(data->csiarray.h.TSfold>0)
    for(csi=1;csi<NCSI;csi++)
      if((data->csiarray.h.TSHP&(one<<csi))!=0)
	{
	  ts=data->csiarray.csi[csi].timestamp;
	  ts_up=data->csiarray.csi[csi].timestamp_up;
	  if(ts_up==rf_ts_up)
	    if(ts==rf_ts)
	      flag_csi|=(one<<csi);
	}
  
  //drop TIGRESS data out of the required limits
   for(pos=1;pos<NPOSTIGR;pos++)
     {
       id=pos-1;
       for(col=0;col<NCOL;col++)
 	 {
 	   id_ge=id*NCOL+col;
 	   drop=(one<<id_ge);
 	   drop&=data->tg.g.GeHP;
 	   if(drop!=0)
 	     {
 	       drop&=flag_ge;
 	       if(drop==0)
 		 {
 		   //drop this crystal
 		   memset(&data->tg.det[pos].ge[col],0,sizeof(SegTIGR));
 		   kill=none-(one<<col);
 		   data->tg.det[pos].h.GeHP&=kill;
 		   data->tg.det[pos].h.Gefold--;
 		   kill=none-(one<<id_ge);
 		   data->tg.g.GeHP&=kill;
 		   data->tg.g.Gefold--;
 		   data->tg.g.THP&=kill;
 		   data->tg.g.Tfold--;
 		 }
 	     }
 	 }
     }

   for(pos=1;pos<NPOSTIGR;pos++)
     {
       id=pos-1;
       if((data->tg.h.GeHP&(1<<id))!=0)
 	 if(data->tg.det[pos].h.Gefold<=0)
 	   {
 	     //drop this position
 	     memset(&data->tg.det[pos],0,sizeof(CssTIGR));
 	     kill=none-(one<<id);
 	     data->tg.h.GeHP&=kill;
 	     data->tg.h.Gefold--;
 	     data->tg.g.PosHP&=kill;
 	     data->tg.g.Posfold--;
 	   }
     }
   
 
   if(data->tg.h.Gefold<=0)
     {
       memset(&data->tg,0,sizeof(Tigress));
       kill=none-TIGRESS_BIT;
       data->h.setupHP&=kill;
     }

  //drop csi out of the time limits
  for(csi=1;csi<NCSI;csi++)
    if((data->csiarray.h.TSHP&(one<<csi))!=0)
      if((flag_csi&(one<<csi))==0)
 	{
 	  memset(&data->csiarray.csi[csi],0,sizeof(channel));
 	  memset(&data->csiarray.wfit[csi],0,sizeof(ShapePar));
 	  memset(&data->csiarray.t0[csi],0,sizeof(double));
 	  data->csiarray.h.Efold--;
 	  data->csiarray.h.Tfold--;
 	  data->csiarray.h.TSfold--;
 	  kill=none-(one<<csi);
 	  data->csiarray.h.TSHP&=kill;
 	  data->csiarray.h.EHP&=kill;
 	  data->csiarray.h.THP&=kill;
 	}
  
  if(data->csiarray.h.TSfold<=0)
    {
      kill=none-CsIArray_BIT;
      data->h.setupHP&=kill;
      memset(&data->csiarray,0,sizeof(CsIArray));
    }

  if((data->h.setupHP&TIGRESS_BIT)==0)
    return SEPARATOR_DISCARD;

  if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;

 if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;

  encode(data,output,enb);

  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  
  input_names_type *name;

  if(argc!=3)
    {
      printf("\n ./separate_RF input.sfu output.sfu\n");
      exit(-1);
    }
 
 
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));

  name->flag.inp_data=1;
  strcpy(name->fname.inp_data,argv[1]);
  name->flag.out_data=1;
  strcpy(name->fname.out_data,argv[2]);

 
 if((output=fopen(name->fname.out_data,"w"))==NULL)
    {
      printf("\nI can't open output file %s for writing\n",name->fname.out_data);
      exit(-2);
    }  
  memset(enb,0,sizeof(enb));
  enb[0]=BUFFER_TAG;
  enb[1]++;
  enb[1]++;

	//save the last buffer which will be dropped otherwise
	//if enb[1]==2 then the buffer contains no data, only the header
	if(enb[1]>2)
		fwrite(enb,sizeof(int),BUFFSIZE,output);

  sort(name);


 
}

