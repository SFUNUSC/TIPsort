#include "sort.h"

int analyze_data(raw_event *data)
{
  double tbgo,trf,func;
  //int take;
  long int a;
  
  if(data->tg.h.BGOfold>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((data->tg.h.BGOHP&(1<<(pos-1)))!=0)
	if(data->tg.det[pos].h.BGOfold>0)
	  for(col=0;col<NCOL;col++)
	    if(((data->tg.det[pos].h.BGOHP)&(1<<col))!=0)
	      if(data->tg.det[pos].bgo[col].h.Tfold>0)
		for(sup=0;sup<NSUP;sup++)
		  if(((data->tg.det[pos].bgo[col].h.THP&(1<<sup))!=0))
		    {
		      tbgo=data->tg.det[pos].bgo[col].sup[sup].cfd&0x00ffffff;
		      tbgo-=(data->tg.det[pos].bgo[col].sup[sup].timestamp*16)&0x00ffffff;

		      /* if(pos==7 && col==0 && sup==3) */
		      /* 	{ */
		      /* 	  printf("BGO pos %2d col %d sup %d time %f ts %d\n",pos,col,sup,tbgo,(data->tg.det[pos].bgo[col].sup[sup].timestamp*16)&0x00ffffff); */
		      /* 	  getc(stdin); */
		      /* 	} */


		      //set RF time only if RF is in the hit pattern
		      if((data->h.setupHP&RF_BIT)!=0)
			{
			  trf=data->rf.sin.t0;

			  //No RF unwrapping
			  /* if(cal_par->DoRFUnwrapping==1) */
			  /*   { */
			  /*     func=(1.000*trf)-(RFphase+cal_par->offset); */
			  /*     a=fmod(tbgo+cal_par->shift,RFphase); */
			      
			  /*     if(a>func)  */
			  /*     	trf+=RFphase; */
			  /*   } */
			}
		      //else set it to 0
		      else
			trf=0.;
		      
		      tbgo-=trf;
		      
		      tbgo-=trf;
		      tbgo+=S16K;
		      
		      if((tbgo>=0) && (tbgo<S32K))
			{
			  hist[pos][col][sup][(int)rint(tbgo)]++;
			  //take=1;
			}
		      else hist[pos][col][sup][S32K-5000]++;
		    }

return SEPARATOR_DISCARD;
}

/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  char mcaFile[132];
  
  if(argc!=2)
    {
      printf("Tigress_BGOTRaw sfu_input_data\n");
      exit(-1);
    }
  
  printf("Program sorts BGOTRaw histograms for Tigress suppressors.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
    
  strcpy(name->fname.inp_data,argv[1]);
  memset(hist,0,sizeof(hist));
    
  sort(name);

  for(pos=1;pos<NPOSTIGR;pos++)
    for(col=0;col<NCOL;col++)
      {
	sprintf(mcaFile,"pos%1d%1d_col%d_BGOTRaw.mca",pos/10,pos-(pos/10)*10,col);
	if((output=fopen(mcaFile,"w"))==NULL)
	  {
	    printf("ERROR!!! I cannot open the mca file!\n");
	    exit(EXIT_FAILURE);
	  }
	for(sup=0;sup<NSUP;sup++) fwrite(hist[pos][col][sup],S32K*sizeof(int),1,output);
	fclose(output);
      }
}
