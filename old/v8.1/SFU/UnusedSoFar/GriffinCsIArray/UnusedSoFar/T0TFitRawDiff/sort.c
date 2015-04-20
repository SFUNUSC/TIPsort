#include "sort.h"

int analyze_data(raw_event *data)
{
 
  unsigned long long int one=1;
  int pos,type,col,csi;
  double tgr,tcsi,chi;
  
 
  if(data->gr.h.Gefold>0)
    for(pos=1;pos<NPOSGRIF;pos++)
      if((data->gr.h.GeHP&(1<<(pos-1)))!=0)
	if(data->gr.det[pos].h.Gefold>0)
	  for(col=0;col<NCOL;col++)
	    if((data->gr.det[pos].h.GeHP&(1<<col))!=0)
	      if(data->gr.det[pos].ge[col].h.Tfold>0)
		if((data->gr.det[pos].ge[col].h.THP&1)!=0)
		    {
		      tgr=data->gr.det[pos].ge[col].t0[0];	
		      //   printf("tgr %f\n",tgr);
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
				      tcsi=data->csiarray.wfit[csi].t[0]; 
				      //    printf("tcsi %f\n",tcsi-tgr+512);
				      h->Fill(tcsi-tgr+512);
				     
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
  
  h = new TH1D("Griffin Csiarray Time","Griffin Csiarray Time",S1K,0,S1K);
  h->Reset();
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);

  printf("Program sorts calibrated 1D histogram for GRIFFIN/CSIARRAY time differences \n");
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
  h->Draw("");
  
  theApp->Run(kTRUE);

}

