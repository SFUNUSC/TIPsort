#include "sort.h"

int analyze_data(raw_event *data)
{

  int t;
  
  if(data->pinbd.h.Tfold>0)
      if(data->pinbd.h.THP!=0)
	{
	  t=data->pinbd.pin.cfd&0x00ffffff;
	  t-=(data->pinbd.pin.timestamp*16)&0x00ffffff;
	  if((data->h.setupHP&RF_BIT)!=0)
	    {
	      t-=(int)data->rf.sin.t0;
	      t+=S16K;
	      if(t<0)t=S32K-2;
	      if(t>S32K) t=S32K-3;
	    }
	  else
	    t=S32K-4;

	  hist[(int)rint(t)]++;
	  h->Fill(t);
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
      printf("\n ./PINBD_TRaw sfu_input_data_file_name\n");
      exit(-1);
    }
  
  h = new TH1D("PINBD Raw Time","PINBD Raw Time",S32K,0,S32K);
  h->Reset();

  printf("Program sorts raw time histogram for the beam dump PIN \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);

  if((output=fopen("PINBD_raw_time.mca","w"))==NULL)
    {
      printf("\nI can't open file %s\n","PINArray_raw_time.mca");
      exit(EXIT_FAILURE);
    }
  
    fwrite(hist,S32K*sizeof(int),1,output);

  fclose(output);
   
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("PINBD Raw Time", "PINBD Raw Time",10,10, 500, 300);
  gPad->SetLogy(1);
  h->Draw();
  
  theApp->Run(kTRUE);

}

