#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  long int t;
  double trf;
  
  if(data->pinarray.h.Tfold>0)
    {
      count_1++;
      for(pin=1;pin<NPIN;pin++)
	{
	  count_2++;
	  if((data->pinarray.h.THP&(one<<pin))!=0)
	    {
	      count_3++;
	      //The bitmasking in the next line is redundant and unneeded since it's done in tig-format.c.
	      //The bitmasking is needed for the timestamp line as we are adding on hex digit.
	      t=data->pinarray.pin[pin].cfd&0x00ffffff;
	      t-=(data->pinarray.pin[pin].timestamp*16)&0x00ffffff;

	      if(((data->h.setupHP&RF_BIT)!=0))
		{
		  count_4++;
		  trf=data->rf.sin.t0;

		  g->Fill(trf);
		  h->Fill(trf,t);
		  
		  if(trf<0 || trf>S4K) trf==S4K-100;
		  histRF[pin][(int)rint(trf)]++;		  
		  
		  t-=(int)rint(trf); 
		  t+=S16K;
		  
		  if(t<0)    t=S32K-200;
		  if(t>S32K) t=S32K-300;
		}
	      else t=S32K-400;
	      hist[pin][t]++;
	    }}}  
  return SEPARATOR_DISCARD;
}
/*=======================================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *ClusterFile, *DataFile;
  input_names_type *name;
  char DataFileEntry[132];
  TApplication *theApp;
  
  if(argc!=2)
    {
      printf("PINArray_RFUnwrapping master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("CFD-TS vs RF","CFD-TS vs RF",720,-10,150,16385,-8192,8192);
  h->Reset();
  g = new TH1D("RFphase","RFphase",720,-10,150);
  g->Reset();
  
  printf("Program sorts wrapped TRawSum histogram for the PINARRAY.\n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  memset(hist,0,sizeof(hist));
  memset(histRF,0,sizeof(histRF));
  read_master(argv[1],name);

  if(name->flag.cluster_file==1)
    {
      printf("Sorting wrapped PINARRAY TRaw histograms based upon the cluster file: %s\n",name->fname.cluster_file);
      if((ClusterFile=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("ERROR! I can't open input file %s\n",name->fname.cluster_file);
	  exit(-2);
	}}
  else
    {
      printf("ERROR! Cluster file not defined\n");
      exit(-1);
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
      
      printf("Sorting RF wrapped PINARRAY timing data from file %s\n", DataFileEntry);
      sort(name);
    }
  
  if((output=fopen("RFphases.spn","w"))==NULL)
    {
      printf("ERROR! I can't open the spn file!");
      exit(EXIT_FAILURE);
    }
  for(int pin=0;pin<NPIN;pin++)
    fwrite(histRF[pin],S4K*sizeof(int),1,output);
  fclose(output);

  theApp=new TApplication("App", &argc, argv);
  TCanvas *c1 = new TCanvas("TRaw", "TRaw",100,100, 1000, 600);
  gStyle->SetPalette(1);
  TPad *c1_1 = new TPad("c1_1","c1_1",0,0,1,1);
  //TPad *c1_1 = new TPad("c1_1","c1_1",0,0,1,0.5);
  c1_1->Draw();
  c1_1->cd();
  gPad->SetLogz(1);
  h->Draw("COLZ");
  c1->cd();
  //TPad *c1_2 = new TPad("c1_2","c1_2",0,0.5,1,1);
  //c1_2->Draw();
  //c1_2->cd();
  //g->Draw();
  //c1_1->Modified();
  //c1->cd();
  theApp->Run(kTRUE);
  
  printf("The event-dropper increments are %d %d %d %d.\n",count_1,(count_2/(NS3RING-1)),count_3,count_4);
}
