#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int type;
  double chi,s,f,r,e;

  if((data->h.setupHP&RF_BIT)==0) return SEPARATOR_DISCARD;
  if((data->h.setupHP&CsIArray_BIT)==0) return SEPARATOR_DISCARD;
  
  for(pos=1;pos<NCSI;pos++)
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
	      
	      if(r>S32K-4) r=S32K-4000;
	      h[pos]->Fill(f,s);
	      
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
  char HistName[132];
  
  if(argc!=6)
    {
      printf("CsIArray_PID_FS master_file_name idmin idmax chimin chimax\n");
      exit(-1);
    }
  
  for(pos=1;pos<NCSI;pos++)
    {
      sprintf(HistName,"PID_FS_%02d",pos);
      h[pos] = new TH2D(HistName,HistName,S1K,0,S8K,S1K,0,S8K);
      h[pos]->Reset();
    }
  
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
    
  printf("Program sorts PID_FS histogram for the CsIArray.\n");
  
  memset(hist1,0,sizeof(hist1));
  memset(hist2,0,sizeof(hist2));
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  read_master(argv[1],name);

  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined!\n");
      exit(EXIT_FAILURE);
    }
    
  sort(name);
  
  if((output=fopen("CsIArray_PID_fast.mca","w"))==NULL)
    {
      printf("ERROR!!! I can't open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(hist1,S32K*sizeof(int),1,output);
  fclose(output);
  
  if((output=fopen("CsIArray_PID_slow.mca","w"))==NULL)
    {
      printf("ERROR!!! I can't open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(hist2,S32K*sizeof(int),1,output);
  fclose(output);
  
  //theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("FS", "FS",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  
  TFile *f= new TFile(name->fname.root_output_file,"RECREATE");
  f->cd();
  for(pos=1;pos<NCSI;pos++)
    {
      sprintf(HistName, "PID_FS_%02d", pos);
      h[pos]->Draw("COLZ");
      h[pos]->Write(HistName);
    }
  f->Close();
  //theApp->Run(kTRUE);
}
