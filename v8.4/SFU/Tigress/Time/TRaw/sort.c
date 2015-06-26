#include "sort.h"

int analyze_data(raw_event *data)
{
  long int tt,a;
  double t;
  double trf,func;

  //Check Ge fold
  if(data->tg.h.Gefold>0)
    //Run through all positions
    for(pos=1;pos<NPOSTIGR;pos++)
      //Check if the position is indicated in the hit pattern
      if((data->tg.h.GeHP&(1<<(pos-1)))!=0)
	//Check if the position has a fold great than zero
	if(data->tg.det[pos].h.Gefold>0)
	  //Run through four cores for each position
	  for(col=0;col<NCOL;col++)
	    //Check if this position is indicated in the hit pattern
	    if((data->tg.det[pos].h.GeHP&(1<<col))!=0)
	      //Check that this combination has a time fold great than zero
	      if(data->tg.det[pos].ge[col].h.Tfold>0)
		//Check if this combination is indicated in the time hit pattern
		if((data->tg.det[pos].ge[col].h.THP&1)!=0)
		  {
		    //The bitmasking in the next line is redundant and unneeded since it's done in tig-format.c.
		    //The bitmasking is needed for the timestamp line as we are adding one hex digit.
		    
		    //t=data->tg.det[pos].ge[col].t0[0]*16;
		    
		    t=data->tg.det[pos].ge[col].seg[0].cfd&0x00ffffff;
		    t-=(data->tg.det[pos].ge[col].seg[0].timestamp*16)&0x00ffffff;
		    
		    //This might not work for data with RF since the unwrapping setup is not up to date.
		    if(((data->h.setupHP&RF_BIT)!=0))
		      {
			trf=data->rf.sin.t0;
			
			g->Fill(trf);
			
			/* func=(1.00*trf)-(RFphase+offset); */
			/* a=fmod(t+shift,RFphase); */
			
			/* if(func<a)  */
			/*   trf+=RFphase; */
			
			/* //Histograms after unwrapping RF. */
			/* h->Fill(trf,t); */

			/* if(trf<0 || trf>S4K)  */
			/*   trf=S4K-100; */
			
			histRF[pos][col][(int)rint(trf)]++;
		      }
		    else 
		      trf=0.;
		    
		    t-=trf;
		    t+=S16K;
		    
		    tt=(int)rint(t);

		    //printf("t %f tt %d\n",t,tt);

		    if(tt<0)    
		      tt=S32K-1000;
		    if(tt>S32K) 
		      tt=S32K-2000;
		    
		    hist[pos][col][tt]++;
		  }

  return SEPARATOR_DISCARD;
}
/*=============================================================*/
int main(int argc, char *argv[])
{
  FILE *output;
  input_names_type *name;
  char n[132];
  TApplication *theApp;

  if(argc!=2)
    {
      printf("Tigress_TRaw master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("CFD-TS vs RF","CFD-TS vs RF",720,-10,300,16385,-8192,8192);
  h->Reset();
  g = new TH1D("","",720,-10,300);
  g->Reset();

  printf("Program sorts RF unwrapped TRaw histograms for Tigress cores and crystals.\n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  memset(hist,0,sizeof(hist));
  memset(histRF,0,sizeof(histRF));
  read_master(argv[1],name);
  
  if(name->flag.inp_data!=1)
    {
      printf("ERROR! Input data file not defined.\n");
      exit(EXIT_FAILURE);
    }

  printf("Sorting timing data from file %s\n",name);

  sort(name);
  
  for(pos=1;pos<NPOSTIGR;pos++)
    {
      sprintf(n,"pos%1d%1d_TRawCore.mca",pos/10,pos-(pos/10)*10);
      if((output=fopen(n,"w"))==NULL)
	{
	  printf("ERROR! I can't open file %s\n",n);
	  exit(EXIT_FAILURE);
	}
      
      for(col=0;col<NCOL;col++)
	fwrite(hist[pos][col],S32K*sizeof(int),1,output);
      
      fclose(output);
    }
  
  for(pos=1;pos<NPOSTIGR;pos++)
    {
      sprintf(n,"pos%1d%1d_RFphase.spn",pos/10,pos-(pos/10)*10);
      if((output=fopen(n,"w"))==NULL)
	{
	  printf("ERROR! I can't open file %s\n",n);
	  exit(EXIT_FAILURE);
	}
      
      for(col=0;col<NCOL;col++)
	fwrite(histRF[pos][col],S4K*sizeof(int),1,output);
      
      fclose(output);
    }

  return 0;

}
