#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double chi,s,f,r,e;

  pos=det_id;
  //for(pos=1;pos<NCSI;pos++)
  if((data->csiarray.h.THP&(one<<pos))!=0)
    {
      type=data->csiarray.wfit[pos].type;
      chi=data->csiarray.wfit[pos].chisq;
      chi/=data->csiarray.wfit[pos].ndf;
      
      if(type>=idmin && type<=idmax)
	if(chi>=chimin && chi<=chimax)
	  {
	    e=data->csiarray.wfit[pos].am[1];
	    s=data->csiarray.wfit[pos].am[3];
	    f=data->csiarray.wfit[pos].am[2];
	    r=s/f*100;
	    
	    h->Fill(e,100+r);
	    
	    if(r>S32K-4) r=S32K-4;
	    
	    hist1[(int)rint(r)]++;
	    hist2[(int)rint(e)]++;
	  }}
  
  return SEPARATOR_DISCARD;
}
/*=================================================================*/
int main(int argc, char *argv[])
{
  FILE  *cluster,*output;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  char DataFile[132];
  
  if(argc!=7)
    {
      printf("CsIArray_PID_ER_Sum cluster_file idmin idmax chimin chimax det_number\n");
      exit(-1);
    }
  h = new TH2D("PID","PID",S1K,0,S2K,S1K,0,S2K);
  h->Reset();

  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  det_id=atoi(argv[6]);
  
  printf("Program sorts CsIArray_PID_ER_Sum.\n");
  
  memset(hist1,0,sizeof(hist1));
  memset(hist2,0,sizeof(hist2));

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  if((cluster=fopen(argv[1],"r"))==NULL)
    {
      printf("ERROR!!! I cannot open the cluster file!\n");
      exit(-2);
    }
  
  while(fscanf(cluster,"%s",DataFile)!=EOF)
    {
      printf("Reading data from file %s.\n",DataFile);
      strcpy(name->fname.inp_data,DataFile);
      sort(name);
    }
  
  fclose(cluster);

  if((output=fopen("CSIArray_PID_ratio.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  //for(int pos=0;pos<NCSI;pos++)
  fwrite(hist1,S32K*sizeof(int),1,output);
  fclose(output);

  if((output=fopen("CSIArray_PID_energy.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  //for(int pos=0;pos<NCSI;pos++)
  fwrite(hist2,S32K*sizeof(int),1,output);
  fclose(output);

  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("PID_ER", "PID_ER",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);
}
