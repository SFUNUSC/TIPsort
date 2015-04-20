#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos;
  double e;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_PINARRAY(data,&cal_par->pinarray,&cev->pinarray);
  if(cev->pinarray.h.FE>0)
    for(pos=1;pos<NPIN;pos++)
      if((cev->pinarray.h.EHP&(one<<pos))!=0)
	{
	  e=cev->pinarray.pin[pos].E/cal_par->pinarray.contr_e;

	  if((e<0) || (e>S32K)) e=S32K-4000;
	  hist[pos][(int)rint(e)]++;
	  h->Fill(e,pos);
	}
  free(cev);
  return SEPARATOR_DISCARD;
}
/*========================================================================*/
int main(int argc, char *argv[])
{
  FILE * output, *cluster;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  char DataFileName[132];
  
  if(argc!=2)
    {
      printf("PINArray_ECalSum master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("ECalSum","ECalSum",S8K,0,S32K-8,NPIN+1,0,NPIN);
  h->Reset();

  printf("Program sorts ECalSum histograms for the PINArray.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  memset(hist,0,sizeof(hist));
  
  read_master(argv[1],name);

  if(name->flag.cluster_file==1)
    {
      printf("Sorting data from %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("ERROR!!! I cannot open the cluster file!\n");
	  exit(-2);
	}}
  else
    {
      printf("ERROR!!! Cluster file not defined!\n");
      exit(-1);
    }

  if(name->flag.PINARRAY_cal_par==1)
    {
      printf("PINArray calibration read from %s\n",name->fname.PINARRAY_cal_par);
      initialize_PINARRAY_calibration(&cal_par->pinarray,name->fname.PINARRAY_cal_par);
    }
  else
    {
      printf("ERROR!!! PINArray calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  while(fscanf(cluster,"%s",DataFileName)!=EOF)
    {
      strcpy(name->fname.inp_data,DataFileName);
      sort(name);
    }
  fclose(cluster);
  
  if((output=fopen("PINArray_ECalSum.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }

  for(int pos=0;pos<NPIN;pos++) fwrite(hist[pos],S32K*sizeof(int),1,output);
  
  fclose(output);
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("ECalSum", "ECalSum",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);
}
