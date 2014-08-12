#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int csi;
  long int tcsi,ttg,t;
   if(data->csiarray.h.Tfold>0)
    for(csi=1;csi<NCSI;csi++)
      if((data->csiarray.h.THP&(one<<csi))!=0)
	{
	  tcsi=data->csiarray.csi[csi].cfd&0x00ffffff;
	  tcsi-=(data->csiarray.csi[csi].timestamp*16)&0x00ffffff;
	  if(data->tg.h.Gefold>0)
	    for(pos=1;pos<NPOSTIGR;pos++)
	      if((data->tg.h.GeHP&(1<<(pos-1)))!=0)
		if(data->tg.det[pos].h.Gefold>0)
		  for(col=0;col<NCOL;col++)
		    if((data->tg.det[pos].h.GeHP&(1<<col))!=0)
		      if(data->tg.det[pos].ge[col].h.Tfold>0)
			if((data->tg.det[pos].ge[col].h.THP&1)!=0)
			  {
			    ttg=data->tg.det[pos].ge[col].seg[0].cfd&0x00ffffff;
			    ttg-=(data->tg.det[pos].ge[col].seg[0].timestamp*16)&0x00ffffff;
			    t=ttg-tcsi;
			    t*=10;
			    t/=16;
			    t+=S1K/2;
			    if(t<0)t=S4K-2;
			    if(t>S4K-10) t=S4K-10;
			    hist[pos][col][t]++;
			    h->Fill(pos,t);
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
      printf("\n ./TIGRESS_CSIARRAY_TTRawDiffCluster master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("TTDiff","TTDiff",NPOSTIGR,0,NPOSTIGR,S4K,0,S4K);
  h->Reset();

  printf("Program sorts calibrated histograms for TIGRESS/CSIARRAY CFD timing difference \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
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
   if(name->flag.TIGRESS_cal_par==1)
     {
          printf("\nTIGRESS calibration read from the file:\n %s\n",name->fname.TIGRESS_cal_par);
          initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
        }
      else
        {
          printf("\nTIGRESS calibration parameters not defined\n");
          exit(-1);
        }

  name->flag.inp_data=1; 

  while(fscanf(cluster,"%s",n)!=EOF)
    {
      strcpy(name->fname.inp_data,n);
      sort(name);
    }
  fclose(cluster);

  for(pos=1;pos<NPOSTIGR;pos++)
    {
      stop=0;
      for(col=0;col<NCOL;col++)
	stop+=cal_par->tg.ceflag[pos][col];
      if(stop>0)	
	{
	  sprintf(n,"pos%1d%1d_Tigress_CSI_CFD_diff.spn",pos/10,pos-(pos/10)*10);
	  if((output=fopen(n,"w"))==NULL)
	    {
	      printf("\nI can't open file %s\n",n);
	      exit(EXIT_FAILURE);
	    }

	  for(col=0;col<NCOL;col++)
	    {
	      fwrite(hist[pos][col],S4K*sizeof(int),1,output);
	    }
	  fclose(output);
	}
    }

 
 
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("TTCal", "TTCal",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

