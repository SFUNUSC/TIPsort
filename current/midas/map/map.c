#include "map.h"
/*======================================================================*/
void print_map(tmap* map)
{
  int i;
 
  if((map->tigr_css_max-map->tigr_css_min)>0)
    {
      printf("=======    ==========================================\n");
      printf("TIGRESS    CHANNEL\n");
      printf("MIN       %d\n",map->tigr_css_min);
      printf("MAX       %d\n",map->tigr_css_max);
      if(map->tigr_ge_fit==1)
	printf("FIT_GE     YES\n");
      else
	printf("FIT_GE     NO\n");
      if(map->tigr_bgo_fit==1)
	printf("FIT_BGO    YES\n");
      else
	printf("FIT_BGO    NO\n");
      printf("KPAR      %d\n",map->tigr_css_kpar);
      printf("DISP        %d\n",map->tigr_css_disp);
      printf("=======    ==========================================\n");
      printf("TIGRESS    GROUP2POSITION\n");
      printf("Group      Position\n");
      for(i=0;i<NPOSTIGR;i++)
	if(map->tigr_ge_map[i]!=0)
	  printf(" %2d         %2d\n",i,map->tigr_ge_map[i]);
      printf("=======    ==========================================\n");
    }

  if((map->grif_css_max-map->grif_css_min)>0)
    { 
      printf("GRIFFIN   CHANNEL\n");
      printf("MIN       %d\n",map->grif_css_min);
      printf("MAX       %d\n",map->grif_css_max);
     if(map->grif_fit==1)
	printf("FIT       YES\n");
      else
	printf("FIT       NO\n");
     printf("KPAR      %d\n",map->grif_css_kpar);
     printf("DISP        %d\n",map->grif_css_disp);
      printf("=======    ==========================================\n");
      
      printf("GRIFFIN    GROUP2POSITION\n");
      printf("Group      Position\n");
      for(i=0;i<NPOSGRIF;i++)
	if(map->grif_ge_map[i]!=0)
	  printf(" %2d         %2d\n",i,map->grif_ge_map[i]);
      printf("=======    ==========================================\n");
    }
  if((map->csiarray_max-map->csiarray_min)>0)
    {
      printf("CSIARRAY  CHANNEL\n");
      printf("MIN       %d\n",map->csiarray_min);
      printf("MAX       %d\n",map->csiarray_max);
      if(map->csiarray_fit==1)
	printf("FIT       YES\n");
      else
	printf("FIT       NO\n");
      printf("TRC       %10.3f\n",map->csiarray_trc);
      printf("TF        %10.3f\n",map->csiarray_tf);
      printf("TS        %10.3f\n",map->csiarray_ts);
      printf("TR        %10.3f\n",map->csiarray_tr);
      printf("KPAR      %d\n",map->csiarray_css_kpar);
     printf("DISP        %d\n",map->csiarray_css_disp);
      printf("=======    ==========================================\n");
      printf("CSIARRAY   GROUP2POSITION\n");
      printf("Group      Position\n");
      for(i=0;i<NCSI;i++)
	if(map->csiarray_map[i]!=0)
	  printf(" %2d         %2d\n",i,map->csiarray_map[i]);
      printf("=======    ==========================================\n");
    }
}
/*======================================================================*/
void read_map(char* name,tmap* map)
{
  FILE *inp;
  char str1[132],str2[132];
  int take;
  char c[2];
  int  ord;
  
  if((inp=fopen(name,"r"))==NULL)
      {
         printf("\nERROR: map file %s can not be opened\n",name);
         exit(EXIT_FAILURE);
      }
  else
    printf("-> TDAQ map read from file: %s\n",name);

  //initialize map to -1 to avoid mis-labeling channels
  memset(map,-1,sizeof(tmap));

  while(fscanf(inp,"%s %s",str1,str2)!=EOF)
    {
    
    	if(strcmp(str1,"TIMESTAMP_ORDER")==0)
			{
				if(strcmp(str2,"YES")==0)
					{
						printf("-> Events will be mapped based on timestamps.\n");
						map->ts_order=1;
					}
				else
					{
						printf("-> Events will be mapped based on trigger number.\n");
						map->ts_order=0;
					}
			}
    
      if(strcmp(str1,"PINBD")==0)
				if(strcmp(str2,"PARAMETERS")==0)
					if(fscanf(inp,"%s %s",str1,str2)!=EOF)
						if(strcmp(str1,"CHANNEL")==0)
							map->pinbd=atoi(str2);
 
     if(strcmp(str1,"RF")==0)
			if(strcmp(str2,"PARAMETERS")==0)
				{
					if(fscanf(inp,"%s %s",str1,str2)!=EOF)
					  if(strcmp(str1,"CHANNEL")==0)
							map->rf=atoi(str2);
					if(fscanf(inp,"%s %s",str1,str2)!=EOF)
					  if(strcmp(str1,"PERIOD")==0)
							map->rf_period=atof(str2);
					//printf("RF channel %d RF period %f\n",map->rf,map->rf_period);
					//getc(stdin);
				}

    if(strcmp(str1,"TIGRESS")==0)
	if(strcmp(str2,"CHANNEL")==0)
	  {

	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"MIN")==0)
		map->tigr_css_min=atoi(str2);
	
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"MAX")==0)
		map->tigr_css_max=atof(str2);

	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"FIT_GE")==0)
		if(strcmp(str2,"YES")==0)
		  {
		  map->tigr_ge_fit=1;
		  //printf("map->tigr_ge_fit=%d\n",map->tigr_ge_fit);
		  //getc(stdin);
		  }	 
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"FIT_BGO")==0)
		if(strcmp(str2,"YES")==0)
		  {
		  map->tigr_bgo_fit=1;
		  //printf("map->tigr_bgo_fit=%d\n",map->tigr_bgo_fit);
		  //getc(stdin);
		  }	 

	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"KPAR")==0)
		map->tigr_css_kpar=atof(str2); 
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"DISP")==0)
		map->tigr_css_disp=atof(str2);
	  }
     take=1;
     if(strcmp(str1,"TIGRESS")==0)
	if(strcmp(str2,"GROUP2POSITION")==0)
	  {
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"Group")==0)
		if(strcmp(str2,"Position")==0)
		  while(take==1)
		      if(fscanf(inp,"%s %s",str1,str2)!=EOF)
			{
			  if((strcmp(str1,"END")==0)||(strcmp(str1,"MAP")==0))
			    take=0;
			  else
			    map->tigr_ge_map[atoi(str1)]=atoi(str2);	   
			}

	  }

     if(strcmp(str1,"GRIFFIN")==0)
	if(strcmp(str2,"CHANNEL")==0)
	  {

	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"MIN")==0)
		map->grif_css_min=atoi(str2);
	
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"MAX")==0)
		map->grif_css_max=atof(str2);

	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"FIT")==0)
		if(strcmp(str2,"YES")==0)
		  map->grif_fit=1;

	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"KPAR")==0)
		map->grif_css_kpar=atof(str2); 
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"DISP")==0)
		map->grif_css_disp=atof(str2);	 
	  }

     take=1;
     if(strcmp(str1,"GRIFFIN")==0)
	if(strcmp(str2,"GROUP2POSITION")==0)
	  {
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"Group")==0)
		if(strcmp(str2,"Position")==0)
		  while(take==1)
		      if(fscanf(inp,"%s %s",str1,str2)!=EOF)
			{
			  if((strcmp(str1,"END")==0)||(strcmp(str1,"MAP")==0))
			    take=0;
			  else
			    map->grif_ge_map[atoi(str1)]=atoi(str2);	   
			}

	  }

     if(strcmp(str1,"PINARRAY")==0)
	if(strcmp(str2,"CHANNEL")==0)
	  {
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"MIN")==0)
		map->pinarray_min=atoi(str2);
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"MAX")==0)
		map->pinarray_max=atof(str2);
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"FIT")==0)
		if(strcmp(str2,"YES")==0)
		  map->pinarray_fit=1;
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"TRC")==0)
		map->pinarray_trc=atof(str2);

	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"KPAR")==0)
		map->pinarray_css_kpar=atof(str2); 
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"DISP")==0)
		map->pinarray_css_disp=atof(str2);
	  }
     take=1;
     if(strcmp(str1,"PINARRAY")==0)
	if(strcmp(str2,"GROUP2POSITION")==0)
	  {
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"Group")==0)
		if(strcmp(str2,"Position")==0)
		  while(take==1)
		      if(fscanf(inp,"%s %s",str1,str2)!=EOF)
			{
			  if((strcmp(str1,"END")==0)||(strcmp(str1,"MAP")==0))
			    take=0;
			  else
			    map->pinarray_map[atoi(str1)]=atoi(str2);
			}
	  }

     if(strcmp(str1,"CSIARRAY")==0)
	if(strcmp(str2,"CHANNEL")==0)
	  {
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"MIN")==0)
		map->csiarray_min=atoi(str2);
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"MAX")==0)
		map->csiarray_max=atof(str2);
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"FIT")==0)
		if(strcmp(str2,"YES")==0)
		  map->csiarray_fit=1;
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"TRC")==0)
		map->csiarray_trc=atof(str2);
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"TF")==0)
		map->csiarray_tf=atof(str2);
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"TS")==0)
		map->csiarray_ts=atof(str2);
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"TR")==0)
		map->csiarray_tr=atof(str2);
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"KPAR")==0)
		map->csiarray_css_kpar=atof(str2); 
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"DISP")==0)
		map->csiarray_css_disp=atof(str2);
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"PILEUP_REJ")==0)
                if(strcmp(str2,"YES")==0)
		  map->csiarray_pileup_rej=1;
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"DIST")==0)
		map->csiarray_filter_dist=atoi(str2);
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"SAMPLES")==0)
		map->csiarray_averaging_samples=atoi(str2);
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"FAMOUNT")==0)
		map->csiarray_fall_amount=atoi(str2);
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"RAMOUNT")==0)
		map->csiarray_rise_amount=atoi(str2);
	  }

     take=1;
     if(strcmp(str1,"CSIARRAY")==0)
	if(strcmp(str2,"GROUP2POSITION")==0)
	  {
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"Group")==0)
		if(strcmp(str2,"Position")==0)
		  while(take==1)
		      if(fscanf(inp,"%s %s",str1,str2)!=EOF)
						{
							if((strcmp(str1,"END")==0)||(strcmp(str1,"MAP")==0))
								take=0;
							else
								map->csiarray_map[atoi(str1)]=atoi(str2);
						}
	  }

     if(strcmp(str1,"S3")==0)
	if(strcmp(str2,"CHANNEL")==0)
	  {
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"MIN")==0)
		map->s3_min=atoi(str2);
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"MAX")==0)
		map->s3_max=atof(str2);
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"KPAR")==0)
		map->s3_css_kpar=atof(str2); 
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"DISP")==0)
		map->s3_css_disp=atof(str2);
	  }

     if(strcmp(str1,"S3")==0)
	if(strcmp(str2,"GROUP2POSITION")==0)
	  {
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"Group")==0)
		if(strcmp(str2,"Position")==0)
		  while(take==1)
		      if(fscanf(inp,"%s %s",str1,str2)!=EOF)
			{
			  if((strcmp(str1,"END")==0)||(strcmp(str1,"MAP")==0))
			    take=0;
			  else
			    {
			      sscanf(str2,"%1s %d",c,&ord); 	
			      if((strcmp(c,"S")==0))
				{
				  map->s3_gid[atoi(str1)]=1;
				  map->s3_ord[atoi(str1)]=ord;
				}
			      if((strcmp(c,"R")==0))
				{
				  map->s3_gid[atoi(str1)]=2;
				  map->s3_ord[atoi(str1)]=ord;
				} 
				 
			    }
			}
	  }

     if(strcmp(str1,"SECTORMAP")==0)
       {
	 ord=atoi(str2);
	 if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	   if(strcmp(str1,"Channel")==0)
	     if(strcmp(str2,"Sector")==0)
	       while(take==1)
		 if(fscanf(inp,"%s %s",str1,str2)!=EOF)
		   {
		     if((strcmp(str1,"END")==0)||(strcmp(str1,"MAP")==0))
		       take=0;
		     else
		       {
			 map->s3_sec[ord][atoi(str1)]=atoi(str2);
		       }
		   }
       }

		if(strcmp(str1,"RINGMAP")==0)
			{
				ord=atoi(str2);
				if(fscanf(inp,"%s %s",str1,str2)!=EOF)
					if(strcmp(str1,"Channel")==0)
						if(strcmp(str2,"Ring")==0)
							while(take==1)
								if(fscanf(inp,"%s %s",str1,str2)!=EOF)
									{
										if((strcmp(str1,"END")==0)||(strcmp(str1,"MAP")==0))
											take=0;
										else
											{
												map->s3_ring[ord][atoi(str1)]=atoi(str2);
											}
									}
			}
      
    }
  fclose(inp);
  //print_map(map);
}
/*======================================================================*/
void map_event(Tig10_event *ptr, short* waveform, raw_event *data,tmap* map,int turn)
{

  int c,d;
  int col,pos,id;
  int type=-1;
  int take;
  uint64_t one=1;
  WaveFormPar wpar;

  if(turn>-1)
    {
      if(data->h.frag_fold==0)
				data->h.trig_num=(ptr->trigger_num&0x00ffffff)+turn*0x01000000;
      else
				{
					if(data->h.trig_num!=((ptr->trigger_num&0x00ffffff)+turn*0x01000000))
						{
							printf("Reconstruction error: inconsistent trigger number\n");
							printf("Event trigger number    %8d\n",(data->h.trig_num));
							printf("Fragment trigger number %8d\n",(ptr->trigger_num));
							printf("Sort forced to terminate\n");
							exit(0);
						}
				}
    }

  data->h.frag_fold++;
  if(data->h.timestamp_up_min==0)
    data->h.timestamp_up_min=ptr->timestamp_up;
  if(ptr->timestamp_up<data->h.timestamp_up_min)
    data->h.timestamp_up_min=ptr->timestamp_up;
  
  if(data->h.timestamp_up_max==0)
    data->h.timestamp_up_max=ptr->timestamp_up;
  if(ptr->timestamp_up>data->h.timestamp_up_max)
    data->h.timestamp_up_max=ptr->timestamp_up;
  
  if(data->h.timestamp_min==0)
    data->h.timestamp_min=ptr->timestamp;
  if(ptr->timestamp<data->h.timestamp_min)
    data->h.timestamp_min=ptr->timestamp;
  
  if(data->h.timestamp_max==0)
    data->h.timestamp_max=ptr->timestamp;
  if(ptr->timestamp>data->h.timestamp_max)
    data->h.timestamp_max=ptr->timestamp; 

  /***************************** Start TIGRESS ***********************/
  if(ptr->channel>=map->tigr_css_min)
    if(ptr->channel<=map->tigr_css_max)
      {
	d=(ptr->channel-map->tigr_css_min)/tigr_css_group;
	pos=map->tigr_ge_map[d];
	if(pos>0)
	  if(pos<NPOSTIGR)
	    {
	      c=ptr->channel%tigr_css_group;
	      switch(c)
		{
		  /*  Blue Ge segments */
		case 0 ... 9:
		  d=c-0;col=0;type=1;
		  break;
		  /* Blue suppresors */
		case 10 ... 14:
		  d=c-10;col=0;type=2;
		  break;
		  /* Green suppresors */
		case 15 ... 19:
		  d=c-15;col=1;type=2;
		  break;
		  /*  Green Ge segments */
		case 20 ... 29:
		  d=c-20;col=1;type=1;
		  break;
		  /*  Red Ge segments */
		case 30 ... 39:
		  d=c-30;col=2;type=1;
		  break;
		  /* Red suppresors */
		case 40 ... 44:
		  d=c-40;col=2;type=2;
		  break;	      
		  /* White suppresors */
		case 45 ... 49:
		  d=c-45;col=3;type=2;
		  break;
		  /*  White Ge segments */
		case 50 ... 59:
		  d=c-50;col=3;type=1;
		  break;
		default:
		  return;
		  break;
		}
	      
	      if(type==1)
		{
		  take=0; /* assume no data */
		  if(ptr->charge_flag==1)
		    if(ptr->charge!=0)
		      {
			data->tg.det[pos].ge[col].seg[d].charge=
			  (ptr->charge/(map->tigr_css_kpar/map->tigr_css_disp));

			data->tg.det[pos].ge[col].h.EHP|=(one<<d);
			data->tg.det[pos].ge[col].h.Efold++;
			take|=1;
		      }
		  if(ptr->cfd_flag==1)
		    if(ptr->cfd!=0)
		      {
			data->tg.det[pos].ge[col].seg[d].cfd=ptr->cfd;
			data->tg.det[pos].ge[col].h.THP|=(one<<d);
			data->tg.det[pos].ge[col].h.Tfold++;
			take|=1;
		      }

		  if(waveform!=NULL)
		    if(d==0) /* only cores */	
		      if(map->tigr_ge_fit==1)
			{
			  fit_newT0(ptr->waveform_length,waveform,&wpar);
			  data->tg.det[pos].ge[col].t0[d]=wpar.t0;
			  /* printf("TIGRESS pos: %d col: %d d: %d t0: %f\n",pos,col,d,wpar.t0); */
			  /* getc(stdin); */
			  take|=1;
			}
		  
		  if(take==1)
		    {
		      data->h.setupHP|=TIGRESS_BIT;
		      
		      data->tg.det[pos].ge[col].seg[d].timestamp=ptr->timestamp;
		      data->tg.det[pos].ge[col].seg[d].timestamp_up=ptr->timestamp_up;
		      data->tg.det[pos].ge[col].h.TSHP|=(one<<d);
		      data->tg.det[pos].ge[col].h.TSfold++;
		      //count crystals
		      id=(pos-1)*NCOL+col;
		      if((data->tg.g.GeHP&(one<<id))==0)
			{
			  data->tg.g.GeHP|=(one<<id);
			  data->tg.g.Gefold++;
			}
		      if((data->tg.g.THP&(one<<id))==0)
			{
			  data->tg.g.THP|=(one<<id);
			  data->tg.g.Tfold++;
			}
		      //count positions
		      id=pos-1;
		      if((data->tg.g.PosHP&(one<<id))==0)
			{
			  data->tg.g.PosHP|=(one<<id);
			  data->tg.g.Posfold++;
			}
		      //count clovers
		      if((data->tg.h.GeHP&(one<<id))==0)
			{
			  data->tg.h.GeHP|=(one<<id);
			  data->tg.h.Gefold++;
			}
		      
		      //count crystals within a clover
		      if((data->tg.det[pos].h.GeHP&(one<<col))==0)
			{
			  data->tg.det[pos].h.GeHP|=(one<<col);
			  data->tg.det[pos].h.Gefold++;
			}
		    }
		}
	      
	      if(type==2)
		{
		  take=0; /* assume no data */
		  
		  if(ptr->charge_flag==1)
		    if(ptr->charge!=0)
		      {
			data->tg.det[pos].bgo[col].sup[d].charge=ptr->charge;
			data->tg.det[pos].bgo[col].h.EHP|=(one<<d);
			data->tg.det[pos].bgo[col].h.Efold++;
			take=1;
		      }
		  if(ptr->cfd_flag==1)
		    if(ptr->cfd!=0)
		      {
			data->tg.det[pos].bgo[col].sup[d].cfd=ptr->cfd;
			data->tg.det[pos].bgo[col].h.THP|=(one<<d);
			data->tg.det[pos].bgo[col].h.Tfold++;
			take=1;
		      }
		  if(waveform!=NULL)
		    if(map->tigr_bgo_fit==1)
		      {
			fit_newT0(ptr->waveform_length,waveform,&wpar);
		       	data->tg.det[pos].bgo[col].t0[d]=wpar.t0;
			//printf("BGO pos: %d col: %d d: %d t0: %f\n",pos,col,d,wpar.t0);
			//getc(stdin);
			take|=1;
		      }
		  
		  if(take==1)
		    {
		      data->h.setupHP|=TIGRESS_BIT;		      
		      data->tg.det[pos].bgo[col].sup[d].timestamp=ptr->timestamp;
		      data->tg.det[pos].bgo[col].sup[d].timestamp_up=ptr->timestamp_up;
		      data->tg.det[pos].bgo[col].h.TSHP|=(one<<d);
		      data->tg.det[pos].bgo[col].h.TSfold++;
		      
		      //count crystals
		      id=(pos-1)*NCOL+col;
		      if((data->tg.g.BGOHP&(one<<id))==0)
			{
			  data->tg.g.BGOHP|=(one<<id);
			  data->tg.g.BGOfold++;
			}
		      if((data->tg.g.THP&(one<<id))==0)
			{
			  data->tg.g.THP|=(one<<id);
			  data->tg.g.Tfold++;
			}
		      //count positions
		      id=pos-1;
		      if((data->tg.g.PosHP&(one<<id))==0)
			{
			  data->tg.g.PosHP|=(one<<id);
			  data->tg.g.Posfold++;
			}
		      //count clovers
		      if((data->tg.h.BGOHP&(one<<id))==0)
			{
			  data->tg.h.BGOHP|=(one<<id);
			  data->tg.h.BGOfold++;
			}
		      //count crystals within a clover
		      if((data->tg.det[pos].h.BGOHP&(one<<col))==0)
			{
			  data->tg.det[pos].h.BGOHP|=(one<<col);
			  data->tg.det[pos].h.BGOfold++;
			}
		    }
		}
	    }
      }
  /******************************stop TIGRESS*************************/

  /***************************** Start GRIFFIN ***********************/
  if(ptr->channel>=map->grif_css_min)
    if(ptr->channel<=map->grif_css_max)
      {
	d=(ptr->channel-map->grif_css_min)/grif_css_group;
	pos=map->grif_ge_map[d];
	if(pos>0)
	  if(pos<NPOSGRIF)
	    {
	      c=ptr->channel%grif_css_group;
	      switch(c)
	    	{
	    	  /*  Blue Ge core 1 */
	    	case 0:
	    	  d=0;col=0;type=1;
	    	  break;
	    	  /*  Green Ge core 1 */
	    	case 1:
	    	  d=0;col=1;type=1;
	    	  break;
	    	  /*  Red Ge core 1 */
	    	case 2:
	    	  d=0 ;col=2;type=1;
	    	  break;
	    	  /*  White Ge core 1 */
	    	case 3:
	    	  d=0;col=3;type=1;
	    	  break;
	    	  /*  Blue Ge core 2 */
	    	case 5:
	    	  d=1;col=0;type=1;
	    	  break;
	    	  /*  Green Ge core 2 */
	    	case 6:
	    	  d=1;col=1;type=1;
	    	  break;
	    	  /*  Red Ge core 2 */
	    	case 7:
	    	  d=1 ;col=2;type=1;
	    	  break;
	    	  /*  White Ge core 2 */
	    	case 8:
	    	  d=1;col=3;type=1;
	    	  break;
	    	default:
	    	  return;
	    	  break;
	    	}

	      if(type==1)
	    	{
	    	  take=0; /* assume no data */
	    	  if(ptr->charge_flag==1)
	    	    if(ptr->charge!=0)
	    	      {
			data->gr.det[pos].ge[col].seg[d].charge=
			  (ptr->charge/(map->grif_css_kpar/map->grif_css_disp));
	    		//data->gr.det[pos].ge[col].seg[d].charge=ptr->charge;
	    		data->gr.det[pos].ge[col].h.EHP|=(one<<d);
	    		data->gr.det[pos].ge[col].h.Efold++;
	    		take|=1;
	    	      }
	    	  if(ptr->cfd_flag==1)
	    	    if(ptr->cfd!=0)
	    	      {
	    		data->gr.det[pos].ge[col].seg[d].cfd=ptr->cfd;
	    		data->gr.det[pos].ge[col].h.THP|=(one<<d);
	    		data->gr.det[pos].ge[col].h.Tfold++;
	    		take|=1;
	    	      }
		  
		  if(waveform!=NULL)
		    if(map->grif_fit==1)
		      {
			get_Griffin_t0(ptr->waveform_length,waveform,&wpar);
		       	data->gr.det[pos].ge[col].t0[d]=wpar.t0;
			take|=1;
		      }

	    	  if(take==1)
	    	    {
	    	      data->h.setupHP|=GRIFFIN_BIT;
		      
	    	      data->gr.det[pos].ge[col].seg[d].timestamp=ptr->timestamp;
	    	      data->gr.det[pos].ge[col].seg[d].timestamp_up=ptr->timestamp_up;
	    	      data->gr.det[pos].ge[col].h.TSHP|=(one<<d);
	    	      data->gr.det[pos].ge[col].h.TSfold++;
	    	      //count crystals
	    	      id=(pos-1)*NCOL+col;
	    	      if((data->gr.g.GeHP&(one<<id))==0)
	    		{
	    		  data->gr.g.GeHP|=(one<<id);
	    		  data->gr.g.Gefold++;
	    		}
	    	      if((data->gr.g.THP&(one<<id))==0)
	    		{
	    		  data->gr.g.THP|=(one<<id);
	    		  data->gr.g.Tfold++;
	    		}
	    	      //count positions
	    	      id=pos-1;
	    	      if((data->gr.g.PosHP&(one<<id))==0)
	    		{
	    		  data->gr.g.PosHP|=(one<<id);
	    		  data->gr.g.Posfold++;
	    		}
	    	      //count clovers
	    	      if((data->gr.h.GeHP&(one<<id))==0)
	    		{
	    		  data->gr.h.GeHP|=(one<<id);
	    		  data->gr.h.Gefold++;
	    		}
		      
	    	      //count crystals within a clover
	    	      if((data->gr.det[pos].h.GeHP&(one<<col))==0)
	    		{
	    		  data->gr.det[pos].h.GeHP|=(one<<col);
	    		  data->gr.det[pos].h.Gefold++;
	    		}
	    	    }
	    	}
	      

	    }
      }
    /******************************stop GRIFFIN*************************/

    /*********************** start  PIN diode array ************************/
  if(ptr->channel>=map->pinarray_min)
    if(ptr->channel<=map->pinarray_max)
      {
	c=ptr->channel-map->pinarray_min;
	pos=map->pinarray_map[c];
	if(pos>0)
	  if(pos<NPIN)
	    {
	      take=0;
	      if(ptr->charge_flag==1)
		if(ptr->charge!=0)
		  {
		    data->pinarray.pin[pos].charge=
		      (ptr->charge/(map->pinarray_css_kpar/map->pinarray_css_disp));
		    //data->pinarray.pin[pos].charge=ptr->charge;
		    data->pinarray.h.Efold++;
		    data->pinarray.h.EHP|=(one<<pos);
		    take|=1;
		  }
	      if(ptr->cfd_flag==1)
		if(ptr->cfd!=0)
		  {
		    data->pinarray.pin[pos].cfd=ptr->cfd;
		    data->pinarray.h.Tfold++;
		    data->pinarray.h.THP|=(one<<pos);
		    take|=1;
		  }
	      
	      if(take==1)
		{
		  data->pinarray.pin[pos].timestamp=ptr->timestamp;
		  data->pinarray.pin[pos].timestamp_up=ptr->timestamp_up;
		  data->pinarray.h.TSfold++;
		  data->pinarray.h.TSHP|=(one<<pos);
		  data->h.setupHP|=PINArray_BIT;
		  memset(&data->pinarray.wfit[pos],0,sizeof(ShapePar));
		  data->pinarray.wfit[pos].t[1]=map->pinarray_trc;
		  if(waveform!=NULL)
		    if(map->pinarray_fit==1)
		      {
			OneComponentFitWaveform(ptr->waveform_length,waveform,&data->pinarray.wfit[pos],&wpar);
			//	print_ShapePar(&data->pinarray.wfit[pos]); 
			//	getc(stdin);
		      }
		}
	    }
      }
  /*********************** stop PIN diode array ************************/

  /*********************** start  CSI array ************************/
  if(ptr->channel>=map->csiarray_min)
    if(ptr->channel<=map->csiarray_max)
      {
				c=ptr->channel-map->csiarray_min;
				pos=map->csiarray_map[c];
				if(pos>0)
					if(pos<NCSI)
						{
							take=0;
							if(ptr->charge_flag==1)
							if(ptr->charge!=0)
								{
									data->csiarray.csi[pos].charge=
										(ptr->charge/(map->csiarray_css_kpar/map->csiarray_css_disp));
									//printf("charge for CsI array pos %d: %f\n",pos,data->csiarray.csi[pos].charge);
									//data->csiarray.csi[pos].charge=ptr->charge;
									data->csiarray.h.Efold++;
									data->csiarray.h.EHP[pos/64]|=(one<<pos%64);
									take|=1;
								}
							if(ptr->cfd_flag==1)
								if(ptr->cfd!=0)
									{
										data->csiarray.csi[pos].cfd=ptr->cfd;
										data->csiarray.h.Tfold++;
										data->csiarray.h.THP[pos/64]|=(one<<pos%64);
										take|=1;
									}
							//printf("pos %d take %d\n",pos,take);
							/*printf("pos: %d, take: %d, charge_flag: %i, cfd_flag: %i\n",pos,take,ptr->charge_flag,ptr->cfd_flag);
							printf("hp index: %i, bitshift amount: %i\n",pos/64,pos);*/
							if(take==1)
								{
									data->csiarray.csi[pos].timestamp=ptr->timestamp;
									data->csiarray.csi[pos].timestamp_up=ptr->timestamp_up;
									data->csiarray.h.TSfold++;
									data->csiarray.h.TSHP[pos/64]|=(one<<pos%64);
									/*printf("Fold: %i, HP: %64.64llx %64.64llx\n",data->csiarray.h.TSfold,data->csiarray.h.TSHP[0],data->csiarray.h.TSHP[1]);
									getc(stdin);*/
									data->h.setupHP|=CsIArray_BIT;
									memset(&data->csiarray.wfit[pos],0,sizeof(ShapePar));
									data->csiarray.wfit[pos].t[1]=map->csiarray_trc;
									data->csiarray.wfit[pos].t[2]=map->csiarray_tf;
									data->csiarray.wfit[pos].t[3]=map->csiarray_ts;
									data->csiarray.wfit[pos].t[4]=map->csiarray_tr;
									data->csiarray.wfit[pos].rf[0]=map->rf_period;
												      data->csiarray.wfit[pos].pileup_rej=map->csiarray_pileup_rej;
												      data->csiarray.wfit[pos].filter_dist=map->csiarray_filter_dist;
												      data->csiarray.wfit[pos].averaging_samples=map->csiarray_averaging_samples;
												      data->csiarray.wfit[pos].fall_amount=map->csiarray_fall_amount;
												      data->csiarray.wfit[pos].rise_amount=map->csiarray_rise_amount;
									if(waveform!=NULL)
										if(map->csiarray_fit==1)
											{
												/* t0 same way as for TIGRESS for CsI fit_t0.
													 Implemented for S1467 analysis of high energy CsI signals */
												fit_newT0(ptr->waveform_length,waveform,&wpar);
												data->csiarray.t0[pos]=wpar.t0;

												/* t0 reset during waveform fit for t0 from fit_function */
												fit_CsI_waveform(ptr->waveform_length,waveform,&data->csiarray.wfit[pos],&wpar);
											}
								}
						}
      }
  
  /*for(int csi=1;csi<NCSI;csi++)
  	if((data->csiarray.h.THP[csi/64]&(one<<csi%64))!=0)
  		{
  			printf("Hit in pos: %i\n",csi);
  			getc(stdin);
  		}*/
      
  /*********************** stop CSI array ************************/
  
  /****************start  Beam Dump PIN diode ************************/
    if(ptr->channel==map->pinbd)
    {
      take=0;
      if(ptr->charge_flag==1)
	if(ptr->charge!=0)
	  {
	    data->pinbd.h.Efold++;
	    data->pinbd.h.EHP|=1;
	    data->pinbd.pin.charge=ptr->charge;
	    take|=1;
	  }
      if(ptr->cfd_flag==1)
	if(ptr->cfd!=0)
	  {
	    data->pinbd.h.Tfold++;
	    data->pinbd.h.THP|=1;
	    data->pinbd.pin.cfd=ptr->cfd;
	    take|=1;
	  }
      if(take==1)
	{
	  data->pinbd.h.TSfold++;
	  data->pinbd.h.TSHP|=1;
	  data->pinbd.pin.timestamp=ptr->timestamp;
	  data->pinbd.pin.timestamp_up=ptr->timestamp_up;
	  data->h.setupHP|=BDPIN_BIT;
       }
    }
  /****************stop  Beam Dump PIN diode ************************/

  /****************start RF ************************/
  if(ptr->channel==map->rf)
    {
      if(waveform!=NULL)
	{
	  //printf("waveform not null\n"); getc(stdin);
	  if(get_sin_par(map->rf_period,ptr->waveform_length,waveform,&data->rf.sin)==1)
	    {
	      data->rf.sin.t0*=16;
	      data->rf.ch.charge=ptr->charge;
	      data->rf.ch.cfd=ptr->cfd;
	      data->rf.ch.timestamp=ptr->timestamp;
	      data->rf.ch.timestamp_up=ptr->timestamp_up;
	      data->h.setupHP|=RF_BIT;
	      //printf("t0 %f RF_BIT %d h.setupHP&RF_BIT %d\n",data->rf.sin.t0,RF_BIT,data->h.setupHP&RF_BIT);
	      //getc(stdin);
	    }
	}
      //      else
      //	printf("NULL pointer to the RF waveform in event %d\n",data->h.trig_num);
    }
  /****************stop RF ************************/

  /***************************** Start S3 ***********************/
  if(ptr->channel>=map->s3_min)
    if(ptr->channel<=map->s3_max)
      {
	c=(ptr->channel-map->s3_min)/s3_group;
	d=ptr->channel-map->s3_min-c*s3_group;

	if(map->s3_gid[c]==1) /*map to sectors*/
	  {
	    pos=map->s3_sec[map->s3_ord[c]][d];
	    take=0;
	    if(ptr->charge_flag==1)
	      if(ptr->charge!=0)
		{
		  data->s3.sh.Efold++;
		  data->s3.sh.EHP|=(one<<pos);
data->s3.sec[pos].charge=ptr->charge/(map->s3_css_kpar/map->s3_css_disp);
//data->s3.sec[pos].charge=ptr->charge;
		  take|=1;
		}
	    if(ptr->cfd_flag==1)
	      if(ptr->cfd!=0)
		{
		  data->s3.sh.Tfold++;
		  data->s3.sh.THP|=(one<<pos);
		  data->s3.sec[pos].cfd=ptr->cfd;
		  take|=1;
		}
	    if(take==1)
	      {
		data->s3.sh.TSfold++;
		data->s3.sh.TSHP|=(one<<pos);
		data->s3.sec[pos].timestamp=ptr->timestamp;
		data->s3.sec[pos].timestamp_up=ptr->timestamp_up;
		data->h.setupHP|=S3SEC_BIT;
	      }	    
	  }

	if(map->s3_gid[c]==2) /*map to rings*/
	  {
	    pos=map->s3_ring[map->s3_ord[c]][d];
	    take=0;
	    if(ptr->charge_flag==1)
	      if(ptr->charge!=0)
		{
		  data->s3.rh.Efold++;
		  data->s3.rh.EHP|=(one<<pos);
		  data->s3.ring[pos].charge=ptr->charge/(map->s3_css_kpar/map->s3_css_disp);
		  //data->s3.ring[pos].charge=ptr->charge
		  take|=1;
		}
	    if(ptr->cfd_flag==1)
	      if(ptr->cfd!=0)
		{
		  data->s3.rh.Tfold++;
		  data->s3.rh.THP|=(one<<pos);
		  data->s3.ring[pos].cfd=ptr->cfd;
		  take|=1;
		}
	    if(take==1)
	      {
		data->s3.rh.TSfold++;
		data->s3.rh.TSHP|=(one<<pos);
		data->s3.ring[pos].timestamp=ptr->timestamp;
		data->s3.ring[pos].timestamp_up=ptr->timestamp_up;
		data->h.setupHP|=S3RING_BIT;
	      }	    
	  }
      }
  return;
}

/*======================================================================*/
