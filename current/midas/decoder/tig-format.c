#include "tig-format.h"


void add_timestamp_to_table(unsigned long long int ts, ts_table* t)
{
	if(t->tableSize>=t->allocatedSize)
		{
			printf("WARNING: allocated timestamp table size exceeded by data!\n");
			return;
		}
		
	//check if the timestamp is already in the table
	//search backwards since if there is a duplicate it is most likely the previous entry
	for(int i=t->tableSize-1;i>=0;i--)
  	{
  		if(t->table[i]==ts)
  			return;
  	}
  	
  //add the timestamp to the table
	t->table[t->tableSize]=ts;
	t->tableSize++;
	
	return;
}
/*================================================================*/
int get_trigger_from_table(int origTrigNum, unsigned long long int ts, ts_table* t)
{
	//printf("ts: %i\n",ts);
	//for(int i=0;i<20;i++)
	//  printf("%.5i   %.12i\n",i,tab[i]);
	
	//start search at the original trigger number, since the mapped 
	//trigger number will likely be slightly larger than it (and if not, 
	//will likely be slightly smaller)
	
	for(int i=origTrigNum;i<t->tableSize;i++) //loop through larger trigger numbers
		if(t->table[i]==ts)
			{
				//printf("Returning trigger: %i\n",i+1);
				//getc(stdin);
				return i+1;
			}
	for(int i=origTrigNum-1;i>=0;i--) //loop through smaller trigger numbers
		if(t->table[i]==ts)
			{
				//printf("Returning trigger: %i\n",i+1);
				//getc(stdin);
				return i+1;
			}
	printf("WARNING: could not find timestamp value: %i.\n",ts&0x0fffffff);
	//getc(stdin);
	return 0; //could not find appropriate trigger num
}
/*================================================================*/
int unpack_tig10_bank(int *data, int length, Tig10_event *ptr, int proc_wave, short* waveform, int mapTS, ts_table* t)
{
   int type, subtype, value, trigger_num, found_trig;
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
            fprintf(stderr,"\nReconstruction error 0\n"); return(-1);
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
            if(ptr->cfd==(value & 0x00ffffff)){//JW 19/6/17: check for duplicate (DAQ wrote same fragment data twice)
                //fprintf(stderr,"\nEvent 0x%x: port %d, cfd already seen - duplicate, keeping fragment\n", trigger_num, ptr->port);
            }else{
                fprintf(stderr,"\nEvent 0x%x: port %d, cfd already seen - discarding fragment\n", trigger_num, ptr->port);
                /* debug_dump_event(evntbuf, evntbuflen, 0, 0); */ return(-1);
            }
        }else{ 
	          ptr->cfd = value & 0x00ffffff;
	          ptr->cfd_flag++;
	      }
	break;
      case 0x5:                             /*        Charge */
         if( ptr->charge_flag!=0){
	          if(ptr->charge==(value & 0x00ffffff)){//JW 19/6/17: check for duplicate (DAQ wrote same fragment data twice)
	          }else{
	              if((value & 0x02000000)&&(ptr->charge == NEG_CHARGE_VALUE)){
	                  //fprintf(stderr,"\nEvent 0x%x: port %d, charge already seen - duplicate, keeping fragment\n", trigger_num, ptr->port);
	              }else{
	                  fprintf(stderr,"\nEvent 0x%x: port %d, charge already seen - discarding fragment\n", trigger_num, ptr->port);
	                  /* debug_dump_event(evntbuf, evntbuflen, 0, 0); */ return(-1);
	              }
	          }
         }
	 //PJV: 10/3/13
	 //The charge reported needs to be divided by the integration K and 
	 //then multiplied by the dispersion (usually set to 4). These are
	 //now parameters in the map file and are read in and used in map.c
	 //Here, we have the "full charge" value reported by the DAQ.
         //ptr->charge = ((value / (700/4) ) & 0xffff);

	 //PJV: 10/3/13
	 //Mask out everything but the lowest 25 bits.
	 ptr->charge = (value & 0x01ffffff);
	 
	 //PJV: 10/3/13
	 //Check the "negative" status, set in the 26th bit. If it is set
	 //report the charge as a large number. Let us use 30000.
	 if(value & 0x02000000)  ptr->charge = NEG_CHARGE_VALUE;

	 ptr->charge_flag++;
         if( value & 0x10000 ){      /* pileup bit of charge */
            ptr->pileup_flag++;
         }
         break;
      case 0x6:                             /*      LED Time */
        if( ptr->led_flag!=0 ){
	          if(ptr->led==(value & 0x00ffffff)){//JW 19/6/17: check for duplicate (DAQ wrote same fragment data twice)
	              //fprintf(stderr,"\nEvent 0x%x: port %d, led already seen - duplicate, keeping fragment\n", trigger_num, ptr->port);
	          }else{
	              fprintf(stderr,"\nEvent 0x%x: port %d, led already seen - discarding fragment\n", trigger_num, ptr->port);
	              /* debug_dump_event(evntbuf, evntbuflen, 0, 0); */ return(-1);
	          }
         }else{
	          ptr->led = value & 0x00ffffff;
	          ptr->led_flag++;
	       }
	break;
      case 0x8: 
	break;                      /*  Event header */
      case 0xa:
         if( subtype == 0 ){                            /*    Time Stamp */
	   if( ptr->timestamp_flag!=0){
	     if(ptr->timestamp==(value & 0x00ffffff)){//JW 19/6/17: check for duplicate (DAQ wrote same fragment data twice)
	         //fprintf(stderr,"\nEvent 0x%x: port %d, time stamp already seen - duplicate, keeping fragment\n", trigger_num, ptr->port);
	     }else{
	         fprintf(stderr,"\nEvent 0x%x: port %d, time stamp already seen - discarding fragment\n", trigger_num, ptr->port);
	         /* debug_dump_event(evntbuf, evntbuflen, 0, 0); */ return(-1);
	     }
	   }else{
           ptr->timestamp  = value & 0x00ffffff;
	         ptr->timestamp_flag++;
	   }
         } else if( subtype == 1 ){              /* timestamp upper bits */
	   if( ptr->timestamp_up_flag!=0){
	     if(ptr->timestamp_up==(value & 0x00ffffff)){//JW 19/6/17: check for duplicate (DAQ wrote same fragment data twice)
	         //fprintf(stderr,"\nEvent 0x%x: port %d, time stamp up already seen - duplicate, keeping fragment\n", trigger_num, ptr->port);
	     }else{
	         fprintf(stderr,"\nEvent 0x%x: port %d, time stamp up already seen - discarding fragment\n", trigger_num, ptr->port);
	         /* debug_dump_event(evntbuf, evntbuflen, 0, 0); */ return(-1);
	     }
	   }else{ 
	     ptr->timestamp_up = value & 0x00ffffff;
	     ptr->timestamp_up_flag++;
	   }
         } else if( subtype == 2 ){                          /* livetime */
	   if( ptr->livetime_flag!=0){
	     if(ptr->livetime==(value & 0x00ffffff)){//JW 19/6/17: check for duplicate (DAQ wrote same fragment data twice)
	         //fprintf(stderr,"\nEvent 0x%x: port %d, livetime already seen - duplicate, keeping fragment\n", trigger_num, ptr->port);
	     }else{
	         fprintf(stderr,"\nEvent 0x%x: port %d, livetime already seen - discarding fragment\n", trigger_num, ptr->port);
	         /* debug_dump_event(evntbuf, evntbuflen, 0, 0); */ return(-1);
	     }
	   }else{
            ptr->livetime  = value & 0x00ffffff;
            ptr->livetime_flag++;
     }
         } else if( subtype == 4 ){                /* triggers requested */
	   if( ptr->trig_req_flag!=0){
	     if(ptr->trig_req==(value & 0x00ffffff)){//JW 19/6/17: check for duplicate (DAQ wrote same fragment data twice)
	         //fprintf(stderr,"\nEvent 0x%x: port %d, trigger requested already seen - duplicate, keeping fragment\n", trigger_num, ptr->port);
	     }else{
	         fprintf(stderr,"\nEvent 0x%x: port %d, trigger requested already seen - discarding fragment\n", trigger_num, ptr->port);
	         /* debug_dump_event(evntbuf, evntbuflen, 0, 0); */ return(-1);
	     }
	   }else{
           ptr->trig_req    = value & 0x00ffffff;
	         ptr->trig_req_flag++;
	   }
         } else if( subtype == 8 ){                 /* accepted triggers */
	   if( ptr->trig_acc!=0){
	     if(ptr->trig_acc==(value & 0x00ffffff)){//JW 19/6/17: check for duplicate (DAQ wrote same fragment data twice)
	         //fprintf(stderr,"\nEvent 0x%x: port %d, trigger accept already seen - duplicate, keeping fragment\n", trigger_num, ptr->port);
	     }else{
	         fprintf(stderr,"\nEvent 0x%x: port %d, trigger accept already seen - discarding fragment\n", trigger_num, ptr->port);
	         /* debug_dump_event(evntbuf, evntbuflen, 0, 0); */ return(-1);
	     }
	   }else{ 
            ptr->trig_acc     = value & 0x00ffffff;
            ptr->trig_acc_flag++;
     }
         }
         break;
      case 0xb: 
	break;                      /*  Trigger Pattern*/
      case 0xc:                            /*   New Channel */
	ptr->port      = (value & 0xFF     );
	ptr->tig10     = (value & 0xFFF00  ) >>  8;
	ptr->collector = (value & 0x0F00000) >> 20;
	ptr->channel = ptr->port;
	ptr->channel += 10*(ptr->tig10 -1);
	ptr->channel += 120*(ptr->collector-1);

	wave_len  = &ptr->waveform_length; /* should be zero here */

 	 break;
      case 0xe: break;                                    /* Event Trailer */
      case 0xf:                                     /* EventBuilder Timeout*/
                fprintf(stderr,"\nReconstruction error 3\n"); return(-1);
      default:  
	fprintf(stderr,"\nReconstruction error 4\n"); 
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
   
   if(mapTS==1) //map triggers based on timestamp
     {
       unsigned long long int ts=((unsigned long long)ptr->timestamp_up&0x00ffffff)<<24;
       ts|=((unsigned long long)ptr->timestamp&0x00ffffff);
       found_trig = get_trigger_from_table(ptr->trigger_num&0x0fffffff,ts,t);
       if(found_trig!=0) //check whether finding the trigger num failed
         ptr->trigger_num = found_trig;
     }
   else if(mapTS==-1) //generate timestamp map
   	{
   		unsigned long long int ts=((unsigned long long)ptr->timestamp_up&0x00ffffff)<<24;
      ts|=((unsigned long long)ptr->timestamp&0x00ffffff);
   		add_timestamp_to_table(ts,t);
   	}
   
   return(0); 
}
/*================================================================*/
//unpack without using a timestamp lookup table
int unpack_tig10_bank(int *data, int length, Tig10_event *ptr, int proc_wave, short* waveform)
{
	return unpack_tig10_bank(data,length,ptr,proc_wave,waveform,0,NULL);
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
