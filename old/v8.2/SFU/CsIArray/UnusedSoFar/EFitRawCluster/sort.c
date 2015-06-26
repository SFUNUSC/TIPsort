#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double e,chi;


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

		  hist[pos][(int)rint(e)]++;
		  h->Fill(e,pos);
		  
		}
      }
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE *cluster, *output; 
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  char n[132];


  if(argc!=6)
    {
      printf("\n ./CSIARRAY_EFitRaw cluster_file_name idmin idmax chimin chimax\n");
      exit(-1);
    }
  h = new TH2D("Energy","Energy",S32K,0,S32K-1,NCSI+1,0,NCSI);
  h->Reset();
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  printf("Program sorts raw energy histogram for the CSIARRAY \n");

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

 

  if((output=fopen("CSIArray_raw_fit_energy.mca","w"))==NULL)
    {
      printf("\nI can't open file %s\n","CSIArray_raw_fit_energy.mca");
      exit(EXIT_FAILURE);
    }
  for(int pos=0;pos<NCSI;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);

  fclose(output);
 theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Raw Energy", "Raw Energy",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}
