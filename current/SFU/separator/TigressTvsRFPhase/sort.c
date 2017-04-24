#include "sort.h"

int analyze_data(raw_event *data)
{
  double tFITlocal,tCFD,trf;//,chi;
  double tdiffFITlocal;
  //double tdiffCFD;
  int drop=0;

  if((data->h.setupHP&RF_BIT)!=0)

    if(data->tg.h.Gefold>0)
      for(pos=1;pos<NPOSTIGR;pos++)
	if((data->tg.h.GeHP&(1<<(pos-1)))!=0)
	  if(data->tg.det[pos].h.Gefold>0)
	    for(col=0;col<NCOL;col++)
	      if((data->tg.det[pos].h.GeHP&(1<<col))!=0)
		if(data->tg.det[pos].ge[col].h.Tfold>0)
		  if((data->tg.det[pos].ge[col].h.THP&1)!=0)
		    {
		      tCFD=data->tg.det[pos].ge[col].seg[0].cfd&0x00ffffff;
		      tCFD-=(data->tg.det[pos].ge[col].seg[0].timestamp*16)&0x00ffffff;
		      
		      /* chi=data->tg.det[pos].ge[col].t0[0].chisq; */
		      /* chi/=data->tg.det[pos].ge[col].t0[0].ndf; */
		      
		      /* if(chi>=chimin && chi<=chimax) */
		      /* { */
			    //In ns, fit local
			    tFITlocal=data->tg.det[pos].ge[col].t0[0]*10;
			    
			    //In CFD units
			    tCFD=data->tg.det[pos].ge[col].seg[0].cfd&0x00ffffff;
			    tCFD-=(data->tg.det[pos].ge[col].seg[0].timestamp*16)&0x00ffffff;
			    
			    //RF in CFD units
			    trf=data->rf.sin.t0;
			    
			    //func=(1.000*trf)-(RFphase+offset);
			    //a=fmod(tFITlocal+shift,RFphase);
			    //printf("trf %f RFphase %f tFITlocal %f offset %f func %f shift %f a %ld\n",trf,RFphase,tFITlocal,offset,func,shift,a);
			    //if(a>func) 
			    //trf+=RFphase;
			    //printf("new trf %f\n",trf);
			    //getc(stdin);
			    
			    //RF in ns
			    trf*=0.625;
			    
			    //Subtract RF and convert to ns
			    tFITlocal*=0.625;
			    tdiffFITlocal=tFITlocal-trf;
			    
			    /*//Subtract RF and convert to ns
			    tCFD*=0.625;
			    tdiffCFD=tCFD-trf;
			    tCFD+=S4K+S1K;*/
			    
			    if((tdiffFITlocal<min_t)||(tdiffFITlocal>max_t))
		          drop=1; //drop this event
			  
		    }
  
	if(drop==0)
		{
			encode(data,output,enb);
			//printf("Event encoded with np %d na %d\n\nEND OF EVENT\n",np,na);
		}
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;

  if(argc!=4)
    {
      printf("\n ./separate_CsIArray_PID_ER master_file_name min_t max_t\n");
      exit(-1);
    }
 
  printf("Program separates events according to the difference between RF phase and local fit time.\n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));

  read_master(argv[1],name);
  min_t=atof(argv[2]);
  max_t=atof(argv[3]);

  if(name->flag.inp_data!=1)
    {
      printf("\nInput data file not defined\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.out_data!=1)
    {
      printf("\nOutput data file not defined\n");
      exit(EXIT_FAILURE);
    }

  if((output=fopen(name->fname.out_data,"w"))==NULL)
    {
      printf("\nI can't open output file %s for writing\n",name->fname.out_data);
      exit(-2);
    }

  memset(enb,0,sizeof(enb));
  enb[0]=BUFFER_TAG;
  enb[1]++;
  enb[1]++;

  sort(name);
  
	//save the last buffer which will be dropped otherwise
	//if enb[1]==2 then the buffer contains no data, only the header
	if(enb[1]>2)
		fwrite(enb,sizeof(int),BUFFSIZE,output);
 
}

