#include "sort_and_assemble.h"

/*================================================================*/
void free_list(node* list, data_pointers* dp)
{
  node *current,*prev;

  for(int ind=0;ind<=dp->trig-dp->first;ind++)
    {
      current=list[ind].next;
      while(current->next!=NULL)
	      {
	        prev=current;
	        current=current->next;
	        free(prev);
	      }
    }
  
  free(current);
  free(list);
  free(dp);
}
/*================================================================*/
void insert_node(node* list,data_pointers* dp,Tig10_event *ptr,short* waveform)
{
  node* newnode;
  size_t s;
  int pos=dp->trig-dp->first;
  newnode=(node*)malloc(dp->son); //possible memory leak?
  newnode->eptr=(Tig10_event*)malloc(dp->sot); //possible memory leak?
  memcpy(newnode->eptr,ptr,dp->sot);
  
  if(ptr->waveform_length>0)
    {
      s=ptr->waveform_length*dp->sos;
      newnode->wptr=(short*)malloc(s);
      memcpy(newnode->wptr,waveform,s);
    }
  else
    newnode->wptr=NULL;
  
  newnode->next=list[pos].next;
  list[pos].next=newnode;
 
}
/*================================================================*/
void remove_event(int ind, node* list)
{
  node* current;
  
  while(list[ind].next!=NULL)
    {
      current=list[ind].next;
      list[ind].next=current->next;
      free(current->eptr);
      free(current->wptr);
      free(current);
    }
 
}
/*================================================================*/
void assemble_event(int ind,node* list, data_pointers* dp,tmap* map)
{
  node* current;
  raw_event data;
  //int frag;

  if(list[ind].next!=NULL)
    {
      //if((ind%100)==0)
      //printf("assembling event %d\n",ind);
      memset(&data,0,sizeof(raw_event));
      current=list[ind].next;
      //            frag=0;
      while(current->next!=NULL)
	      {
          //frag++;
	        //printf("mapping fragment %d\n",frag);//getc(stdin);
	        map_event(current->eptr,current->wptr,&data,map,dp->turn);
	        current=current->next;
	      }
      //frag++;
      //printf("mapping fragment %d\n",frag);//getc(stdin);
      map_event(current->eptr,current->wptr,&data,map,dp->turn);
      //printf("Analyzing assembled event\n");
      analyze_assembled_event(&data);
    }
  }
/*================================================================*/
void add_tig10_event(Tig10_event *ptr,short* waveform,node* list, data_pointers *dp,tmap* map)
{ 
  int diff,i;
  int turn;

  dp->trig=ptr->trigger_num&0x00ffffff;


  if(dp->trig==0x00800000)
    {
      dp->turn_enable=1;
    }

  if(dp->trig==0)
    if(dp->turn_enable==1)
      {
				dp->turn++;
				dp->turn_enable=0;
				dp->turn_in_progress=1;
      }

  if(dp->trig==0x00200000)
    if(dp->turn_in_progress==1)
      dp->turn_in_progress=0;

  if(dp->turn_in_progress==1)
    {
      if(dp->trig>0x00c00000)
				{
					dp->trig+=(dp->turn-1)*0x01000000;
					turn=dp->turn-1;
				}
      else
				{
					dp->trig+=dp->turn*0x01000000;
					turn=dp->turn;
				}
    }
  else
    {
      dp->trig+=dp->turn*0x01000000;
      turn=dp->turn;
    }

 if(dp->trig>=dp->first)
    {
      if(dp->trig<dp->last)
				{
					/* insert fragment into the proper trigger link list */
					insert_node(list,dp,ptr,waveform);
				}
      else
				{
					/* new trigger from beyond the buffer depth has arrived */
					diff=dp->trig-dp->last;
					/*check if shift is going to help*/
					if(diff<dp->shift)
						{
							for(i=0;i<dp->shift;i++)
								assemble_event(i,list,dp,map);
							/* clean up old events */
							for(i=0;i<dp->shift;i++)
								remove_event(i,list);
							/* shift the data in the array up */
							for(i=dp->shift;i<DEPTH;i++)
								memcpy(&list[i-dp->shift],&list[i],dp->son);
							/* clean up the remaining part for new data */
							for(i=DEPTH-dp->shift;i<DEPTH;i++)
								{
									list[i].next=NULL;
									list[i].eptr=NULL;
									list[i].wptr=NULL;
								}
							/* adjust the limits after shift */
							dp->first+=dp->shift;
							dp->last+=dp->shift;
							insert_node(list,dp,ptr,waveform);
			  		}
					else
						{
							printf("Reconstruction error unrecoverable by shifting\n");
							printf("Fragment number             %8ld\n",dp->proc);
							printf("Trigger  number             %8d\n",dp->trig);
							printf("Required shift              %8d\n",diff);
							exit(0);
						}

				}
    }
  else
    {
      printf("Reconstruction error trig<first\n");
      printf("Fragment number             %8ld\n",dp->proc);
      printf("Trigger  number             %8d\n",dp->trig);
      printf("First trigger in the buffer %8d\n",dp->first);
      exit(0);
    }



  dp->proc++;
	if((dp->proc%10000)==0) 
    printf("-> %10i fragments assembled.\r",dp->proc);
    
}

