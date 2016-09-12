#include "sort.h"

int analyze_data(raw_event *data)
{
  double tFITlocal,tCFD,trf;//,chi;
  double tdiffFITlocal,tdiffCFD;
  double func;
  long int a;

  if((data->h.setupHP&RF_BIT)!=0)

    if(data->tg.h.Gefold>0)
      for(pos=1;pos<NPOSTIGR;pos++)
	if((data->tg.h.GeHP&(1<<(pos-1)))!=0)
	  if(data->tg.det[pos].h.Gefold>0)
	    for(col=0;col<NCOL;col++)
	      if((data->tg.det[pos].h.GeHP&(1<<col))!=0)
		if(data->tg.det[pos].ge[col].h.Tfold>0)
		  if((data->tg.det[pos].ge[col].h.THP&1)!=0)
		    {
		      tCFD=data->tg.det[pos].ge[col].seg[0].cfd&0x00ffffff;
		      tCFD-=(data->tg.det[pos].ge[col].seg[0].timestamp*16)&0x00ffffff;
		      
		      /* chi=data->tg.det[pos].ge[col].t0[0].chisq; */
		      /* chi/=data->tg.det[pos].ge[col].t0[0].ndf; */
		      
		      /* if(chi>=chimin && chi<=chimax) */
		      /* { */
			  //In ns, fit local
			  tFITlocal=data->tg.det[pos].ge[col].t0[0]*10;
			  
			  //In CFD units
			  tCFD=data->tg.det[pos].ge[col].seg[0].cfd&0x00ffffff;
			  tCFD-=(data->tg.det[pos].ge[col].seg[0].timestamp*16)&0x00ffffff;
			  
			  //RF in CFD units
			  trf=data->rf.sin.t0;
			  
			  //func=(1.000*trf)-(RFphase+offset);
			  //a=fmod(tFITlocal+shift,RFphase);
			  //printf("trf %f RFphase %f tFITlocal %f offset %f func %f shift %f a %ld\n",trf,RFphase,tFITlocal,offset,func,shift,a);
			  //if(a>func) 
			  //trf+=RFphase;
			  //printf("new trf %f\n",trf);
			  //getc(stdin);
			  
			  //RF in ns
			  trf*=0.625;
			  
			  //Subtract RF and convert to ns
			  tFITlocal*=0.625;
			  tdiffFITlocal=tFITlocal-trf;
			  
			  //Subtract RF and convert to ns
			  tCFD*=0.625;
			  tdiffCFD=tCFD-trf;
			  tCFD+=S4K+S1K;			  
			  
			  h_tFITlocal->Fill(tFITlocal);
			  h_tCFD->Fill(tCFD);
			  h_trf->Fill(trf);
			  h_tdiffFITlocal->Fill(tdiffFITlocal);
			  h_tdiffCFD->Fill(tdiffCFD);
			  hFITlocal->Fill(trf,tFITlocal);
			  hCFD->Fill(trf,tCFD);
		    }
/* } */
  
  return SEPARATOR_DISCARD;
}
/*===============================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  char title[132];
  
  if(argc!=4)
    {
      printf("Tigress_TvsRFPhase sfu_input_data offset shift\n");
      exit(-1);
    }
  //chimin=atof(argv[2]);
  //chimax=atof(argv[3]);
  offset=atof(argv[2]);
  shift=atof(argv[3]);
  
  printf("Program sorts TvsRFPhase histograms for Tigress.\n");
  
  hFITlocal = new TH2D("TvsRFPhase Fit","TvsRFPhase Fit",135,0,134,S4K,0,S4K-1);
  hFITlocal->Reset();
  hCFD = new TH2D("TvsRFPhase CFD","TvsRFPhase CFD",135,0,134,S4K,0,S4K-1);
  hCFD->Reset();

  h_tFITlocal = new TH1D("tFITlocal","tFITlocal",S4K,0,S4K-1);
  h_tFITlocal->Reset();
  h_tCFD = new TH1D("tCFD","tCFD",S2K,0,S4K-1);
  h_tCFD->Reset();

  h_trf = new TH1D("trf","trf",135,0,134);
  h_trf->Reset();

  h_tdiffFITlocal = new TH1D("tdiffFITlocal","tdiffFITlocal",S4K,0,S4K-1);
  h_tdiffFITlocal->Reset();
  h_tdiffCFD = new TH1D("tdiffCFD","tdiffCFD",S2K,0,S4K-1);
  h_tdiffCFD->Reset();
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);
  
  sprintf(title,"Tigress_TvsRFPhase.root");
  TFile f(title, "recreate");
  
  hFITlocal->GetXaxis()->SetTitle("RF Phase [ns]");
  hFITlocal->GetXaxis()->CenterTitle(true);
  hFITlocal->GetYaxis()->SetTitle("Tigress TFitlocal [ns]");
  hFITlocal->GetYaxis()->CenterTitle(true);
  hFITlocal->GetYaxis()->SetTitleOffset(1.5);
  hFITlocal->SetOption("COLZ");
  gStyle->SetPalette(1);
 
  hCFD->GetXaxis()->SetTitle("RF Phase [ns]");
  hCFD->GetXaxis()->CenterTitle(true);
  hCFD->GetYaxis()->SetTitle("Tigress TCFD [ns]");
  hCFD->GetYaxis()->CenterTitle(true);
  hCFD->GetYaxis()->SetTitleOffset(1.5);
  hCFD->SetOption("COLZ");
  gStyle->SetPalette(1);
  
  hFITlocal->Write();
  hCFD->Write();
  h_tFITlocal->Write();
  h_tCFD->Write();
  h_trf->Write();
  h_tdiffFITlocal->Write();
  h_tdiffCFD->Write();
}
