#include "sort.h"

int analyze_data(raw_event *data)
{
  long int t;
  double trf;

  if(data->tg.h.Gefold>0) //Check Ge fold
    for(pos=1;pos<NPOSTIGR;pos++) //Run through all positions
      if((data->tg.h.GeHP&(1<<(pos-1)))!=0) //Check if pos is indicated in the hit pattern
	if(data->tg.det[pos].h.Gefold>0) //Check if pos has a fold great than zero
	  for(col=0;col<NCOL;col++) //Run through four cores
	    if((data->tg.det[pos].h.GeHP&(1<<col))!=0) //Check if pos/col is indicated in the hit pattern
	      if(data->tg.det[pos].ge[col].h.Tfold>0) //Check if pos/col has time fold great than zero
		if((data->tg.det[pos].ge[col].h.THP&1)!=0) //Check if pos/col is indicated in the time hit pattern
		  {
		    //The bitmasking in the next line is redundant and unneeded since it's done in tig-format.c.
		    //The bitmasking is needed for the timestamp line as we are adding one hex digit.
		    t=data->tg.det[pos].ge[col].seg[0].cfd&0x00ffffff;
		    t-=(data->tg.det[pos].ge[col].seg[0].timestamp*16)&0x00ffffff;
		    
		    if(((data->h.setupHP&RF_BIT)!=0))
		      {
			trf=data->rf.sin.t0;
			
			//Histograms prior to unwrapping RF.
			g->Fill(trf);
			h->Fill(trf,t);
			
			if(trf<0 || trf>S4K) trf==S4K-100;
			histRF[pos][col][(int)rint(trf)]++;
			
			t-=(int)rint(trf);
			t+=S16K;
			
			if(t<0)    t=S32K-200;
			if(t>S32K) t=S32K-300;
		      }
		    else t=S32K-400;
		    hist[pos][col][t]++;
		  }
  return SEPARATOR_DISCARD;
}
/*=============================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *ClusterFile, *DataFile;
  input_names_type *name;
  char n[132], DataFileEntry[256], ClusterFileName[256];
  TApplication *theApp;
  
  if(argc!=2)
    {
      printf("Tigress_RFUnwrappingSumSum cluster_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("CFD-TS vs RF","CFD-TS vs RF",320,-10,140,8192,-8192,8192);
  h->Reset();
  g = new TH1D("","",720,-10,300);
  g->Reset();
  
  printf("Program sorts RF wrapped TRawSum histograms for Tigress cores and crystals.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  memset(hist,0,sizeof(hist));
  memset(histRF,0,sizeof(histRF));
  strcpy(ClusterFileName,argv[1]);

  if((ClusterFile=fopen(ClusterFileName,"r")) == NULL)
    {
      printf("ERROR! I cannot find %s\n",ClusterFileName);
      exit(-2);
    }
  
  while(fscanf(ClusterFile,"%s",DataFileEntry) != EOF)
    {
      if((DataFile=fopen(DataFileEntry,"r")) == NULL)
	{
	  printf("ERROR! I cannot find %s\n",DataFileEntry);
	  exit(-2);
	}
      
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFileEntry);
      
      printf("Sorting timing data from file %s\n", DataFileEntry);
      sort(name);
    }
  
  for(pos=1;pos<NPOSTIGR;pos++)
    {
      sprintf(n,"pos%1d%1d_RawClusterCoreTime.mca",pos/10,pos-(pos/10)*10);
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
  
  theApp=new TApplication("App", &argc, argv);
  TCanvas *c1 = new TCanvas("TRaw", "TRaw",100,100, 1000, 600);
  gStyle->SetPalette(1);
  gPad->SetLogz(1);
  TPad *c1_1 = new TPad("c1_1","c1_1",0,0,1,1);
  //TPad *c1_1 = new TPad("c1_1","c1_1",0,0,1,0.5);
  c1_1->Draw();
  c1_1->cd();
  h->Draw("COLZ");
  c1_1->Modified();
  c1->cd();
  //TPad *c1_2 = new TPad("c1_2","c1_2",0,0.5,1,1);
  //c1_2->Draw();
  //c1_2->cd();
  //g->Draw();
  //c1_2->Modified();
  //c1->cd();
  theApp->Run(kTRUE);
}