/*================================================================*/
int get_and_assemble_fragments(node* list, data_pointers* dp,tmap* map,int mapTS,ts_table* t)
{
   int *bank_data, items;
   char *bank_name;
   static Tig10_event tig10_event;
   static short waveform[MAX_SAMPLES];
   int process_waveforms=1;

  /* loop over banks, looking for WFDN ones (or others ...)  */
   while(1){
      if( (items = next_bank( &bank_name, &bank_data )) < 0 ){ return(-1); }
      //      fprintf(stdout,"bank:%s length:%d\n", bank_name, items);
      if( strcmp(bank_name,"WFDN") ){ continue; } /* ignore other banks */
      swapInt( (char *)bank_data, items*sizeof(int) );
      unpack_tig10_bank( bank_data, items, &tig10_event, process_waveforms,waveform,mapTS,t);
      if(mapTS>=0) //don't assemble to list if just generating timestamp table
      	add_tig10_event( &tig10_event,waveform,list,dp,map);
   }
   return(-1); /* go to next event */
}
/*================================================================*/
//Assemble fragments without remapping trigger numbers based on timestamp
int get_and_assemble_fragments(node* list, data_pointers* dp,tmap* map)
{
	return get_and_assemble_fragments(list,dp,map,0,NULL);
}
/*================================================================*/
void sort_and_assemble(char* inp_name,char* map_name)
{
  FILE* input;
  int state=END_OF_RECORD;
  int stop=0;
  static tmap    map;
  data_pointers* dp=NULL;
  node*          list;
  ts_table*      tstable; //lookup table mapping trigger numbers to timestamps
  int i;
	
 
 
 /* open the input file*/
  if((input=fopen(inp_name,"r"))==NULL)
    {
      printf("\nI can't open input file %s\n",inp_name);
      exit(-2);
    }
 
  /*initialize the sort */
  dp=(data_pointers*)malloc(sizeof(data_pointers));
  dp->first=0;
  dp->last=dp->first+DEPTH;
  dp->proc=0;
  dp->turn=0;
  dp->turn_enable=0;
  dp->turn_in_progress=0;
  dp->shift=SHIFT;
  dp->son=sizeof(node);
  dp->sot=sizeof(Tig10_event);
  dp->sos=sizeof(short);

  list=(node*) malloc(DEPTH*dp->son);

  for(i=0;i<DEPTH;i++)
    {
      list[i].next=NULL;
      list[i].eptr=NULL;
      list[i].wptr=NULL;
    }
 /* get the map */
  read_map(map_name,&map);
  
  if(map.ts_order)
  	{
			tstable=(ts_table*)malloc(sizeof(ts_table));
			printf("-> Generating timestamp table (may take some time) ...\n");
			tstable->tableSize=0;
			tstable->allocatedSize=5000000;
			tstable->table=(unsigned long long int*)malloc(5000000*sizeof(unsigned long long int));
			if(tstable->table==NULL)
			{
				printf("ERROR: Could not allocate memory for timestamp table!\n");
				exit(-2);
			}
		}
	else
		{
			printf("-> Getting and assembling fragments ...\n");
		}
 	
  while(stop==0){
    switch(state){
    case END_OF_RECORD:
      if( next_record(input) <= 0 ){ state = END_OF_FILE;}
      else {state = NEXT_EVENT;}
      break;
    case NEXT_EVENT:
      if( next_event() < 0 ){ state = END_OF_RECORD; }
      else { state = GET_FRAGMENTS; }
      break;
    case GET_FRAGMENTS:
    	if(map.ts_order)
    		{
    			//timestamp mapping specified by "-1" argument
		  		if( get_and_assemble_fragments(list,dp,&map,-1,tstable) < 0 ){ state = NEXT_EVENT;}
		    	else { state = GET_FRAGMENTS;  }
    		}
    	else
    		{
    			if( get_and_assemble_fragments(list,dp,&map) < 0 ){ state = NEXT_EVENT;}
      		else { state = GET_FRAGMENTS;  }
    		}
      break;
    case END_OF_FILE:
      fclose(input);

      if(map.ts_order)
      	{
 					printf("-> Timestamp table generated, contains %8i unique timestamps.\n",tstable->tableSize);
 					/*printf("EVENT#     TIMESTAMP\n");
					for(int i=1;i<=20;i++)
						printf("%.5i   %.12i\n",i,tstable->table[i]);
					for(int i=0;i<3;i++)
						printf("    .              .\n");*/
 				}
 			else
 				{
 					printf("\n-> Total number of assembled fragments is %8ld, corresponding to %8ld triggers.\n",dp->proc,dp->trig-dp->first);
 					printf("-> Events are ordered based on trigger number.\n");
 					printf("-> Assembling SFU file ...\n");
					for(int i=0;i<=dp->trig-dp->first;i++)
						{
							assemble_event(i,list,dp,&map);
							if(i%100==0)
								printf("-> %f %% complete ( %i / %i triggers).\r", i/(0.01*(dp->trig-dp->first)), i, dp->trig-dp->first);
						}
 				}
 			stop=1;
      break;  
    default:
      printf("Unknown case\n");
      exit(0);
      break;
    }
  }
  
  //if neccesary, reassemble the data from the midas file, 
  //this time with corrected trigger numbers
 	if(map.ts_order)
  	{
  		//reinitialize variables
  		stop=0;
  		state=END_OF_RECORD;
  		
  		/* open the input file*/
			if((input=fopen(inp_name,"r"))==NULL)
				{
				  printf("\nI can't open input file %s\n",inp_name);
				  exit(-2);
				}
  		
  		/*initialize the sort */
			dp=(data_pointers*)malloc(sizeof(data_pointers));
			dp->first=0;
			dp->last=dp->first+DEPTH;
			dp->proc=0;
			dp->turn=0;
			dp->turn_enable=0;
			dp->turn_in_progress=0;
			dp->shift=SHIFT;
			dp->son=sizeof(node);
			dp->sot=sizeof(Tig10_event);
			dp->sos=sizeof(short);
			
			list=(node*) malloc(DEPTH*dp->son);

			for(i=0;i<DEPTH;i++)
				{
				  list[i].next=NULL;
				  list[i].eptr=NULL;
				  list[i].wptr=NULL;
				}
		 
			printf("-> Getting and assembling fragments ...\n");
			while(stop==0){
				switch(state){
				case END_OF_RECORD:
				  if( next_record(input) <= 0 ){ state = END_OF_FILE;}
				  else {state = NEXT_EVENT;}
				  break;
				case NEXT_EVENT:
				  if( next_event() < 0 ){ state = END_OF_RECORD; }
				  else { state = GET_FRAGMENTS; }
				  break;
				case GET_FRAGMENTS:
				  //trigger number correction is here
				  if( get_and_assemble_fragments(list,dp,&map,1,tstable) < 0 ){ state = NEXT_EVENT;}
				  else { state = GET_FRAGMENTS;  }
				  break;
				case END_OF_FILE:
				  fclose(input);
      		printf("\n-> Total number of assembled fragments is %8ld, corresponding to %8ld triggers.\n",dp->proc,dp->trig-dp->first);
				  printf("-> Assembling SFU file ...\n");
					for(int i=0;i<=dp->trig-dp->first;i++)
						{
							assemble_event(i,list,dp,&map);
							if(i%100==0)
								printf("-> %f %% complete ( %i / %i triggers).\r", i/(0.01*(dp->trig-dp->first)), i, dp->trig-dp->first);
						}
				  stop=1;
				  break;
				default:
				  printf("Unknown case\n");
				  exit(0);
				  break;
				}
			}
			
  }
  
  
  
}
  
