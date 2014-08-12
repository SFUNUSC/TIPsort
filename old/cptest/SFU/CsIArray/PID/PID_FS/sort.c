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
	      
	      if(r>S32K-4) r=S32K-4;
	      if(pos==det_id) h->Fill(f,s);
	      
	      hist1[(int)rint(f)]++;
	      hist2[(int)rint(s)]++;
	    }}

  return SEPARATOR_DISCARD;
}
/*=========================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  
  if(argc!=7)
    {
      printf("CsIArray_PID_FS sfu_input_data idmin idmax chimin chimax det_number\n");
      exit(-1);
    }
  
  h = new TH2D("PID F/S","PID F/S",S4K,0,S8K,S4K,0,S8K);
  h->Reset();
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  det_id=atoi(argv[6]);
  
  printf("Program sorts PID_FS histogram for the CsIArray.\n");
  
  memset(hist1,0,sizeof(hist1));
  memset(hist2,0,sizeof(hist2));
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  
  sort(name);
  
  if((output=fopen("CSIArray_PID_fast.mca","w"))==NULL)
    {
      printf("ERROR!!! I can't open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  //for(int pos=0;pos<NCSI;pos++)
  fwrite(hist1,S32K*sizeof(int),1,output);
  fclose(output);
  
  if((output=fopen("CSIArray_PID_slow.mca","w"))==NULL)
    {
      printf("ERROR!!! I can't open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  //for(int pos=0;pos<NCSI;pos++)
  fwrite(hist2,S32K*sizeof(int),1,output);
  fclose(output);
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("FS", "FS",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);
}
