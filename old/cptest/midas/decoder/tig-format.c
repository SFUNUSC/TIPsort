#include "tig-format.h"


int unpack_tig10_bank(int *data, int length, Tig10_event *ptr, int proc_wave, short* waveform)
{
  int type, subtype, value, trigger_num;
   unsigned int *bankend = (unsigned *)data + length;
   unsigned int *evntbuf = (unsigned *)data;
   int *wave_len = NULL;
   short w;

   memset( ptr, 0, CLEAR_SIZE ); 
   ptr->waveform_length=0; 
   ptr->trigger_num = trigger_num = *(evntbuf++);
   //   printf("# \n");
   while( evntbuf < bankend ){
      if( *evntbuf == 0xdeaddead ){ ++evntbuf; continue; }
      type = *evntbuf >> 28;
      subtype = (*evntbuf & 0x0f000000) >> 24;
      value = *(evntbuf++) & 0x0fffffff;
      switch( type ){
      case 0x0: if( wave_len == NULL ){
            fprintf(stderr,"Reconstruction error 0\n"); return(-1);
         }
	//	printf("extracting waveforms\n");
	 if( proc_wave )
	   if(*(wave_len)<MAX_SAMPLES-2)
	     {
	       w=value&0x3fff;
	       if( w & (1<<13) )
		 { w |= 0x3 << 14; }     /* charge bit extension 14 to 16 bits */
	       w+=8192;
	       waveform[*(wave_len)  ]=w;

	       w=((value & 0x0fffffff)>>14) & 0x3fff;
	       if( w & (1<<13) )
		 { w |= 0x3 << 14; }     /* charge bit extension 14 to 16 bits */
	       w+=8192;
	       waveform[*(wave_len)+1]=w;
	     }
	 
         (*wave_len) += 2;
	 //	 printf("waveform length %d\n",*(wave_len));
	 break;           /* waveform data */
      case 0x1:            
	break;           /*  trapeze data */
      case 0x4:                             /*      CFD Time */
        if( ptr->cfd_flag!=0 ){
            fprintf(stderr,"Event 0x%x: port %d, ", trigger_num, ptr->port );
            fprintf(stderr,"cfd already seen - discarding\n");
            /* debug_dump_event(evntbuf, evntbuflen, 0, 0); */ return(-1);
         }
	ptr->cfd = value & 0x00ffffff;
	ptr->cfd_flag++;
	break;
      case 0x5:                             /*        Charge */
         if( ptr->charge_flag!=0){
            fprintf(stderr,"Event 0x%x: port %d, ", trigger_num, ptr->port );
            fprintf(stderr,"charge already seen - discarding\n");
            /* debug_dump_event(evntbuf, evntbuflen, 0, 0); */ return(-1);
         }
         ptr->charge = value & 0xffff;
	 ptr->charge_flag++;
         if( value & 0x10000 ){      /* pileup bit of charge */
            ptr->pileup_flag++;
         }
         break;
      case 0x6:                             /*      CFD Time */
        if( ptr->led_flag!=0 ){
            fprintf(stderr,"Event 0x%x: port %d, ", trigger_num, ptr->port );
            fprintf(stderr,"led already seen - discarding\n");
            /* debug_dump_event(evntbuf, evntbuflen, 0, 0); */ return(-1);
         }
	ptr->led = value & 0x00ffffff;
	ptr->led_flag++;
	break;
      case 0x8: 
	break;                      /*  Event header */
      case 0xa:
         if( subtype == 0 ){                            /*    Time Stamp */
	   if( ptr->timestamp_flag!=0){
	     fprintf(stderr,"Event 0x%x: port %d, ", trigger_num, ptr->port );
	     fprintf(stderr,"time stamp already seen - discarding\n");
	     /* debug_dump_event(evntbuf, evntbuflen, 0, 0); */ return(-1);
	   } 
           ptr->timestamp  = value & 0x00ffffff;
	   ptr->timestamp_flag++;
         } else if( subtype == 1 ){              /* timestamp upper bits */
	   if( ptr->timestamp_up_flag!=0){
	     fprintf(stderr,"Event 0x%x: port %d, ", trigger_num, ptr->port );
	     fprintf(stderr,"time stamp up already seen - discarding\n");
	     /* debug_dump_event(evntbuf, evntbuflen, 0, 0); */ return(-1);
	   } 
	    ptr->timestamp_up = value & 0x00ffffff;
	    ptr->timestamp_up_flag++;
         } else if( subtype == 2 ){                          /* livetime */
	   if( ptr->livetime_flag!=0){
	     fprintf(stderr,"Event 0x%x: port %d, ", trigger_num, ptr->port );
	     fprintf(stderr,"livetime already seen - discarding\n");
	     /* debug_dump_event(evntbuf, evntbuflen, 0, 0); */ return(-1);
	   } 
            ptr->livetime  = value & 0x00ffffff;
            ptr->livetime_flag++;
         } else if( subtype == 4 ){                /* triggers requested */
	   if( ptr->trig_req_flag!=0){
	     fprintf(stderr,"Event 0x%x: port %d, ", trigger_num, ptr->port );
	     fprintf(stderr,"trigger requested already seen - discarding\n");
	     /* debug_dump_event(evntbuf, evntbuflen, 0, 0); */ return(-1);
	   }  
           ptr->trig_req    = value & 0x00ffffff;
	   ptr->trig_req_flag++;
         } else if( subtype == 8 ){                 /* accepted triggers */
	   if( ptr->trig_acc!=0){
	     fprintf(stderr,"Event 0x%x: port %d, ", trigger_num, ptr->port );
	     fprintf(stderr,"trigger accept already seen - discarding\n");
	     /* debug_dump_event(evntbuf, evntbuflen, 0, 0); */ return(-1);
	   } 
            ptr->trig_acc     = value & 0x00ffffff;
            ptr->trig_acc_flag++;
         }
         break;
      case 0xb: 
	break;                      /*  Trigger Pattern*/
      case 0xc:                            /*   New Channel */
	ptr->port      = (value & 0xFF     );
	ptr->tig10     = (value & 0xFFF00  ) >>  8;
	ptr->collector = (value & 0xFF00000) >> 20;
	ptr->channel = ptr->port;
	ptr->channel += 10*(ptr->tig10 -1);
	ptr->channel += 120*(ptr->collector-1);

	wave_len  = &ptr->waveform_length; /* should be zero here */

 	 break;
      case 0xe: break;                                    /* Event Trailer */
      case 0xf:                                     /* EventBuilder Timeout*/
                fprintf(stderr,"Reconstruction error 3\n"); return(-1);
      default:  
	fprintf(stderr,"Reconstruction error 4\n"); 
	printf("=========================================================\n");
	printf("Trigger  number : %8d\n",ptr->trigger_num&0x0fffffff);
	printf("           Port : %8d\n",ptr->port);
	printf("          Tig10 : %8d\n",ptr->tig10);
	printf("      Collector : %8d\n",ptr->collector);
        printf("        Channel : %8d\n",ptr->channel);
	printf("         Charge : %8d\n",ptr->charge);
        printf("Waveform length : %8d\n",ptr->waveform_length);
	return(-1);
      }
   }
   return(0); 
}
/*================================================================*/
void print_fragment_info(Tig10_event *ptr,int time_offset)
{
  int time;
  time=ptr->cfd&0x00ffffff;
  time-=(ptr->timestamp*16)&0x00ffffff;
  time+=time_offset;
  printf("=========================================================\n");
  printf(" Trigger  number : %8d 0x%8.8x\n",ptr->trigger_num&0x0fffffff,ptr->trigger_num);
  printf("            Port : %8d 0x%8.8x\n",ptr->port,ptr->port);
  printf("           Tig10 : %8d 0x%8.8x\n",ptr->tig10,ptr->tig10);
  printf("       Collector : %8d 0x%8.8x\n",ptr->collector,ptr->collector);
  printf("         Channel : %8d 0x%8.8x\n",ptr->channel,ptr->channel);
  printf("          Charge : %8d 0x%8.8x\n",ptr->charge,ptr->charge);
  printf("             CFD : %8d 0x%8.8x\n",ptr->cfd,ptr->cfd);
  printf("      Time Stamp : %8d 0x%8.8x\n",ptr->timestamp,ptr->timestamp);
  printf("   Time Stamp Up : %8d 0x%8.8x\n",ptr->timestamp_up,ptr->timestamp_up);
  printf("            Time : %8d 0x%8.8x\n",time, time);
  printf(" Waveform length : %8d 0x%8.8x\n",ptr->waveform_length,ptr->waveform_length);
}