/*================================================================*/
void sort_and_assemble_list(char* inp_name,char* map_name)
{
  FILE* midas_list;
  FILE* input;
  char name[132];
  int state=END_OF_RECORD;
  int stop=0;
  static tmap    map;
  data_pointers* dp=NULL;
  node*          list;
  ts_table*      tstable; //lookup table mapping trigger numbers to timestamps
  int i;
  
  tstable=(ts_table*)malloc(sizeof(ts_table));
 
  /* open the list of midas files */
  if((midas_list=fopen(inp_name,"r"))==NULL)
    {
      printf("ERROR!!! I can't open the midas file list!\n");
      exit(-2);
    }

  /* read name of first input file */
  fscanf(midas_list,"%s",name);

  /* open the first input file*/
  if((input=fopen(name,"r"))==NULL)
    {
      printf("\nI can't open input file %s\n",name);
      exit(-2);
    }
 
  /*initialize the sort */
  dp=(data_pointers*)malloc(sizeof(data_pointers));
  dp->first=0;
  dp->last=dp->first+DEPTH;
  dp->proc=0;
  dp->turn=0;
  dp->turn_enable=0;
  dp->turn_in_progress=0;
  dp->shift=SHIFT;
  dp->son=sizeof(node);
  dp->sot=sizeof(Tig10_event);
  dp->sos=sizeof(short);

  list=(node*) malloc(DEPTH*dp->son);

  for(i=0;i<DEPTH;i++)
    {
      list[i].next=NULL;
      list[i].eptr=NULL;
      list[i].wptr=NULL;
    }
  /* get the map */
  read_map(map_name,&map);
 
 	if(map.ts_order)
  	{
			tstable=(ts_table*)malloc(sizeof(ts_table));
			printf("-> Generating timestamp table (may take some time) ...\n");
			tstable->tableSize=0;
			tstable->allocatedSize=5000000;
			tstable->table=(unsigned long long int*)malloc(5000000*sizeof(unsigned long long int));
			if(tstable->table==NULL)
			{
				printf("ERROR: Could not allocate memory for timestamp table!\n");
				exit(-2);
			}
		}
	else
		{
			printf("-> Getting and assembling fragments ...\n");
		}
		
  while(stop==0){
    switch(state){
    case END_OF_RECORD:
      if( next_record(input) <= 0 ){ state = END_OF_FILE;}
      else {state = NEXT_EVENT;}
      break;
    case NEXT_EVENT:
      if( next_event() < 0 ){ state = END_OF_RECORD; }
      else { state = GET_FRAGMENTS; }
      break;
    case GET_FRAGMENTS:
    	if(map.ts_order)
    		{
    			//timestamp mapping specified by "-1" argument
		  		if( get_and_assemble_fragments(list,dp,&map,-1,tstable) < 0 ){ state = NEXT_EVENT;}
		    	else { state = GET_FRAGMENTS;  }
    		}
    	else
    		{
    			if( get_and_assemble_fragments(list,dp,&map) < 0 ){ state = NEXT_EVENT;}
      		else { state = GET_FRAGMENTS;  }
    		}
      break;
    case END_OF_FILE:
      fclose(input);
      if(map.ts_order)
      	{
      		printf("\n-> Finished reading timestamps from file %s.\n",name);
      	}
      else
      	{
      		printf("\n-> Fragments from file %s are assembled.  %10i fragments, %8i triggers constructed so far.\n",name,dp->proc,dp->trig-dp->first);
      	}
      /* read the name of the next input file */
      if(fscanf(midas_list,"%s",name)==EOF){ state = END_OF_SORT; }
      else
				{
					/* open the next file */
					if((input=fopen(name,"r"))==NULL)
						{
							printf("\nI can't open input file %s\n",name);
							exit(-2);
						}
					/* sort next file */
					state = END_OF_RECORD;
				}
      break;
    case END_OF_SORT:
      /* close the midas list */
      fclose(midas_list);
      
      if(map.ts_order)
      	{
 					printf("-> Timestamp table generated, contains %8i unique timestamps.\n",tstable->tableSize);
 					/*printf("EVENT#     TIMESTAMP\n");
					for(int i=1;i<=20;i++)
						printf("%.5i   %.12i\n",i,tstable->table[i]);
					for(int i=0;i<3;i++)
						printf("    .              .\n");*/
 				}
 			else
 				{
 					printf("-> Events are ordered based on trigger number.\n");
 					printf("-> Assembling SFU file ...\n");
					for(int i=0;i<=dp->trig-dp->first;i++)
						{
							assemble_event(i,list,dp,&map);
							if(i%100==0)
								printf("-> %f %% complete ( %i / %i triggers).\r", i/(0.01*(dp->trig-dp->first)), i, dp->trig-dp->first);
						}
 				}
 				
      stop=1;
      break;
    default:
      printf("Unknown case\n");
      exit(0);
      break;
    }
  }
  
  
  //if neccesary, reassemble the data from the midas file, 
  //this time with corrected trigger numbers
 	if(map.ts_order)
  	{
  		//reinitialize variables
  		stop=0;
  		state=END_OF_RECORD;
  		
  		/* open the list of midas files */
			if((midas_list=fopen(inp_name,"r"))==NULL)
				{
				  printf("ERROR!!! I can't open the midas file list!\n");
				  exit(-2);
				}

			/* read name of first input file */
			fscanf(midas_list,"%s",name);

			/* open the first input file*/
			if((input=fopen(name,"r"))==NULL)
				{
				  printf("\nI can't open input file %s\n",name);
				  exit(-2);
				}
  		
  		/*initialize the sort */
			dp=(data_pointers*)malloc(sizeof(data_pointers));
			dp->first=0;
			dp->last=dp->first+DEPTH;
			dp->proc=0;
			dp->turn=0;
			dp->turn_enable=0;
			dp->turn_in_progress=0;
			dp->shift=SHIFT;
			dp->son=sizeof(node);
			dp->sot=sizeof(Tig10_event);
			dp->sos=sizeof(short);
			
			list=(node*) malloc(DEPTH*dp->son);

			for(i=0;i<DEPTH;i++)
				{
				  list[i].next=NULL;
				  list[i].eptr=NULL;
				  list[i].wptr=NULL;
				}
		 
			printf("-> Getting and assembling fragments ...\n");
			while(stop==0){
				switch(state){
				case END_OF_RECORD:
				  if( next_record(input) <= 0 ){ state = END_OF_FILE;}
				  else {state = NEXT_EVENT;}
				  break;
				case NEXT_EVENT:
				  if( next_event() < 0 ){ state = END_OF_RECORD; }
				  else { state = GET_FRAGMENTS; }
				  break;
				case GET_FRAGMENTS:
					//trigger number correction is here
				  if( get_and_assemble_fragments(list,dp,&map,1,tstable) < 0 ){ state = NEXT_EVENT;}
				  else { state = GET_FRAGMENTS;  }
				  break;
				case END_OF_FILE:
				  fclose(input);
				  printf("\n-> Fragments from file %s are assembled.  %10i fragments, %8i triggers constructed so far.\n",name,dp->proc,dp->trig-dp->first);
				  /* read the name of the next input file */
				  if(fscanf(midas_list,"%s",name)==EOF){ state = END_OF_SORT; }
				  else
						{
							/* open the next file */
							if((input=fopen(name,"r"))==NULL)
								{
									printf("\nI can't open input file %s\n",name);
									exit(-2);
								}
							/* sort next file */
							state = END_OF_RECORD;
						}
				  break;
				case END_OF_SORT:
				  /* close the midas list */
				  fclose(midas_list);
				  
 					/* analyze the last data assembly buffer */
 					printf("-> Assembling SFU file ...\n");
					for(int i=0;i<=dp->trig-dp->first;i++)
						{
							assemble_event(i,list,dp,&map);
							if(i%100==0)
								printf("-> %f %% complete ( %i / %i triggers).\r", i/(0.01*(dp->trig-dp->first)), i, dp->trig-dp->first);
						}
		 				
				  stop=1;
				  break;
				default:
				  printf("Unknown case\n");
				  exit(0);
				  break;
				}
			}
  }

}
