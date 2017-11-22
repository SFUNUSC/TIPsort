#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  uint64_t one=1;
  int pos,type;
  double chi,eCal,s,f,r;
  
  if((data->h.setupHP&RF_BIT)==0) return SEPARATOR_DISCARD;
  if((data->h.setupHP&CsIArray_BIT)==0) return SEPARATOR_DISCARD;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  
 if(cev->csiarray.h.FE>0)
  for(pos=1;pos<NCSI;pos++)
    if((data->csiarray.h.THP[pos/64]&(one<<pos%64))!=0)
      if((cev->csiarray.h.EHP[pos/64]&(one<<pos%64))!=0)
	{
	  {
	    type=data->csiarray.wfit[pos].type;
	    chi=data->csiarray.wfit[pos].chisq;
	    chi/=data->csiarray.wfit[pos].ndf;

	    if(type>=idmin && type<=idmax)
	      if(chi>=chimin && chi<=chimax)
		{
		  eCal=cev->csiarray.csi[pos].E/cal_par->csiarray.contr_e;
		  s=data->csiarray.wfit[pos].am[3];
		  f=data->csiarray.wfit[pos].am[2];
		  r=100+s/f*100;
		  
		  h[pos]->Fill(eCal,r);
		  h_e[pos]->Fill(eCal);
		  
		  //hist1[(int)rint(eCal)]++;
		  //hist2[(int)rint(eFit)]++;
		}
	  }
	}

 free(cev);
 return SEPARATOR_DISCARD;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  char HistName1[132],HistName2[132];

  if(argc!=6)
    {
      printf("CsIArray_PID_vs_ECal master_file_name idmin idmax chimin chimax\n");
      exit(-1);
    }
  
  for(pos=1;pos<NCSI;pos++)
    {
      sprintf(HistName1,"PID_ER_%02d",pos);
      h[pos] = new TH2D(HistName1,HistName1,S8K,0,S32K,S2K,0,S1K);
      h[pos]->Reset();
      sprintf(HistName2,"PID_E_%02d",pos);
      h_e[pos] = new TH1D(HistName2,HistName2,S8K,0,S32K);
      h_e[pos]->Reset();
    }
  
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);

  printf("Program sorts PID_ER histogram for the CsIArray \n");
  
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

  if((output=fopen("CsIArray_PID_ECal.mca","w"))==NULL)
    {
      printf("ERROR!!! I can't open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(hist1,S32K*sizeof(int),1,output);
  fclose(output);

  if((output=fopen("CsIArray_PID_EFit.mca","w"))==NULL)
    {
      printf("ERROR!!! I can't open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(hist2,S32K*sizeof(int),1,output);
  fclose(output);
  
  //theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("PID_ER", "PID_ER",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);

  TFile *f= new TFile(name->fname.root_output_file,"RECREATE");
  f->cd();
  for(pos=1;pos<NCSI;pos++)
    {
      sprintf(HistName1, "PID_ER_%02d", pos);
      sprintf(HistName2, "PID_E_%02d", pos);
      h[pos]->Draw("COLZ");
      h[pos]->Write(HistName1);
      h_e[pos]->Draw("");
      h_e[pos]->Write(HistName2);
    }
  f->Close();
  //theApp->Run(kTRUE); 
}
