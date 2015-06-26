#include "sort.h"

int analyze_data(raw_event *data)
{
 
  unsigned long long int one=1;
  int pos,col,csi;
  double tgr,tcsi;
  double ecsi,egr;
  cal_event* cev;
  
  if((data->h.setupHP&GRIFFIN_BIT)==0)
    return SEPARATOR_DISCARD;

  if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;

 if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;


  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_GRIFFIN(data,&cal_par->gr,&cev->gr);
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);

  if(cev->csiarray.h.FE>0)
    for(csi=1;csi<NCSI;csi++)
      if((cev->csiarray.h.EHP&(one<<csi))!=0)
	{
	  ecsi=cev->csiarray.csi[csi].E;
	  tcsi=16*data->csiarray.wfit[csi].t[0];
	  if(ecsi>cal_par->csiarray.elow[csi])
	    if(ecsi<cal_par->csiarray.ehigh[csi])
	      if(cev->gr.h.FE>0)
		for(pos=1;pos<NPOSGRIF;pos++)
		  if((cev->gr.h.EHP&(one<<(pos-1)))!=0)
		    if(cev->gr.det[pos].hge.FE>0)
		      for(col=0;col<NCOL;col++)
			if((cev->gr.det[pos].hge.EHP&(one<<col))!=0)
			  if(cev->gr.det[pos].ge[col].h.FE>0)
			    if((cev->gr.det[pos].ge[col].h.EHP&1)!=0)
			      {
				egr=cev->gr.det[pos].ge[col].seg[0].E;
				tgr=16*data->gr.det[pos].ge[col].t0[0];
				if(egr>cal_par->gr.celow[pos][col])
				  if(egr<cal_par->gr.cehigh[pos][col])
				    {
				      tgr-=data->rf.sin.t0;
				      tgr/=16;
				      tgr+=S1K/2;

				      tcsi-=data->rf.sin.t0;
				      tcsi/=16;
				      tcsi+=S1K/2;

				      h->Fill(tgr,tcsi);

				      
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

  if(argc!=2)
    {
      printf("\n ./GRIFFIN_CSIARRAY_T0TFitRaw master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("Griffin Csiarray T0/RF TFit/RF","Griffin Csiarray T0/RF TFit/RF",S1K,0,S1K,S1K,0,S1K);
  h->Reset();
 

  printf("Program sorts calibrated 1D histogram for GRIFFIN/CSIARRAY T0 TFit difference \n");
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
          exit(EXIT_FAILURE);
        }


  if(name->flag.CSIARRAY_cal_par==1)
        {
          printf("\nCSIARRAY calibration read from the file:\n %s\n",name->fname.CSIARRAY_cal_par);
          initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
	  
        }
      else
        {
          printf("\nCSIARRAY calibration parameters not defined\n");
          exit(EXIT_FAILURE);
        }



  sort(name);

 

  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("T0TFitRF", "T0TFitRF",10,10, 700, 700);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}
