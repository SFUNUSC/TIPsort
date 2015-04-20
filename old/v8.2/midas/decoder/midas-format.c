#include "midas-format.h"
#define RECORDSIZE (1<<17)       /* use 128k records (any size will do)*/
static char record_buffer[RECORDSIZE];
static int recordpos, recordlen;

#define EVBUFSIZE (1<<24)      /* allow up to 8Mbyte events*/
static char event_buffer[EVBUFSIZE];
static int evbufpos, eventlen;

#define MAX_BANK_SIZE (1<<18)  /* allow up to 256kbyte Banks*/
static int bank_buffer[MAX_BANK_SIZE];
static int bank_len;

typedef struct midas_event_header_struct {
   short     event_id;   short trigger_mask;   int     serial_num;
   int      timestamp;   int      data_size;
} Midas_event_header;

typedef struct midas_allbank_header_struct {
   int    allbanksize;   int          flags;
} Midas_allbank_header;

typedef struct midas_bank16_header_struct { /* old 16 bit version of header */
   char       name[4];   short    data_type;   unsigned short data_size;
} Midas_bank16_header;

typedef struct midas_bank_header_struct {
   char       name[4];   int      data_type;   int      data_size;
} Midas_bank_header;

static Midas_event_header          ev_head;
static Midas_allbank_header   allbank_head;
static Midas_bank_header         bank_head;

/* Variables containing event-unpacking state*/
static unsigned short *evptr;  /* while processing events - current position*/
static int swap_required;
static int first_bank;
static int debug=0;

void swapInt (char* b, int len)
{
   char t;
   while( len > 0 ){
      t=b[0]; b[0]=b[3]; b[3]=t; t=b[1]; b[1]=b[2]; b[2]=t; b+=4; len-=4;
   }
}
void swapShort(char* b, int len)
{
   char t;
   while( len > 0 ){ t=b[0]; b[0]=b[1]; b[1]=t; b+=2; len-=2; }
}
void swapWords (char* b, int len)
{
   char t;
   while( len > 0 ){
      t=b[0]; b[0]=b[2]; b[2]=t; t=b[1]; b[1]=b[3]; b[3]=t; b+=4; len-=4;
   }
}

int next_record(FILE* input)
{
   int bytes;
   bytes = fread(record_buffer, 1, RECORDSIZE, input);
   recordpos = 0; recordlen = bytes;
   return(bytes);
}

int next_event()
{
   int bytes;
   if( ev_head.data_size == 0 ){/* start new event - attempt to read header */
      if( (bytes=sizeof(Midas_event_header)-evbufpos) > recordlen-recordpos ){
         bytes = recordlen - recordpos;
      }
      memcpy(event_buffer+evbufpos, record_buffer+recordpos, bytes );
      recordpos += bytes; evbufpos += bytes;
      if( evbufpos < (int)sizeof(Midas_event_header) ){
         return(-1); /* record ends in event header */
      }
      memcpy((char *)&ev_head, event_buffer, sizeof(Midas_event_header) );
      if( debug ){
         printf("\n\nEvent     id: %d",             ev_head.event_id     );
         printf("    Trigger mask: %d",             ev_head.trigger_mask );
         printf("    Serial   num: %d\ntime",       ev_head.serial_num   );
         printf("stamp   :      %s",ctime((time_t*)&ev_head.timestamp)   );
         printf("data size   : %d",                 ev_head.data_size    );
      }
      evbufpos = 0; /* overwrite header since saved it */
   }
   if( (bytes = ev_head.data_size - evbufpos) > recordlen - recordpos ){
      bytes = recordlen - recordpos;
   }
   memcpy(event_buffer+evbufpos, record_buffer+recordpos, bytes);
   recordpos += bytes; evbufpos += bytes;
   if( evbufpos < ev_head.data_size ){
      return(-1); /* end of record */
   }
   eventlen = ev_head.data_size;
   evbufpos = ev_head.data_size = 0; /* ready for reading in next event */
   /*trig = ev_head.trigger_mask;*/
   first_bank = 1;
   return(0);
}

#define BANK_IS_32BIT (1<<4)

