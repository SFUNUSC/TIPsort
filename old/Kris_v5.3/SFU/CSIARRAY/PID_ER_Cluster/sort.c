#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double chi,s,f,r,e;


  for(pos=1;pos<NCSI;pos++)
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
		  e=data->csiarray.wfit[pos].am[1];
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
  FILE  *cluster,*output;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  char n[132];

  if(argc!=6)
    {
      printf("\n ./CSIARRAY_PID_ER_Cluster cluster_file_name idmin idmax chimin chimax\n");
      exit(-1);
    }
  h = new TH2D("PID","PID",S1K,0,S2K,S1K,0,S2K);
  h->Reset();
  g = new TH2D("PID","PID",S1K,0,S2K,S1K,0,S2K);
  g->Reset();
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  printf("Program sorts PID histogram for the CSIARRAY \n");

  memset(hist,0,sizeof(hist));
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));

  if((cluster=fopen(argv[1],"r"))==NULL)
	    {
	      printf("\nI can't open input file %s\n",argv[1]);
	      exit(-2);
	    }
  
  while(fscanf(cluster,"%s",n)!=EOF)
    {
      strcpy(name->fname.inp_data,n);
      sort(name);
    }
  fclose(cluster);



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
  delete canvas;
  canvas = new TCanvas("PID_FS", "PID_FS",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  g->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

