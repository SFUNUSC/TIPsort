#include "SFU-encoder.h"

/*================================================================*/
int encode(raw_event* data, FILE* encoded_output,int* enb)
{
  int e[BUFFSIZE],pck[BUFFSIZE];
  unsigned long long int one=1;

  if(data->h.trig_num!=0)
    { 
      memset(e,0,sizeof(e));
      e[0]=EVENT_TAG;
      e[1]++;
      e[1]++;

      /* pack the header */
      memset(pck,0,sizeof(pck));
      pck[0]=HEADER_TAG;
      pck[1]=2+sizeof(rheader)/sizeof(int);
      memcpy(&pck[2],&data->h,sizeof(rheader));
      if(((e[1]+pck[1])<BUFFSIZE)&&(pck[1]<BUFFSIZE)) /* buffer has space and not overflow         */
	{                                             /* e[1] behavior undefined when pck overflow */
	                                              /* added by ASC 20 Feb 2018.                 */ 
	  memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
	  e[1]+=pck[1];
	}	  
      else
	{
	  printf("Event dropped due to the length restriction:\n");
	  printf("Event (header) length is %d BUFFSIZE is %d\n",e[1]+pck[1],BUFFSIZE);
	  /* getc(stdin); */
	  return 0;
	}
      
      /* pack the RF */
      if((data->h.setupHP&RF_BIT)!=0)
	{
	  memset(pck,0,sizeof(pck));
	  pck[0]=RF_TAG;
	  pck[1]=2+sizeof(RF)/sizeof(int);
	  memcpy(&pck[2],&data->rf,sizeof(RF));
	  if(((e[1]+pck[1])<BUFFSIZE)&&(pck[1]<BUFFSIZE)) /* buffer has space and not overflow */
	    {
	      memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
	      e[1]+=pck[1];
	    }	  
	  else
	    {
	      printf("Event dropped due to the length restriction:\n");
	      printf("Event (RF) length is %d BUFFSIZE is %d\n",e[1]+pck[1],BUFFSIZE);
	      /* getc(stdin); */
	      return 0;
	    }
	}
      
      /* pack TIGRESS */
      if((data->h.setupHP&TIGRESS_BIT)!=0)
	{
	  memset(pck,0,sizeof(pck));
	  pck[0]=TIGRESS_TAG;
	  pck[1]=2+(sizeof(gheader)+sizeof(theader))/sizeof(int);
	  memcpy(&pck[2],&data->tg,(sizeof(gheader)+sizeof(theader)));
	  memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
	  e[1]+=pck[1];
	  /* pack CSS */
	  if(data->tg.g.Posfold>0)
	    for(int pos=1;pos<NPOSTIGR;pos++)
	      if((data->tg.g.PosHP&(1<<(pos-1)))!=0)	
		{
		  memset(pck,0,sizeof(pck));
		  pck[0]=CSSTIGR_TAG;
		  pck[1]=3+sizeof(theader)/sizeof(int);
		  pck[2]=pos;
		  memcpy(&pck[3],&data->tg.det[pos].h,sizeof(theader));
		  if(((e[1]+pck[1])<BUFFSIZE)&&(pck[1]<BUFFSIZE)) /* buffer has space and not overflow */
		    {
		      memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
		      e[1]+=pck[1];
		    }	  
		  else
		    {
		      printf("Event dropped due to the length restriction:\n");
		      printf("Event (TIGRESS CSS) length is %d BUFFSIZE is %d\n",e[1]+pck[1],BUFFSIZE);
		      /* getc(stdin); */
		      return 0;
		    }
		  /* pack Ge */
		  if(data->tg.det[pos].h.Gefold>0)
		    for(int col=0;col<NCOL;col++)
		      if((data->tg.det[pos].h.GeHP&(1<<col))!=0)
			{
			  memset(pck,0,sizeof(pck));
			  pck[0]=TIGRGe_TAG;
			  pck[1]=3+sizeof(header)/sizeof(int);
			  pck[2]|=(pos<<16);
			  pck[2]|=col;
			  memcpy(&pck[3],&data->tg.det[pos].ge[col].h,sizeof(header));		  
			  if(data->tg.det[pos].ge[col].h.TSfold>0)
			    for(int seg=0;seg<NSEGTIGR;seg++)
			      if((data->tg.det[pos].ge[col].h.TSHP&(1<<seg))!=0)
				{
				  pck[pck[1]]=CHANNEL_TAG;
				  pck[1]++;
				  pck[pck[1]]=seg;
				  pck[1]++;
				  memcpy(&pck[pck[1]],&data->tg.det[pos].ge[col].seg[seg],sizeof(channel));
				  pck[1]+=sizeof(channel)/sizeof(int);
     				  memcpy(&pck[pck[1]],&data->tg.det[pos].ge[col].t0[seg],sizeof(double));
      				  pck[1]+=sizeof(double)/sizeof(int);
				}
			  if(((e[1]+pck[1])<BUFFSIZE)&&(pck[1]<BUFFSIZE)) /* buffer has space and not overflow */
			    {
			      memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
			      e[1]+=pck[1];
			    }	  
			  else
			    {
			      printf("Event dropped due to the length restriction:\n");
			      printf("Event (TIGRESS HPGe) length is %d BUFFSIZE is %d\n",e[1]+pck[1],BUFFSIZE);
			      /* getc(stdin); */
			      return 0;
			    }
			}
		  /* pack BGO */
		  if(data->tg.det[pos].h.BGOfold>0)
		    for(int col=0;col<NCOL;col++)
		      if((data->tg.det[pos].h.BGOHP&(1<<col))!=0)
			{
			  memset(pck,0,sizeof(pck));
			  pck[0]=BGO_TAG;
			  pck[1]=3+sizeof(header)/sizeof(int);
			  pck[2]|=(pos<<16);
			  pck[2]|=col;
			  memcpy(&pck[3],&data->tg.det[pos].bgo[col].h,sizeof(header));		  
			  if(data->tg.det[pos].bgo[col].h.TSfold>0)
			    for(int sup=0;sup<NSUP;sup++)
			      if((data->tg.det[pos].bgo[col].h.TSHP&(1<<sup))!=0)
				{
				  pck[pck[1]]=CHANNEL_TAG;
				  pck[1]++;
				  pck[pck[1]]=sup;
				  pck[1]++;
				  memcpy(&pck[pck[1]],&data->tg.det[pos].bgo[col].sup[sup],sizeof(channel));
				  pck[1]+=sizeof(channel)/sizeof(int);
    				  memcpy(&pck[pck[1]],&data->tg.det[pos].bgo[col].t0[sup],sizeof(double));		    
      				  pck[1]+=sizeof(double)/sizeof(int);
				}
			  if(((e[1]+pck[1])<BUFFSIZE)&&(pck[1]<BUFFSIZE)) /* buffer has space and not overflow */
			    {
			      memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
			      e[1]+=pck[1];
			    }	  
			  else
			    {
			      printf("Event dropped due to the length restriction:\n");
			      printf("Event (TIGRESS BGO) length is %d BUFFSIZE is %d\n",e[1]+pck[1],BUFFSIZE);
			      /* getc(stdin); */
			      return 0;
			    }
			}
		}	    	      
	}

      /* pack GRIFFIN */
      if((data->h.setupHP&GRIFFIN_BIT)!=0)
      	{
      	  memset(pck,0,sizeof(pck));
      	  pck[0]=GRIFFIN_TAG;
      	  pck[1]=2+(sizeof(gheader)+sizeof(theader))/sizeof(int);
      	  memcpy(&pck[2],&data->gr,(sizeof(gheader)+sizeof(theader)));
      	  memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
      	  e[1]+=pck[1];
      	  /* pack CSS */
      	  if(data->gr.g.Posfold>0)
      	    for(int pos=1;pos<NPOSGRIF;pos++)
      	      if((data->gr.g.PosHP&(1<<(pos-1)))!=0)
      		{
      		  memset(pck,0,sizeof(pck));
      		  pck[0]=CSSGRIF_TAG;
      		  pck[1]=3+sizeof(theader)/sizeof(int);
      		  pck[2]=pos;
      		  memcpy(&pck[3],&data->gr.det[pos].h,sizeof(theader));
		  if(((e[1]+pck[1])<BUFFSIZE)&&(pck[1]<BUFFSIZE)) /* buffer has space and not overflow */
		    {
		      memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
		      e[1]+=pck[1];
		    }	  
		  else
		    {
		      printf("Event dropped due to the length restriction:\n");
		      printf("Event (GRIFFIN CSS) length is %d BUFFSIZE is %d\n",e[1]+pck[1],BUFFSIZE);
		      /* getc(stdin); */
		      return 0;
		    }
      		  /* pack Ge */
      		  if(data->gr.det[pos].h.Gefold>0)
      		    for(int col=0;col<NCOL;col++)
      		      if((data->gr.det[pos].h.GeHP&(1<<col))!=0)
      			{
      			  memset(pck,0,sizeof(pck));
      			  pck[0]=GRIFGe_TAG;
      			  pck[1]=3+sizeof(header)/sizeof(int);
      			  pck[2]|=(pos<<16);
      			  pck[2]|=col;
      			  memcpy(&pck[3],&data->gr.det[pos].ge[col].h,sizeof(header));
      			  if(data->gr.det[pos].ge[col].h.TSfold>0)
      			    for(int seg=0;seg<NSEGGRIF;seg++)
      			      if((data->gr.det[pos].ge[col].h.TSHP&(1<<seg))!=0)
      				{
      				  pck[pck[1]]=CHANNEL_TAG;
      				  pck[1]++;
      				  pck[pck[1]]=seg;
      				  pck[1]++;
      				  memcpy(&pck[pck[1]],&data->gr.det[pos].ge[col].seg[seg],sizeof(channel));
      				  pck[1]+=sizeof(channel)/sizeof(int);
      				  memcpy(&pck[pck[1]],&data->gr.det[pos].ge[col].t0[seg],sizeof(double));
      				  pck[1]+=sizeof(double)/sizeof(int);
      				}
			  if(((e[1]+pck[1])<BUFFSIZE)&&(pck[1]<BUFFSIZE)) /* buffer has space and not overflow */
			    {
			      memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
			      e[1]+=pck[1];
			    }	  
			  else
			    {
			      printf("Event dropped due to the length restriction:\n");
			      printf("Event (GRIFFIN HPGe) length is %d BUFFSIZE is %d\n",e[1]+pck[1],BUFFSIZE);
			      /* getc(stdin); */
			      return 0;
			    }
      			}
      		}
      	}

      /* pack PINARRAY */
      if((data->h.setupHP&PINArray_BIT)!=0)
	{
	  memset(pck,0,sizeof(pck));
	  pck[0]=PINArray_TAG;
	  pck[1]=2+sizeof(pheader)/sizeof(int);
	  memcpy(&pck[2],&data->pinarray.h,sizeof(pheader));
	  
	  if(data->pinarray.h.TSfold>0)
	    for(int pos=1;pos<NPIN;pos++)
	      if((data->pinarray.h.TSHP&(one<<pos))!=0)
		{
		  pck[pck[1]]=CHANNEL_TAG;
		  pck[1]++;
		  pck[pck[1]]=pos;
		  pck[1]++;
		  memcpy(&pck[pck[1]],&data->pinarray.pin[pos],sizeof(channel));
		  pck[1]+=sizeof(channel)/sizeof(int);
		  pck[pck[1]]=PINFIT_TAG;
		  pck[1]++;
		  pck[pck[1]]=pos;
		  pck[1]++;
		  memcpy(&pck[pck[1]],&data->pinarray.wfit[pos],sizeof(ShapePar));
		  pck[1]+=sizeof(ShapePar)/sizeof(int);
		}
	  if(((e[1]+pck[1])<BUFFSIZE)&&(pck[1]<BUFFSIZE)) /* buffer has space and not overflow */
	    {
	      memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
	      e[1]+=pck[1];
	    }	  
	  else
	    {
	      printf("Event dropped due to the length restriction:\n");
	      printf("Event (PIN array) length is %d BUFFSIZE is %d\n",e[1]+pck[1],BUFFSIZE);
	      /* getc(stdin); */
	      return 0;
	    }
	  
	}
      
      /* printf("Pre-CsI e[1] %d pck[1] %d\n",e[1],pck[1]); */
      /* int epre = e[1]; */
      /* pack CSIARRAY */
      if((data->h.setupHP&CsIArray_BIT)!=0)
	{
	  memset(pck,0,sizeof(pck));
	  pck[0]=CsIArray_TAG;
	  pck[1]=2+sizeof(caheader)/sizeof(int);
	  memcpy(&pck[2],&data->csiarray.h,sizeof(caheader));
	
	  if(data->csiarray.h.TSfold>0)
	    for(int pos=1;pos<NCSI;pos++)
	      if((data->csiarray.h.TSHP[pos/64]&(one<<pos%64))!=0)
		{
		  //printf("Hit in pos: %i, packing csi array...\n",pos);
		  //getc(stdin);
		  pck[pck[1]]=CHANNEL_TAG;
		  pck[1]++;
		  pck[pck[1]]=pos;
		  pck[1]++;
		  memcpy(&pck[pck[1]],&data->csiarray.csi[pos],sizeof(channel));
		  pck[1]+=sizeof(channel)/sizeof(int);
		  pck[pck[1]]=CSIFIT_TAG;
		  pck[1]++;
		  pck[pck[1]]=pos;
		  pck[1]++;
		  memcpy(&pck[pck[1]],&data->csiarray.wfit[pos],sizeof(ShapePar));
		  pck[1]+=sizeof(ShapePar)/sizeof(int);
		  memcpy(&pck[pck[1]],&data->csiarray.t0[pos],sizeof(double));
		  pck[1]+=sizeof(double)/sizeof(int);
		}	  
	  if(((e[1]+pck[1])<BUFFSIZE)&&(pck[1]<BUFFSIZE)) /* buffer has space and not overflow */
	    {
	      /* printf("e[1] %d pck[1] %d BUFFSIZE %d\n",e[1],pck[1],BUFFSIZE); */
	      memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
	      e[1]+=pck[1];
	    }	  
	  else
	    {
	      /* printf("\n\nepre %d e[1] %d pck[1] %d\n",epre,e[1],pck[1]); */
	      printf("Trigger %d:\n",data->h.trig_num);
	      printf("Event dropped due to the length restriction:\n");
	      printf("Event (CsI array) length is %d BUFFSIZE is %d\n\n",e[1]+pck[1],BUFFSIZE);
	      /* getc(stdin); */
	      return 0;
	    }
	}
      
      /* pack BDPIN */
      if((data->h.setupHP&BDPIN_BIT)!=0)
	{
	  memset(pck,0,sizeof(pck));
	  pck[0]=BDPIN_TAG;
	  pck[1]=2+sizeof(header)/sizeof(int);
	  memcpy(&pck[2],&data->pinbd.h,sizeof(header));
	  if(data->pinbd.h.TSfold>0)
	    if((data->pinbd.h.TSHP&1)!=0)
	      {
		pck[pck[1]]=CHANNEL_TAG;
		pck[1]++;
		pck[pck[1]]=1;
		pck[1]++;
		memcpy(&pck[pck[1]],&data->pinbd.pin,sizeof(channel));
		pck[1]+=sizeof(channel)/sizeof(int);
	      }
	  if(((e[1]+pck[1])<BUFFSIZE)&&(pck[1]<BUFFSIZE)) /* buffer has space and not overflow */
	    {
	      memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
	      e[1]+=pck[1];
	    }	  
	  else
	    {
	      printf("Event dropped due to the length restriction:\n");
	      printf("Event (BD PIN) length is %d BUFFSIZE is %d\n",e[1]+pck[1],BUFFSIZE);
	      /* getc(stdin); */
	      return 0;
	    }
	}      
      
      /* pack S3 sectors*/
      if((data->h.setupHP&S3SEC_BIT)!=0)
	{
	  memset(pck,0,sizeof(pck));
	  pck[0]=S3SEC_TAG;
	  pck[1]=2+sizeof(pheader)/sizeof(int);
	  memcpy(&pck[2],&data->s3.sh,sizeof(pheader));
	  
	  if(data->s3.sh.TSfold>0)
	    for(int pos=1;pos<NS3SEC;pos++)
	      if((data->s3.sh.TSHP&(one<<pos))!=0)
		{
		  pck[pck[1]]=CHANNEL_TAG;
		  pck[1]++;
		  pck[pck[1]]=pos;
		  pck[1]++;
		  memcpy(&pck[pck[1]],&data->s3.sec[pos],sizeof(channel));
		  pck[1]+=sizeof(channel)/sizeof(int);
		}
	  if(((e[1]+pck[1])<BUFFSIZE)&&(pck[1]<BUFFSIZE)) /* buffer has space and not overflow */
	    {
	      memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
	      e[1]+=pck[1];
	    }	  
	  else
	    {
	      printf("Event dropped due to the length restriction:\n");
	      printf("Event (S3 sector) length is %d BUFFSIZE is %d\n",e[1]+pck[1],BUFFSIZE);
	      /* getc(stdin); */
	      return 0;
	    }
	}
      
      /* pack S3 rings*/
      if((data->h.setupHP&S3RING_BIT)!=0)
	{
	  memset(pck,0,sizeof(pck));
	  pck[0]=S3RING_TAG;
	  pck[1]=2+sizeof(pheader)/sizeof(int);
	  memcpy(&pck[2],&data->s3.rh,sizeof(pheader));
	  
	  if(data->s3.rh.TSfold>0)
	    for(int pos=1;pos<NS3RING;pos++)
	      if((data->s3.rh.TSHP&(one<<pos))!=0)
		{
		  pck[pck[1]]=CHANNEL_TAG;
		  pck[1]++;
		  pck[pck[1]]=pos;
		  pck[1]++;
		  memcpy(&pck[pck[1]],&data->s3.ring[pos],sizeof(channel));
		  pck[1]+=sizeof(channel)/sizeof(int);
		}
	  if(((e[1]+pck[1])<BUFFSIZE)&&(pck[1]<BUFFSIZE)) /* buffer has space and not overflow */
	    {
	      memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
	      e[1]+=pck[1];
	    }	  
	  else
	    {
	      printf("Event dropped due to the length restriction:\n");
	      printf("Event (S3 ring) length is %d BUFFSIZE is %d\n",e[1]+pck[1],BUFFSIZE);
	      /* getc(stdin); */
	      return 0;
	    }
	}
      
      if(e[1]<BUFFSIZE-2)
	{
	  if((enb[1]+e[1])<BUFFSIZE) /* buffer has space */
	    {
	      memcpy(&enb[enb[1]],&e[0],e[1]*sizeof(int));
	      enb[1]+=e[1];
	    }
	  else /* no space, save current and start a new buffer */
	    {
	      fwrite(enb,sizeof(int),BUFFSIZE,encoded_output);
	      memset(enb,0,BUFFSIZE*sizeof(int));
	      enb[0]=BUFFER_TAG;
	      enb[1]++;
	      enb[1]++;
	      memcpy(&enb[enb[1]],&e[0],e[1]*sizeof(int));
	      enb[1]+=e[1];
	    }
	}
      else
	{
	  printf("Event dropped due to the length restriction\n");
	  /* printf("Event length is 0x%8.8x\n",e[1]); */
	  printf("Event length is %d BUFFSIZE is %d\n",e[1],BUFFSIZE);
	  return 0;
	}
    }
  return 0;
}
