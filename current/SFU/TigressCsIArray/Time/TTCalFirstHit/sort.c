#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  uint64_t one=1;
  int pos,col,csi;
  double ttg,tcsi,tdiff;
  double ttg1,tcsi1,thit;
  //double etg;
  int type;
  double chisq;
  int ndf;
  bool first_hit;

  if((data->h.setupHP&TIGRESS_BIT)==0)
    return SEPARATOR_DISCARD;

  if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;

 if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);

  //Given any number of TIGRESS detector hits during the event, find the time at which 
  //the first and second hits occur.
  first_hit = true;
  if(cev->tg.h.FT>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.THP&(1<<(pos-1)))!=0)
	if(cev->tg.det[pos].hge.FT>0)
	  for(col=0;col<NCOL;col++)
	    if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
	      if(cev->tg.det[pos].ge[col].h.FT>0)
		if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
		  {
                    thit=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;
                    if (first_hit == true)
                      {
                        first_hit = false;
                        ttg1=thit;
                      }
                    else if (ttg1>thit)
                      {
                        ttg1=thit; //assign the time of the first hit
                      }
                  }

  //Given any number of CsI detector hits during the event, find the time at which 
  //the first and second hits occur.
  first_hit = true;
  if(cev->csiarray.h.FT>0)
    for(pos=1;pos<NCSI;pos++) //look at each CsI position
      if((cev->csiarray.h.THP[pos/64]&(one<<pos%64))!=0) //is there a hit in the detector?
        {
          thit=cev->csiarray.csi[pos].T/cal_par->csiarray.contr_t;
          if (first_hit == true)
            {
              first_hit = false;
              tcsi1=thit;
            }
          else if (tcsi1>thit)
            {
              tcsi1=thit; //assign the time of the first hit
            }
        }

  if(cev->tg.h.FT>0)
    if(cev->csiarray.h.FT>0)
      for(pos=1;pos<NPOSTIGR;pos++)
	if((cev->tg.h.THP&(1<<(pos-1)))!=0)
	  if(cev->tg.det[pos].hge.FT>0)
	    for(col=0;col<NCOL;col++)
	      if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
		if(cev->tg.det[pos].ge[col].h.FT>0)
		  if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
		    {
		      ttg=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;
		      if(ttg==ttg1)//check whether the tigress hit is the first hit
                        {
		        for(csi=1;csi<NCSI;csi++)
			  if((cev->csiarray.h.THP[csi/64]&(one<<csi%64))!=0)
			    {
			      type=cev->csiarray.type[csi];
			      chisq=cev->csiarray.chisq[csi];
			      ndf=cev->csiarray.ndf[csi];
			      chisq/=ndf;
			      if(type==1 || type==2) /* two component or fast only */
			        if(chisq>0 && chisq<10000)
				  {
				    tcsi=cev->csiarray.csi[csi].T/cal_par->csiarray.contr_t;
				    //printf("tcsi %10.3f\n",tcsi);
				    //getc(stdin);
				    if(tcsi==tcsi1)//check whether the CsI hit is the first hit
                                      {
				        tdiff=ttg-tcsi;
				        //tdiff+=S4K;
				        h_tcsi->Fill(tcsi);
				        h_ttg->Fill(ttg);
				        h_tdiff->Fill(tdiff);
				        h->Fill(ttg,tcsi);
                                      }
				  }
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
  char title[132];

  if(argc!=2)
    {
      printf("TigressCsIArray_TTCalFirstHit master_file_name\n");
      printf("Generates a TTCal spectrum with only the first Tigress and CsI hits in a given event.\n");
      exit(-1);
    }
  
  h = new TH2D("TigressCsIArray TCal","TigressCsIArray TCal",S4K,0,S16K,S4K,0,S16K);
  h->Reset();
  h_tcsi = new TH1D("CsI TCal","CsI TCal",S32K,0,S32K-1);
  h_tcsi->Reset();
  h_ttg = new TH1D("Tigress TCal","Tigress TCal",S32K,0,S32K-1);
  h_ttg->Reset();
  h_tdiff = new TH1D("TDiff","TDiff",S16K,-S8K,S8K-1);
  h_tdiff->Reset();

  printf("Program sorts calibrated 2D histogram for TIGRESS/CSIARRAY timing \n");
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

  if(name->flag.TIGRESS_cal_par==1)
        {
          printf("\nTIGRESS calibration read from the file:\n %s\n",name->fname.TIGRESS_cal_par);
          initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
	  
        }
      else
        {
          printf("\nTIGRESS calibration parameters not defined\n");
          exit(-1);
        }

  if(name->flag.CSIARRAY_cal_par==1)
        {
          printf("\nCSIARRAY calibration read from the file:\n %s\n",name->fname.CSIARRAY_cal_par);
          initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
	  
        }
      else
        {
          printf("\nCSIARRAY calibration parameters not defined\n");
          exit(-1);
        }

  sort(name);
  sprintf(title,name->fname.root_output_file);
  //sprintf(title,"TigressCsIArray_TTCalFirstHit.root");
  TFile f(title, "recreate");
  h->GetXaxis()->SetTitle("T_{TIGRESS}-#phi_{RF} [ns]");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("T_{CsI}-#phi_{RF} [ns]");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  gStyle->SetPalette(1);
  
  h->Write();
  h_ttg->Write();
  h_tcsi->Write();
  h_tdiff->Write();
}

