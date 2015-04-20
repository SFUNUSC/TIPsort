#include "sort.h"

/*-----------------------------------------------------*/
int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double chi,t,e;


  pos=det_no;
    if((data->csiarray.h.THP&(one<<pos))!=0)
      {
	type=data->csiarray.wfit[pos].type;
	chi=data->csiarray.wfit[pos].chisq;
	chi/=data->csiarray.wfit[pos].ndf;
	if(type>=idmin)
	  if(type<=idmax)
	    if(chi>=chimin)
	      if(chi<=chimax)
		{
		  
		  e=data->csiarray.csi[pos].charge;
		  t=16.*data->csiarray.wfit[pos].t[0]; 
		  if((data->h.setupHP&RF_BIT)!=0)
		    {
		      t-=data->rf.sin.t0;
		      
		      t+=S4K;
		      t/=16;
		      if(t<0)t=S32K-2;
		      if(t>S32K) t=S32K-3;
		    }
		  else
		    t=S32K-4;

		  h->Fill(t,e);
	
		}
      }
 
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
 
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;


  if(argc!=7)
    {
      printf("\n ./CSIARRAY_ERawTFitRaw sfu_input_data_file_name idmin idmax chimin chimax det\n");
      exit(-1);
    }
  h = new TH2D("ERawTFitRaw","ERawTFitRaw",400,250,650,S1K,0,S16K);
  h->Reset();
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  det_no=atoi(argv[6]);
  printf("Program sorts ERaw TFitRaw histogram for the CSIARRAY \n");

  memset(hist,0,sizeof(hist));
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);

 
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("ERawTFitRaw", "ERawTFitRaw",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

