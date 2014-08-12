#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos,type,col,csi;
  double tgr,tcsi,chi;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_GRIFFIN(data,&cal_par->gr,&cev->gr);
  
  if(cev->gr.h.FT>0)
      for(pos=1;pos<NPOSGRIF;pos++)
	if((cev->gr.h.THP&(1<<(pos-1)))!=0)
	  if(cev->gr.det[pos].hge.FT>0)
	    for(col=0;col<NCOL;col++)
	      if((cev->gr.det[pos].hge.THP&(1<<col))!=0)
		if(cev->gr.det[pos].ge[col].h.FT>0)
		  if((cev->gr.det[pos].ge[col].h.THP&1)!=0)
		    {
		      tgr=cev->gr.det[pos].ge[col].seg[0].T/cal_par->gr.contr_t;		   
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
  free(cev);
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
      printf("\n ./GRIFFIN_CSIARRAY_TCalTFitRaw master_file_name idmin idmax chimin chimax\n");
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
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

  read_master(argv[1],name);

  if(name->flag.inp_data!=1)
    {
      printf("\nInput data file not defined\n");
      exit(EXIT_FAILURE);
    }

  if(name->flag.GRIFFIN_cal_par==1)
        {
          printf("\nGRIFFIN calibration read from the file:\n %s\n",name->fname.GRIFFIN_cal_par);
          initialize_GRIFFIN_calibration(&cal_par->gr,name->fname.GRIFFIN_cal_par);
	  
        }
      else
        {
          printf("\nGRIFFIN calibration parameters not defined\n");
          exit(-1);
        }



  sort(name);

 
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("TTCal", "TTCal",10,10, 500, 500);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

