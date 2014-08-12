#include "sort.h"

/*-----------------------------------------------------*/
int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double chi,t,e;


  pos=detector;
    if((data->csiarray.h.TSHP&(one<<pos))!=0)
      {
	type=data->csiarray.wfit[pos].type;
	chi=data->csiarray.wfit[pos].chisq;
	chi/=data->csiarray.wfit[pos].ndf;
	if(type>=idmin)
	  if(type<=idmax)
	    if(chi>=chimin)
	      if(chi<=chimax)
		{
		  t=data->csiarray.wfit[pos].t[0]*16;
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

		  e=data->csiarray.csi[pos].charge;
		
		  h->Fill(t,e);
		  if(t>S32K-4)
		    t=S32K-4;
		  if(e>S32K-4)
		    e=S32K-4;
		  hist[0][(int)rint(t)]++;
		  hist[pos][(int)rint(e)]++;
		}
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


  if(argc!=7)
    {
      printf("\n ./CSIARRAY_PID_TR sfu_input_data_file_name idmin idmax chimin chimax detector\n");
      exit(-1);
    }
  h = new TH2D("PID","PID",400,250,650,S4K,0,S32K);
  h->Reset();
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  detector=atoi(argv[6]);
  printf("Program sorts PID histogram for the CSIARRAY \n");

  memset(hist,0,sizeof(hist));
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);

  if((output=fopen("CSIArray_PID_ratio.mca","w"))==NULL)
    {
      printf("\nI can't open file %s\n","CSIArray_PID_ratio.mca");
      exit(EXIT_FAILURE);
    }
  for(int pos=0;pos<NCSI;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);

  fclose(output);
 theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("PID_ET", "PID_ET",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

