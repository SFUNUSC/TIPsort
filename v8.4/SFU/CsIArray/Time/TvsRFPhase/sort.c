#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double tcsiFITlocal,tcsiFITfunction,tcsiCFD,chi,trf;
  double tdiffFITlocal,tdiffFITfunction,tdiffCFD;
  
  if((data->csiarray.h.Tfold>0))
    if((data->h.setupHP&RF_BIT)!=0)
      for(pos=1;pos<NCSI;pos++)
	if((data->csiarray.h.THP&(one<<pos))!=0)
	  {
	    type=data->csiarray.wfit[pos].type;
	    chi=data->csiarray.wfit[pos].chisq;
	    chi/=data->csiarray.wfit[pos].ndf;
	    
	    if(type>=idmin && type<=idmax)
	      if(chi>=chimin && chi<=chimax)
                {
                  //In ns, fit local
		  tcsiFITlocal=data->csiarray.t0[pos]*10;
		  //In ns, fit function
		  tcsiFITfunction=data->csiarray.wfit[pos].t[0]*10;
		  
		  //In CFD units
                  tcsiCFD=data->csiarray.csi[pos].cfd&0x00ffffff;
                  tcsiCFD-=(data->csiarray.csi[pos].timestamp*16)&0x00ffffff;
                  //In ns
		  tcsiCFD*=0.625; tcsiCFD+=S4K-S1K;
		  
		  //In ns
		  trf=data->rf.sin.t0*0.625;
		  
		  tdiffFITlocal=tcsiFITlocal-trf;
		  tdiffFITfunction=tcsiFITfunction-trf;
		  tdiffCFD=tcsiCFD-trf;
		  
		  h_tcsiFITlocal->Fill(tcsiFITlocal);
		  h_tcsiFITfunction->Fill(tcsiFITfunction);
		  h_tcsiCFD->Fill(tcsiCFD);
		  h_trf->Fill(trf);
		  h_tdiffFITlocal->Fill(tdiffFITlocal);
		  h_tdiffFITfunction->Fill(tdiffFITfunction);
		  h_tdiffCFD->Fill(tdiffCFD);
		  hFITlocal->Fill(trf,tcsiFITlocal);
		  hFITfunction->Fill(trf,tcsiFITfunction);
		  hFITandCFD->Fill(tcsiCFD,tcsiFITfunction);
		  hCFD->Fill(trf,tcsiCFD);
		}}

  return SEPARATOR_DISCARD;
}
/*===============================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  char title[132];
  
  if(argc!=6)
    {
      printf("CsIArray_TvsRFPhase sfu_input_data idmin idmax chimin chimax\n");
      exit(-1);
    }
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  
  printf("Program sorts TvsRFPhase histograms for the CsIArray.\n");
  
  hFITlocal = new TH2D("TvsRFPhase","TvsRFPhase",135,0,134,S4K,0,S4K-1);
  hFITlocal->Reset();
  hFITfunction = new TH2D("TvsRFPhase","TvsRFPhase",135,0,134,S4K,0,S4K-1);
  hFITfunction->Reset();
  hCFD = new TH2D("TvsRFPhase","TvsRFPhase",135,0,134,S1K,0,S1K-1);
  hCFD->Reset();
  hFITandCFD = new TH2D("FITvsCFD","FITvsCFD",S1K,0,S1K-1,S2K,0,S4K-1);
  hFITandCFD->Reset();

  h_tcsiFITlocal = new TH1D("tcsiFITlocal","tcsiFITlocal",S4K,0,S4K-1);
  h_tcsiFITlocal->Reset();
  h_tcsiFITfunction = new TH1D("tcsiFITfunction","tcsiFITfunction",S4K,0,S4K-1);
  h_tcsiFITfunction->Reset();
  h_tcsiCFD = new TH1D("tcsiCFD","tcsiCFD",S1K,0,S1K-1);
  h_tcsiCFD->Reset();

  h_trf = new TH1D("trf","trf",135,0,134);
  h_trf->Reset();

  h_tdiffFITlocal = new TH1D("tdiffFITlocal","tdiffFITlocal",S4K,0,S4K-1);
  h_tdiffFITlocal->Reset();
  h_tdiffFITfunction = new TH1D("tdiffFITfunction","tdiffFITfunction",S4K,0,S4K-1);
  h_tdiffFITfunction->Reset();
  h_tdiffCFD = new TH1D("tdiffCFD","tdiffCFD",S1K,0,S1K-1);
  h_tdiffCFD->Reset();

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);

  sprintf(title,"CsIArray_TvsRFPhase.root");
  TFile f(title, "recreate");

  hFITlocal->GetXaxis()->SetTitle("RF Phase [ns]");
  hFITlocal->GetXaxis()->CenterTitle(true);
  hFITlocal->GetYaxis()->SetTitle("Csi TFitlocal [ns]");
  hFITlocal->GetYaxis()->CenterTitle(true);
  hFITlocal->GetYaxis()->SetTitleOffset(1.5);
  hFITlocal->SetOption("COLZ");
  gStyle->SetPalette(1);

  hFITfunction->GetXaxis()->SetTitle("RF Phase [ns]");
  hFITfunction->GetXaxis()->CenterTitle(true);
  hFITfunction->GetYaxis()->SetTitle("Csi TFitfunction [ns]");
  hFITfunction->GetYaxis()->CenterTitle(true);
  hFITfunction->GetYaxis()->SetTitleOffset(1.5);
  hFITfunction->SetOption("COLZ");
  gStyle->SetPalette(1);
 
  hCFD->GetXaxis()->SetTitle("RF Phase [ns]");
  hCFD->GetXaxis()->CenterTitle(true);
  hCFD->GetYaxis()->SetTitle("Csi TCFD [ns]");
  hCFD->GetYaxis()->CenterTitle(true);
  hCFD->GetYaxis()->SetTitleOffset(1.5);
  hCFD->SetOption("COLZ");
  gStyle->SetPalette(1);

  hFITandCFD->GetXaxis()->SetTitle("Csi TCFD [ns]");
  hFITandCFD->GetXaxis()->CenterTitle(true);
  hFITandCFD->GetYaxis()->SetTitle("Csi TFITfunction [ns]");
  hFITandCFD->GetYaxis()->CenterTitle(true);
  hFITandCFD->GetYaxis()->SetTitleOffset(1.5);
  hFITandCFD->SetOption("COLZ");
  gStyle->SetPalette(1);

  hFITlocal->Write();
  hFITfunction->Write();
  hCFD->Write();
  hFITandCFD->Write();
  h_tcsiFITlocal->Write();
  h_tcsiFITfunction->Write();
  h_tcsiCFD->Write();
  h_trf->Write();
  h_tdiffFITlocal->Write();
  h_tdiffFITfunction->Write();
  h_tdiffCFD->Write();
}
