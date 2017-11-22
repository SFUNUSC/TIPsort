#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  uint64_t one=1;
  int type;
  double chi,s,f,r,t;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  
  if((data->h.setupHP&RF_BIT)==0) return SEPARATOR_DISCARD;
  if((data->h.setupHP&CsIArray_BIT)==0) return SEPARATOR_DISCARD;

  if(cev->csiarray.h.FT>0)
    for(pos=1;pos<NCSI;pos++)
      if((cev->csiarray.h.THP[pos/64]&(one<<pos%64))!=0)
	{
	  type=data->csiarray.wfit[pos].type;
	  chi=data->csiarray.wfit[pos].chisq;
	  chi/=data->csiarray.wfit[pos].ndf;
	  
	  if(type==1)
	    if((chi>=chimin) && (chi<=chimax))
	      {
		t=cev->csiarray.csi[pos].T/cal_par->csiarray.contr_t;
			      
		if(t<0)t=S32K-2000;
		if(t>S32K) t=S32K-3000;
		
		s=data->csiarray.wfit[pos].am[3];
		f=data->csiarray.wfit[pos].am[2];
		r=s/f*100;
		r+=100;
		
		h[pos]->Fill(t,r);

		if(r>S32K-4) r=S32K-4;
		if(r<0) r=S32K-2;
		
		hist1[pos][(int)rint(r)]++;
		hist2[pos][(int)rint(t)]++;
	      }}
  
  return SEPARATOR_DISCARD;
}
/*==========================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  TCanvas *canvas;
  //TApplication *theApp;
  char HistName[132];
  
  if(argc!=4)
    {
      printf("CsIArray_PID_TR master_file_name chimin chimax\n");
      exit(-1);
    }

  for(pos=1;pos<NCSI;pos++)
    {
      sprintf(HistName,"PID_TR_%02d",pos);
      //h[pos] = new TH2D(HistName,HistName,512,0,S4K,S1K/2,0,S1K/2);
      h[pos] = new TH2D(HistName,HistName,S2K,0,S16K,S4K/2,0,S4K/2);
      h[pos]->Reset();
    }
  
  chimin=atof(argv[2]);
  chimax=atof(argv[3]);
  
  printf("Program sorts PID histogram for the CsIArray \n");

  memset(hist1,0,sizeof(hist1));
  memset(hist2,0,sizeof(hist2));
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

  read_master(argv[1],name);

  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined!\n");
      exit(EXIT_FAILURE);
    }

  if(name->flag.CSIARRAY_cal_par==1)
    {
      printf("CsIArray calpar read from: %s\n",name->fname.CSIARRAY_cal_par);
      initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
    }
  else
    {
      printf("ERROR! CsIArray calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }

  sort(name);
  
  if((output=fopen("CsIArray_PID_Ratio.mca","w"))==NULL)
    {
      printf("ERROR! I can't open the mca file!\n"); 
      exit(EXIT_FAILURE);
    }

  for(int i=0;i<NCSI;i++)
    fwrite(hist1[i],S32K*sizeof(int),1,output);

  fclose(output);

  if((output=fopen("CsIArray_PID_Time.mca","w"))==NULL)
    {
      printf("ERROR! I can't open the mca file!\n"); 
      exit(EXIT_FAILURE);
    }
 
  for(int i=0;i<NCSI;i++)
    fwrite(hist2[i],S32K*sizeof(int),1,output);
  
  fclose(output);

  //theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("PID_TR", "PID_TR",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  
  TFile *f= new TFile(name->fname.root_output_file,"RECREATE");
  f->cd();
  for(pos=1;pos<NCSI;pos++)
    {
      sprintf(HistName, "PID_TR_%02d", pos);
      h[pos]->GetXaxis()->SetTitle("CsI TCal [ns]");
      h[pos]->GetYaxis()->SetTitle("PID");
      h[pos]->Draw("COLZ");
      h[pos]->Write(HistName);
    }
  f->Close();
  
  //theApp->Run(kTRUE);
}
