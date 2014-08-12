#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int sec;
  int t;
  
  if(data->s3.sh.Tfold>0)
    for(sec=1;sec<NS3SEC;sec++)
      if((data->s3.sh.THP&(one<<sec))!=0)
	{
	  t=data->s3.sec[sec].cfd&0x00ffffff;
	  t-=(data->s3.sec[sec].timestamp*16)&0x00ffffff;
	  if((data->h.setupHP&RF_BIT)!=0)
	    {
	      t-=(int)data->rf.sin.t0;
	      t+=S16K;
	      if(t<0)t=S32K-2;
	      if(t>S32K) t=S32K-3;
	    }
	  else
	    t=S32K-4;

	  hist[sec][(int)rint(t)]++;
	  h->Fill(t,sec);
	  }
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;

  if(argc!=2)
    {
      printf("\n ./S3_SecTRaw sfu_input_data_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("Raw Time","Raw Time",S32K,0,S32K-1,NS3SEC+1,0,NS3SEC);
  h->Reset();

  printf("Program sorts raw time histogram for the S3 \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);

  if((output=fopen("S3_sec_raw_time.mca","w"))==NULL)
    {
      printf("\nI can't open file %s\n","PINArray_raw_time.mca");
      exit(EXIT_FAILURE);
    }
  for(int sec=0;sec<NS3SEC;sec++)
    fwrite(hist[sec],S32K*sizeof(int),1,output);

  fclose(output);
   
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Raw Time", "Raw Time",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

