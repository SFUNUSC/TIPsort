#include "SFU-decoder.h"

/*====================================================================================*/
int next_event(int **bp, int *end, counters *c, int* e)
{
  if((c->event%10000)==0)
    printf("Number of analyzed events is %10ld\r",c->event);
  if(**bp!=(int)EVENT_TAG)
    return -1;
 
  if((*bp+*(*bp+1))<=end)
    {
      memset(e,0,BUFFSIZE*sizeof(int));
      memcpy(e,*bp,(*(*bp+1)*sizeof(int)));
      *bp+=*(*bp+1);
      c->event++;
      return 1;
    }
 
    return -1;
}
/*====================================================================================*/
int process_event(int* e, raw_event* data)
{
  int *ep,*end;
  int s,sh,sc,sp,sca,sf,sd,n,m;
  int pos,col,seg,sup;

  sh=(int)(sizeof(header)/sizeof(int));
  sp=(int)(sizeof(pheader)/sizeof(int));
  sca=(int)(sizeof(caheader)/sizeof(int));
  sc=(int)(sizeof(channel)/sizeof(int));
  sd=(int)(sizeof(double)/sizeof(int));
  sf=(int)(sizeof(ShapePar)/sizeof(int));

  ep=&e[0];
  if(*ep!=(int)EVENT_TAG)
    {
      printf("Wrong event tag  0x%8.8x\n",*ep);
      printf("Terminating sort\n");
      exit(-1);
    }
  ep++;
  end=&e[*ep];
  ep++;
  while(ep<end)
    {
      switch(*ep)
	{
	case HEADER_TAG:
	  ep++; s=*ep-2;ep++;
	  memcpy(&data->h,ep,s*sizeof(int));
	  ep+=s;
	  break;
	case RF_TAG:
	  ep++; s=*ep-2;ep++;
	  memcpy(&data->rf,ep,s*sizeof(int));
	  ep+=s;
	  break;
	case GRIFFIN_TAG:
	  ep++; s=*ep-2;ep++;
	  memcpy(&data->gr,ep,s*sizeof(int));
	  ep+=s;
	  break;
	case CSSGRIF_TAG:
	  ep++; s=*ep-3;ep++;
	  pos=*ep;ep++;
	  memcpy(&data->gr.det[pos].h,ep,s*sizeof(int));
	  ep+=s;
	  break;
	case GRIFGe_TAG:
	  ep++; s=*ep-3;
	  ep++; pos=(*ep&0xffff0000)>>16;col=*ep&0x0000ffff;ep++;
	  memcpy(&data->gr.det[pos].ge[col].h,ep,sh*sizeof(int));
	  ep+=sh;
	  n=(s-sh)/(sc+sd+2);
	  for(m=0;m<n;m++)
	    {
	      if(*ep!=(int)CHANNEL_TAG)
		{
		  printf("GRIFFIN Ge decoding\n");
		  printf("Pos %2d Col %2d\n",pos,col);
		  printf("Wrong channel tag  0x%8.8x\n",*ep);
		  printf("Terminating sort\n");
		  exit(-1);
		}
	      ep++;
	      seg=*ep;
	      ep++;
	      memcpy(&data->gr.det[pos].ge[col].seg[seg],ep,sc*sizeof(int));
	      ep+=sc;
	      memcpy(&data->gr.det[pos].ge[col].t0[seg],ep,sd*sizeof(int));
	      ep+=sd;
	    }
	  break;
	case TIGRESS_TAG:
	  ep++; s=*ep-2;ep++;
	  memcpy(&data->tg,ep,s*sizeof(int));
	  ep+=s;
	  break;
	case CSSTIGR_TAG:
	  ep++; s=*ep-3;ep++;
	  pos=*ep;ep++;
	  memcpy(&data->tg.det[pos].h,ep,s*sizeof(int));
	  ep+=s;
	  break;
	case TIGRGe_TAG:
	  ep++; s=*ep-3;
	  ep++; pos=(*ep&0xffff0000)>>16;col=*ep&0x0000ffff;ep++;
	  memcpy(&data->tg.det[pos].ge[col].h,ep,sh*sizeof(int));
	  ep+=sh;
	  n=(s-sh)/(sc+sd+2);
	  for(m=0;m<n;m++)
	    {
	      if(*ep!=(int)CHANNEL_TAG)
		{
		  printf("TIGRESS Ge decoding\n");
		  printf("Pos %2d Col %2d\n",pos,col);
		  printf("Wrong channel tag  0x%8.8x\n",*ep);
		  printf("Terminating sort\n");
		  exit(-1);
		}
	      ep++;
	      seg=*ep;
	      ep++;
	      memcpy(&data->tg.det[pos].ge[col].seg[seg],ep,sc*sizeof(int));
	      ep+=sc;
	      memcpy(&data->tg.det[pos].ge[col].t0[seg],ep,sd*sizeof(int));
	      ep+=sd;
	    }
	  break;
	case BGO_TAG:
	  ep++; s=*ep-3;
	  ep++; pos=(*ep&0xffff0000)>>16;col=*ep&0x0000ffff;ep++;
	  memcpy(&data->tg.det[pos].bgo[col].h,ep,sh*sizeof(int));
	  ep+=sh;
	  n=(s-sh)/(sc+sd+2);
	  for(m=0;m<n;m++)
	    {
	      if(*ep!=(int)CHANNEL_TAG)
		{
		  printf("TIGRESS BGO decoding\n");
		  printf("Pos %2d Col %2d\n",pos,col);
		  printf("Wrong channel tag  0x%8.8x\n",*ep);
		  printf("Terminating sort\n");
		  exit(-1);
		}
	      ep++;
	      sup=*ep;
	      ep++;
	      memcpy(&data->tg.det[pos].bgo[col].sup[sup],ep,sc*sizeof(int));
	      ep+=sc;
	      memcpy(&data->tg.det[pos].bgo[col].t0[sup],ep,sd*sizeof(int));
	      ep+=sd;
	    }
	  break;

	case CsIArray_TAG:
	  ep++; s=*ep-2;
	  ep++;
	  memcpy(&data->csiarray.h,ep,sca*sizeof(int));
	  ep+=sca;
	  n=(s-sca)/(sc+sf+sd+4);
	  for(m=0;m<n;m++)
	    {
	      if(*ep!=(int)CHANNEL_TAG)
	  	{
		  printf("\n\n");
		  printf("Energy     fold                   %4.4x\n",data->csiarray.h.Efold);
		  printf("CFD        fold                   %4.4x\n",data->csiarray.h.Tfold);
		  printf("Time Stamp fold                   %4.4x\n",data->csiarray.h.TSfold);
		  printf("Energy hit pattern                %12.12llx\n",data->csiarray.h.EHP);
		  printf("CFD hit pattern                   %12.12llx\n",data->csiarray.h.THP);
		  printf("Time Stamp hit pattern            %12.12llx\n",data->csiarray.h.TSHP);
	  	  printf("CsI array decoding n=%d m=%d\n",n,m);
	  	  printf("Wrong channel tag  0x%8.8x\n",*ep);
		  for(int xx=0;xx<8;xx++)
		    ep--;
		  for(int xx=0;xx<16;xx++)
		    {
		      printf("0x%8.8x ",*ep);
		      ep++;
		    }
	  	  printf("Terminating sort\n");
	  	  exit(-1);
	  	}
	      ep++;
	      pos=*ep;
	      ep++;
	      memcpy(&data->csiarray.csi[pos],ep,sc*sizeof(int));
	      ep+=sc;

	      if(*ep!=(int)CSIFIT_TAG)
	  	{
	  	  printf("CsI array decoding\n");
	  	  printf("Wrong waveform fit tag  0x%8.8x\n",*ep);
	  	  printf("Terminating sort\n");
	  	  exit(-1);
	  	}
	      ep++;
	      pos=*ep;
	      ep++;
	      memcpy(&data->csiarray.wfit[pos],ep,sf*sizeof(int));
	      ep+=sf;
	      memcpy(&data->csiarray.t0[pos],ep,sd*sizeof(int));
	      ep+=sd;

	   }
	  break;

	case PINArray_TAG:
	  ep++; s=*ep-2;
	  ep++;
	  memcpy(&data->pinarray.h,ep,sp*sizeof(int));
	  ep+=sp;
	  n=(s-sp)/(sc+sf+4);
	  for(m=0;m<n;m++)
	    {
	      if(*ep!=(int)CHANNEL_TAG)
	  	{
	  	  printf("PIN array decoding\n");
	  	  printf("Wrong channel tag  0x%8.8x\n",*ep);
	  	  printf("Terminating sort\n");
	  	  exit(-1);
	  	}
	      ep++;
	      pos=*ep;
	      ep++;
	      memcpy(&data->pinarray.pin[pos],ep,sc*sizeof(int));
	      ep+=sc;
	      if(*ep!=(int)PINFIT_TAG)
	  	{
	  	  printf("PIN array decoding\n");
	  	  printf("Wrong waveform fit tag  0x%8.8x\n",*ep);
	  	  printf("Terminating sort\n");
	  	  exit(-1);
	  	}
	      ep++;
	      pos=*ep;
	      ep++;
	      memcpy(&data->pinarray.wfit[pos],ep,sf*sizeof(int));
	      ep+=sf;
	   }
	  break;

	case S3SEC_TAG:
	  ep++; s=*ep-2;
	  ep++;
	  memcpy(&data->s3.sh,ep,sp*sizeof(int));
	  ep+=sp;
	  n=(s-sp)/(sc+2);
	  for(m=0;m<n;m++)
	    {
	      if(*ep!=(int)CHANNEL_TAG)
	  	{
	  	  printf("S3 sector decoding\n");
	  	  printf("Wrong channel tag  0x%8.8x\n",*ep);
	  	  printf("Terminating sort\n");
	  	  exit(-1);
	  	}
	      ep++;
	      pos=*ep;
	      ep++;
	      memcpy(&data->s3.sec[pos],ep,sc*sizeof(int));
	      ep+=sc;
	   }
	  break;

	case S3RING_TAG:
	  ep++; s=*ep-2;
	  ep++;
	  memcpy(&data->s3.rh,ep,sp*sizeof(int));
	  ep+=sp;
	  n=(s-sp)/(sc+2);
	  for(m=0;m<n;m++)
	    {
	      if(*ep!=(int)CHANNEL_TAG)
	  	{
	  	  printf("S3 ring decoding\n");
	  	  printf("Wrong channel tag  0x%8.8x\n",*ep);
	  	  printf("Terminating sort\n");
	  	  exit(-1);
	  	}
	      ep++;
	      pos=*ep;
	      ep++;
	      memcpy(&data->s3.ring[pos],ep,sc*sizeof(int));
	      ep+=sc;
	   }
	  break;

	case BDPIN_TAG:
	  ep++; s=*ep-2;
	  ep++;
	  memcpy(&data->pinbd.h,ep,sh*sizeof(int));
	  ep+=sh;
	  n=(s-sh)/(sc+2);
	  for(m=0;m<n;m++)
	    {
	      if(*ep!=(int)CHANNEL_TAG)
	  	{
	  	  printf("PIN array decoding\n");
	  	  printf("Wrong channel tag  0x%8.8x\n",*ep);
	  	  printf("Terminating sort\n");
	  	  exit(-1);
	  	}
	      ep++;
	      pos=*ep;
	      ep++;
	      memcpy(&data->pinbd.pin,ep,sc*sizeof(int));
	      ep+=sc;
	   }
	  break;

	default:
	  printf("Wrong tag  0x%8.8x\n",*ep);
	  printf("Terminating sort\n");
	  exit(-1);
	  break;
	}
    }
  return 1;
}
/*====================================================================================*/
void print_data(int* d,int limit)
{
  int i,j;
  for(i=0;i<limit;i+=16)
    {
      printf(" %4.4d ",i);
      for(j=0;j<16;j++)
	if((i+j)<limit)
	  printf(" 0x%8.8x ",d[i+j]);
      printf("\n");
    }
}
/*====================================================================================*/
void sort(input_names_type *name)
{
  FILE *input=NULL,*output=NULL;
  int state=END_OF_RECORD;
  int   stop;
  counters   *c;
  int         b[BUFFSIZE],e[BUFFSIZE],sb[BUFFSIZE];
  int         *bp,*end;
  raw_event   data;  
  
  if((input=fopen(name->fname.inp_data,"r"))==NULL)
    {
      printf("\nI can't open input file %s\n",name->fname.inp_data);
      exit(-2);
    }
   else
   printf("Sorting data from file %s\n",name->fname.inp_data);
  if(name->flag.separate==1)
    {
      if((output=fopen(name->fname.out_data,"w"))==NULL)
	{
	  printf("\nI can't open output file %s\n",name->fname.out_data);
	  exit(-2);
	}
      else
	{
	  printf("Separated data written to file %s\n",name->fname.out_data);
	  memset(sb,0,BUFFSIZE*sizeof(int));
	  sb[0]=BUFFER_TAG;
	  sb[1]++;
	  sb[1]++;
	}
    }
  /*initialize*/
  c=(counters *) malloc(sizeof(counters));
  memset(c,0,sizeof(counters));
  bp=NULL;
  end=NULL;

  /*do sorting */
  stop=0;
  while(stop==0){
    switch(state){
    case END_OF_RECORD:
      if( fread(&b,sizeof(int),BUFFSIZE,input) != BUFFSIZE ){state = END_OF_FILE;}
      else 
	{
	  bp=&b[0];
	  end=&(b[b[1]]);

	  if(*bp!=(int)BUFFER_TAG)
	    {
	      printf("Wrong buffer tag  0x%8.8x\n",*b);
	      printf("Terminating sort\n");
	      exit(-1);
	    }
	  c->buffer++;
	  bp+=2;
	  state = NEXT_EVENT;
	}
      break;
    case NEXT_EVENT:
      if( next_event(&bp,end,c,e) < 0 ){state = END_OF_RECORD;}
      else {state = PROCESSING_EVENT;}
      break;
    case PROCESSING_EVENT:
      memset(&data,0,sizeof(raw_event));
      if( process_event(e,&data) < 0 ){ state = NEXT_EVENT;}
      else { state = ANALYZE_DATA;  }
      break;
    case ANALYZE_DATA:
      if(analyze_data(&data)==SEPARATOR_KEEP)
	if(name->flag.separate==1)
	  {
	    if(e[1]<BUFFSIZE-2)
	      {
		if((sb[1]+e[1])<BUFFSIZE) /* separator buffer has space */
		  {
		    memcpy(&sb[sb[1]],&e[0],e[1]*sizeof(int));
		    sb[1]+=e[1];
		  }
		else /* no space, save current and start a new buffer */
		  {
		    fwrite(sb,sizeof(int),BUFFSIZE,output);
		    memset(sb,0,BUFFSIZE*sizeof(int));
		    sb[0]=BUFFER_TAG;
		    sb[1]++;
		    sb[1]++;
		    memcpy(&sb[sb[1]],&e[0],e[1]*sizeof(int));
		    sb[1]+=e[1];
		  }
	      }
	    else
	      {
		printf("Event dropped due to the length restriction\n");
		printf("Event length is 0x%8.8x\n",e[1]);
	      }
	  }
      state=NEXT_EVENT;
      break;
    case END_OF_FILE:
      printf("End of file %s\n",name->fname.inp_data);
      fclose(input);
      if(name->flag.separate==1)
	{
	  fwrite(sb,sizeof(int),BUFFSIZE,output);
	  fclose(output);
	}
      printf("Number of analyzed buffers is %8ld\n",c->buffer);
      printf("Number of analyzed events  is %8ld\n",c->event);
      stop=1;
      break;
    default:
      printf("Unknown case\n");
      exit(0);
      break;
    }
  }
  free(c);
}

/*====================================================================================*/
