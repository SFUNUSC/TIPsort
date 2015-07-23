#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int TIGpos,BGOpos,TIGcol,BGOcol,BGOsup;
  double ttg,tbgo,tdiff;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));

  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);

  //set Tigress position == 1 and col == 0 and BGO suppressor == 0
  //this is upstream Tigress pos 1, blue core, blue front suppressor 1
  /* TIGpos=1; */
  /* TIGcol=0; */
  /* BGOpos=15; */
  /* BGOcol=0; */
  /* BGOsup=0; */
  
  //check TIGRESS hit fold
  if(cev->tg.h.FT>0)
    //check BGO hit fold
    if(cev->tg.h.BFT>0)
      //TIGRESS part - loop through positions, cores; check associated hit patterns
      for(TIGpos=1;TIGpos<NPOSTIGR;TIGpos++)
	if((cev->tg.h.THP&(1<<(TIGpos-1)))!=0)
	  if(cev->tg.det[TIGpos].hge.FT>0)
	    for(TIGcol=0;TIGcol<NCOL;TIGcol++)
	      if((cev->tg.det[TIGpos].hge.THP&(1<<TIGcol))!=0)
		if(cev->tg.det[TIGpos].ge[TIGcol].h.FT>0)
		  if((cev->tg.det[TIGpos].ge[TIGcol].h.THP&1)!=0)
		    //BGO part - loop through positions, cores; check associated hit patterns
		    for(BGOpos=1;BGOpos<NPOSTIGR;BGOpos++)
		      if((cev->tg.h.BTHP&(1<<(BGOpos-1)))!=0)
			if(cev->tg.det[BGOpos].hbgo.BFT>0)
			  for(BGOcol=0;BGOcol<NCOL;BGOcol++)
			    if((cev->tg.det[BGOpos].hbgo.BTHP&(1<<BGOcol))!=0)
			      if(cev->tg.det[BGOpos].bgo[BGOcol].h.BFT>0)
				for(BGOsup=0;BGOsup<NSUP;BGOsup++)
				  if((cev->tg.det[BGOpos].bgo[BGOcol].h.BTHP&(1<<BGOsup))!=0)
				    //if(cal_par->tg.sup_map[TIGpos][TIGcol][BGOpos][BGOcol][BGOsup]==1)
				      {
					//get TIGRESS and BGO time from calibrated event file
					ttg=cev->tg.det[TIGpos].ge[TIGcol].seg[0].T/cal_par->tg.contr_t;
					tbgo=cev->tg.det[BGOpos].bgo[BGOcol].sup[BGOsup].T/cal_par->tg.contr_t;
					/* printf("tig pos %d col %d time %.4f\nbgo pos %d col %d sup %d time %.4f\n",TIGpos,TIGcol,ttg,BGOpos,BGOcol,BGOsup,tbgo); */
					/* getc(stdin); */
					
					tdiff=ttg-tbgo;
					tdiff+=S4K;
					//printf("tdiff = ttg-tbgo: %.4f\n",tdiff);
					//getc(stdin);
					
					//fill root histograms
					h_tbgo->Fill(tbgo);
					h_ttg->Fill(ttg);
					h_tdiff->Fill(tdiff);
					h->Fill(ttg,tbgo);
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
      printf("TigressBGO_TTCal master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("TigressBGO TCal","TigressBGO TCal",S4K,0,S16K,S4K,0,S16K);
  h->Reset();
  h_tbgo = new TH1D("BGO TCal","BGO TCal",S32K,0,S32K-1);
  h_tbgo->Reset();
  h_ttg = new TH1D("Tigress TCal","Tigress TCal",S32K,0,S32K-1);
  h_ttg->Reset();
  h_tdiff = new TH1D("TDiff","TDiff",S2K,-S16K,S16K-1);
  h_tdiff->Reset();

  printf("Program sorts calibrated 2D histogram for TIGRESS/BGO \ntiming method specified in the TIGRESS_par.dat file. \n");
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

  sort(name);
  sprintf(title,name->fname.root_output_file);
  //sprintf(title,"TigressBGO_TTCal.root");
  TFile f(title, "recreate");
  h->GetXaxis()->SetTitle("T_{TIGRESS}-#phi_{RF} [ns]");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("T_{BGO}-#phi_{RF} [ns]");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  gStyle->SetPalette(1);
  
  h->Write();
  h_ttg->Write();
  h_tbgo->Write();
  h_tdiff->Write();
}

