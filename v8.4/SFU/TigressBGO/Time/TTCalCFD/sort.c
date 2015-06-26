#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int TIGpos,BGOpos,TIGcol,BGOcol,BGOsup;
  double ttg,tbgo,tdiff;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));

  //calibration removed since no BGO calibration right now
  /* calibrate_TIGRESS(data,&cal_par->tg,&cev->tg); */
  /* calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray); */

  //set Tigress position == 1 and col == 0 and BGO suppressor == 0
  //this is upstream Tigress pos 1, blue core, blue front suppressor 1
  /* TIGpos=1; */
  /* TIGcol=0; */
  /* BGOpos=15; */
  /* BGOcol=0; */
  /* BGOsup=0; */

  //Check Ge fold
  if(data->tg.h.Gefold>0)
    //Run through all TIGRESS positions
    for(TIGpos=1;TIGpos<NPOSTIGR;TIGpos++)
      //Check if the position is indicated in the hit pattern
      if((data->tg.h.GeHP&(1<<(TIGpos-1)))!=0)
	//Check if the position has a fold great than zero
	if(data->tg.det[TIGpos].h.Gefold>0)
	  //Run through four cores for each TIGRESS position
	  for(TIGcol=0;TIGcol<NCOL;TIGcol++)
	    //Check if this position is indicated in the hit pattern
	    if((data->tg.det[TIGpos].h.GeHP&(1<<TIGcol))!=0)
	      //Check that this combination has a time fold great than zero
	      if(data->tg.det[TIGpos].ge[TIGcol].h.Tfold>0)
		//Check if this combination is indicated in the time hit pattern
		if((data->tg.det[TIGpos].ge[TIGcol].h.THP&1)!=0)
		  //Check BGO fold
		  if(data->tg.h.BGOfold>0)
		    //Run through all BGO positions
		    for(BGOpos=1;BGOpos<NPOSTIGR;BGOpos++)
		      //check if this position is in the hit pattern with fold > 0
		      if(((data->tg.h.BGOHP&(1<<(BGOpos-1)))!=0) && (data->tg.det[BGOpos].h.BGOfold>0))
			//run through all four colors for each BGO position
			for(BGOcol=0;BGOcol<NCOL;BGOcol++)
			  //check if this color is in the hit pattern with a fold > 0
			  if(((data->tg.det[BGOpos].h.BGOHP&(1<<BGOcol))!=0) && (data->tg.det[BGOpos].bgo[BGOcol].h.Tfold>0))
			    //run through all BGO suppressors for each BGO position and color
			    for(BGOsup=0;BGOsup<NSUP;BGOsup++)
			      //check if this suppressor is in the hit pattern with fold > 0
			      if(((data->tg.det[BGOpos].bgo[BGOcol].h.THP&(1<<BGOsup))!=0))// && (take==0))
				{
				  //timestamp and RF phase are subtracted in order to fit nicely onto a plot
				  //tigress timing from cfd
				  ttg=data->tg.det[TIGpos].ge[TIGcol].seg[0].cfd&0x00ffffff;
				  //printf("tigress cfd: %0.f\n",ttg);
				  ttg-=(data->tg.det[TIGpos].ge[TIGcol].seg[0].timestamp*16)&0x00ffffff;
				  //printf("tigress cfd-timestamp: %0.f\n",ttg);
				  ttg*=0.625;
				  
				  //bgo timing from cfd
				  tbgo=data->tg.det[BGOpos].bgo[BGOcol].sup[BGOsup].cfd&0x00ffffff;
				  //printf("bgo cfd: %0.f\n",tbgo);
				  tbgo-=(data->tg.det[BGOpos].bgo[BGOcol].sup[BGOsup].timestamp*16)&0x00ffffff;
				  //printf("bgo cfd-timestamp: %0.f\n",tbgo);
				  tbgo*=0.625;
				  
				  if((data->h.setupHP&RF_BIT)!=0)
				  {
				  //subtract RF phase from bgo time and tigress time
				  tbgo-=(int)data->rf.sin.t0;
				  ttg-=(int)data->rf.sin.t0;
				  
				  //printf("rf phase: %d\n",(int)data->rf.sin.t0);
				  //printf("bgo cfd-timestamp-RF: %0.f\n",tbgo);
				  //printf("tigress cfd-timestamp-RF: %0.f\n",ttg);
				  
				  //add offset
				  tbgo+=S8K;
				  ttg+=S8K;
				  
				  tdiff=ttg-tbgo;
				  tdiff+=S4K;
				  //printf("tdiff = ttg-tbgo: %0.f\n",tdiff);
				  //getc(stdin);
				  
				  h_tbgo->Fill(tbgo);
				  h_ttg->Fill(ttg);
				  h_tdiff->Fill(tdiff);
				  h->Fill(ttg,tbgo);
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
      printf("TigressBGO_TCal master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("TigressBGO TCal","TigressBGO TCal",S4K,0,S16K,S4K,0,S16K);
  h->Reset();
  h_tbgo = new TH1D("BGO TCal","BGO TCal",S32K,0,S32K-1);
  h_tbgo->Reset();
  h_ttg = new TH1D("Tigress TCal","Tigress TCal",S32K,0,S32K-1);
  h_ttg->Reset();
  h_tdiff = new TH1D("TDiff","TDiff",S1K,-S8K,S8K-1);
  h_tdiff->Reset();

  printf("Program sorts uncalibrated 2D histogram for TIGRESS/BGO timing using the time from the CFD \n");
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

  //OLD BGO STUFF - CAN REPLACE WITH BGO CALIBRATOR LATER
  /********************************************************************
if(name->flag.BGOARRAY_cal_par==1)
        {
          printf("\nBGOARRAY calibration read from the file:\n %s\n",name->fname.BGOARRAY_cal_par);
          initialize_BGOARRAY_calibration(&cal_par->bgoarray,name->fname.BGOARRAY_cal_par);
	  
        }
      else
        {
          printf("\nBGOARRAY calibration parameters not defined\n");
          exit(-1);
	  }
  **********************************************************************/

  sort(name);

  sprintf(title,"TigressBGO_TTCal.root");
  TFile f(title, "recreate");
  h->GetXaxis()->SetTitle("T_{TIGRESS}-timestamp [ns]");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("T_{BGO}-timestamp [ns]");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  gStyle->SetPalette(1);
  
  h->Write();
  h_ttg->Write();
  h_tbgo->Write();
  h_tdiff->Write();
}

