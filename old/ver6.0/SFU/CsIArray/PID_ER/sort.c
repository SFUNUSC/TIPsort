#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double chi,s,f,r,e;


  //  for(pos=1;pos<NCSI;pos++)
  pos=det_id;
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
		  s=data->csiarray.wfit[pos].am[3];
		  f=data->csiarray.wfit[pos].am[2];
		  r=s/f*100;
		  h->Fill(e,100+r);
		  g->Fill(f,s);
		  if(r>S32K-4) r=S32K-4;
		  hist[pos][(int)rint(r)]++;
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
      printf("\n ./CSIARRAY_PID_ER sfu_input_data_file_name idmin idmax chimin chimax det_id\n");
      exit(-1);
    }
  h = new TH2D("PID","PID",S1K,0,S4K,S1K,0,S1K);
  h->Reset();
  g = new TH2D("PID F/S","PID F/S",S1K,0,S2K,S1K,0,S2K);
  g->Reset();
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  det_id=atoi(argv[6]);
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
  canvas = new TCanvas("PID_ER", "PID_ER",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);
 g->Draw("COLZ");
  
  theApp->Run(kTRUE);

}
