#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos,col,ring,csi;
  double etig,ecsi;

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

  /* Require good time and energy for the event */
  if(cev->tg.h.FH>0)
    if(cev->csiarray.h.FH>0)
      for(pos=1;pos<NPOSTIGR;pos++)
	if(((cev->tg.h.HHP&(1<<(pos-1)))!=0) && (cev->tg.det[pos].hge.FE>0))
	  if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
	    {	
	      etig = cev->tg.det[pos].addback.E/cal_par->tg.contr_e; /* TIGRESS energy in keV */
	      ring=cev->tg.det[pos].addbackR;

	      if(cev->csiarray.h.FH>0)
		for(csi=1;csi<NCSI;csi++)
		  if((cev->csiarray.h.HHP&(one<<csi))!=0)
		    {
		      ecsi=cev->csiarray.csi[csi].E/cal_par->csiarray.contr_e; /* CsI energy in keV/4 */
		      //printf("etig %f ecsi %f\n",etig,ecsi);
		      h[ring]->Fill(ecsi,etig);
		      ht->Fill(ecsi,etig);
		      //printf("h[%d] pair: pos %2d col %1d ring %1d etig %f csi %2d ecsi %f\n",ring,pos,col,ring,etig,csi,ecsi);
		    }
	    }
  
  //getc(stdin);

  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  char title[132];
  char HistName1[132];

  if(argc!=2)
    {
      printf("TigressCsIArray_TCal master_file_name\n");
      exit(-1);
    }
  
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

  for(int i=0;i<NRING;i++)
    {
      sprintf(HistName1,"ring_%d",i);
      h[i]=new TH2D(HistName1,HistName1,S1K,0,S16K,S1K,0,S4K);
      h[i]->Reset();
     }

  ht=new TH2D("sum","sum",S1K,0,S16K,S1K,0,S4K);
  ht->Reset();
  
  sort(name);

  sprintf(title,"TigressCsIArray_EECal.root");
  TFile f(title, "recreate");

  /* write the histograms */
  for(int i=0;i<NRING;i++)
    {
      if(h[i]->GetEntries())
	{
	  h[i]->GetXaxis()->SetTitle("CsI Energy [keV/4]");
	  h[i]->GetXaxis()->CenterTitle(true);
	  h[i]->GetYaxis()->SetTitle("Corrected TIGRESS Energy [keV]");
	  h[i]->GetYaxis()->CenterTitle(true);
	  h[i]->GetYaxis()->SetTitleOffset(1.5);
	  h[i]->SetOption("COLZ");
	  h[i]->Write();
	}
    }

  ht->GetXaxis()->SetTitle("CsI Energy [keV/4]");
  ht->GetXaxis()->CenterTitle(true);
  ht->GetYaxis()->SetTitle("Corrected TIGRESS Energy [keV]");
  ht->GetYaxis()->CenterTitle(true);
  ht->GetYaxis()->SetTitleOffset(1.5);
  ht->SetOption("COLZ");
  ht->Write();

  return 0;

}