int next_bank(char **bank_name, int **bank_data) /* loop over banks in event*/
{
   Midas_bank16_header bank16_head;
   char* evbase = event_buffer;
   int evlen = eventlen;
   static char bank[5];

   if( first_bank ){ first_bank = 0;
      evptr = (unsigned short *)event_buffer;
      if( ev_head.event_id & 0x8000 ){ return(-1); } /* "odb" event */
      memcpy((char *)&allbank_head, (char *)evptr, sizeof(Midas_allbank_header) );
      swap_required = allbank_head.flags > 0x10000; 
      if( swap_required ){
         swapInt( (char *)&allbank_head, 2*sizeof(int) );
      }
      if( debug ){
         printf("    Allbank Size : %d",   allbank_head.allbanksize );
         printf("    Flags        : %d",   allbank_head.flags );
         printf("    (swap needed): %s\n", swap_required ? "yes" : "no" );
      }
      evptr += sizeof(Midas_allbank_header)/sizeof(short);
      memset((char *)&bank_head, 0, sizeof(Midas_bank_header) );
   }

   /* NOTE: size is rounded up to next multiple of 8 bytes (4 shorts) */
   /* before reading first bank, data_size is zero */
   evptr +=  ((bank_head.data_size/sizeof(short) + 3) & ~3);
   if( (char *)evptr - evbase >= evlen || 
       (char *)evptr - evbase >= allbank_head.allbanksize ){ /* end of event*/
      if( (char *)evptr - evbase != evlen ){
         fprintf(stderr,"wrong event length\n"); return(-1);
      }
      return(-1);
   }
   if( allbank_head.flags & BANK_IS_32BIT ){
      memcpy((char *)&bank_head, (char *)evptr, sizeof(Midas_bank_header) );
      evptr += sizeof(Midas_bank_header)/sizeof(short);
      if( swap_required ){
         swapInt( (char *)&bank_head.data_type, 2*sizeof(int) );
      }
   } else {
      /* old 16bit items - just copy items to new 32bit header */
      memcpy((char*)&bank16_head,(char*)evptr,sizeof(Midas_bank16_header));
      evptr += sizeof(Midas_bank16_header)/sizeof(short);
      if( swap_required ){
         swapShort( (char *)&bank16_head.data_type, 2*sizeof(short) );
      }
      memcpy((char *)&bank_head, (char *)&bank16_head, 4 );
      bank_head.data_type = bank16_head.data_type; 
      bank_head.data_size = bank16_head.data_size; 
   }
   if( bank_head.data_size < 0 ){
      fprintf(stderr,"bank length < 0\n"); return(-1);      
   }
   if( debug ){
      printf("  Bank Name : %6s", bank_head.name );
      printf("  Type : %d",  bank_head.data_type );
      printf("  size : %d\n",  bank_head.data_size );
      printf("  Print ENTER to continue\n");
      getc(stdin);
   }
   memcpy(bank, bank_head.name, 4); bank[4]='\0';
   *bank_name = bank; *bank_data = bank_buffer;
   return( unpack_bank(evptr, bank_head.data_type, 0) );
}
/*------------------------------------------------------------------*/
int unpack_bank (unsigned short* evptr, int type, int max_items)
{
   char format[64], name[5];
   int i, items;
  

   memcpy(name, bank_head.name, 4); name[4]='\0';
   if( bank_head.data_type != type ){
     //fprintf(stderr,"%s bank has unknown type: %d ... ignoring\n",
     //  name, bank_head.data_type );
      return(-1);
   }
   switch(type){
   case 4:
      if( swap_required  ){ swapShort( (char *)evptr, bank_head.data_size ); }
      if( debug ){ sprintf(format,"    %%s[%%2d] = %%6u (0x%%04x)"); }
      items = bank_head.data_size/sizeof(short); break;
   case 6: /* scaler byte order seems to be wrong */
      if( swap_required  ){ swapWords( (char *)evptr, bank_head.data_size ); }
      else { swapShort( (char *)evptr, bank_head.data_size ); }
      if( debug ){ sprintf(format,"    %%s[%%2d] = %%10u (0x%%08x)"); }
      items = bank_head.data_size/sizeof(int); break;
   case 9:
      if( ! swap_required ){ swapWords( (char *)evptr, bank_head.data_size ); }
      if( debug ){ sprintf(format,"    %%s[%%2d] = %%6.2f (0x%%08x)"); }
      items = bank_head.data_size/sizeof(int); break;
   default:
     //fprintf(stderr,"%s bank has unknown type: %d ... ignoring\n",name,type );
      return(-1);
   }
   if( max_items > 0 && items > max_items ){ /* max_items=0 => dont check */
      fprintf(stderr,"%s bank has too many items: %d (max %d) ... ignoring\n",
         name, items, max_items );
      return(-1);
   }
   bank_len = 0;
   for(i=0; i<items; i++){
      bank_buffer[bank_len++] = *evptr++;
      if( type == 6 || type == 9 ){
         bank_buffer[bank_len-1] <<=16; bank_buffer[bank_len-1] += *evptr++;
      }
      if( debug ){
	 int v = bank_buffer[bank_len-1];
         if( type != 9 ){ fprintf(stdout,format,name,i,v,v); }
         else 
	   { float *f=(float *)&v; fprintf(stdout,format,name,i,*f,v); }
         if( ((i+1)%2) == 0 ){ fprintf(stdout,"\n"); }
      }
   }
   if( debug ){ if( ((i+1)%2) == 0 ){ fprintf(stdout,"\n"); } }
   return(items);
}

