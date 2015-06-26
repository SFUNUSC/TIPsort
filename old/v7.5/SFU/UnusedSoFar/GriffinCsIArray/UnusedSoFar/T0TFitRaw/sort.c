#include "sort.h"

int analyze_data(raw_event *data)
{
 
  unsigned long long int one=1;
  int pos,type,col,csi;
  double tgr,tcsi,chi;
  
 
  if((data->h.setupHP&GRIFFIN_BIT)==0)
    return SEPARATOR_DISCARD;

  if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;

  if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;

  for(pos=1;pos<NPOSGRIF;pos++)
    for(col=0;col<NCOL;col++)
      {
	tgr=data->gr.det[pos].ge[col].t0[0];	
	if(tgr>0)
	  {
	    tgr-=data->rf.sin.t0;
	    tgr/=16;
	    tgr+=512;
	    
	    for(csi=1;csi<NCSI;csi++)
	      if((data->csiarray.h.THP&(one<<csi))!=0)
		{
		  type=data->csiarray.wfit[csi].type;
		  chi=data->csiarray.wfit[csi].chisq;
		  chi/=data->csiarray.wfit[csi].ndf;
		  if(type>=idmin)
		    if(type<=idmax)
		      if(chi>=chimin)
			if(chi<=chimax)
			  {
			    tcsi=16.*data->csiarray.wfit[csi].t[0]; 
			    if((data->h.setupHP&RF_BIT)!=0)
			      {
				tcsi-=data->rf.sin.t0;
				
				tcsi+=S4K;
				tcsi/=16;
				if(tcsi<0)tcsi=S32K-2;
				if(tcsi>S32K) tcsi=S32K-3;
			      }
			    else
			      tcsi=S32K-4;
			    h->Fill(tgr,tcsi);
			  } 
		}
	    }
      }
  
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;

  if(argc!=6)
    {
      printf("\n ./GRIFFIN_CSIARRAY_T0TFitRaw input_file_name idmin idmax chimin chimax\n");
      exit(-1);
    }
  
  h = new TH2D("Griffin Csiarray Time","Griffin Csiarray Time",S1K,0,S1K,S1K,0,S1K);
  h->Reset();
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);

  printf("Program sorts calibrated 2D histogram for GRIFFIN/CSIARRAY timing \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);

  sort(name);

 
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("T0TFitRaw", "T0TFitRaw",10,10, 700, 700);
  // gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}
