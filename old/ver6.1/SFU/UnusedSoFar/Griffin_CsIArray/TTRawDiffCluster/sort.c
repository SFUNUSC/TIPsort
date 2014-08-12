#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int csi;
  long int tcsi,tgr;
  Int_t t1,t2;
  int   t;
  if(data->csiarray.h.Tfold>0)
    for(csi=1;csi<NCSI;csi++)
      if((data->csiarray.h.THP&(one<<csi))!=0)
	{
	  tcsi=data->csiarray.csi[csi].cfd&0x00ffffff;
	  t1=tcsi-(data->csiarray.csi[csi].timestamp*16&0x00ffffff);
	  t1+=S16K;
	  //	  tcsi+=S2K;
	  if(data->gr.h.Gefold>0)
	    for(pos=1;pos<NPOSGRIF;pos++)
	      if((data->gr.h.GeHP&(1<<(pos-1)))!=0)
		if(data->gr.det[pos].h.Gefold>0)
		  for(col=0;col<NCOL;col++)
		    if((data->gr.det[pos].h.GeHP&(1<<col))!=0)
		      if(data->gr.det[pos].ge[col].h.Tfold>0)
			if((data->gr.det[pos].ge[col].h.THP&1)!=0)
			  {
			    tgr=data->gr.det[pos].ge[col].seg[0].cfd&0x00ffffff;
			    t2=tgr-(data->gr.det[pos].ge[col].seg[0].timestamp*16&0x00ffffff);
			    t2+=S16K;
			    t=tgr-tcsi;
			    //			    t*=10;
			     t/=16;
			     t+=2048;
			    if(t<0)t=S4K-2;
			    if(t>S4K-10) t=S4K-10;
			    hist[t]++;
			    
			    h->Fill(t1/16,t2/16);
			  }

	}	

  return SEPARATOR_DISCARD;
 }
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  FILE *cluster,*output;
  char n[132];
  TCanvas *canvas;
  TApplication *theApp;
  int stop;

  if(argc!=2)
    {
      printf("\n ./GRIFESS_CSIARRAY_TTRawDiffCluster master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("TT","TT",S4K,0,S4K,S4K,0,S4K);
  h->Reset();

  printf("Program sorts raw histograms for GRIFESS/CSIARRAY CFD timing difference \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  memset(hist,0,sizeof(hist));

  read_master(argv[1],name);

   if(name->flag.cluster_file==1)
        {
          printf("\nSorting data from cluster file:\n %s\n",name->fname.cluster_file);
	  if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	    {
	      printf("\nI can't open input file %s\n",name->fname.cluster_file);
	      exit(-2);
	    }
        }
      else
        {
          printf("\nCluster file not defined\n");
          exit(-1);
        }
  
  name->flag.inp_data=1; 

  while(fscanf(cluster,"%s",n)!=EOF)
    {
      strcpy(name->fname.inp_data,n);
      sort(name);
    }
  fclose(cluster);

  
  	  if((output=fopen("TT.spn","w"))==NULL)
  	    {
  	      printf("\nI can't open file %s\n",n);
  	      exit(EXIT_FAILURE);
  	    }

  	      fwrite(hist,S4K*sizeof(int),1,output);
  
  	  fclose(output);
 

 
 
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("TTCal", "TTCal",10,10, 500, 500);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

