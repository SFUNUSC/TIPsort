#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  uint64_t one=1;
  int pos,col,ring,csi;
  double etig,etig_corr,ecsi;

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
	      etig = cev->tg.det[pos].addback.E; /* TIGRESS energy in keV */
	      col = cev->tg.det[pos].addbackC;
	      ring=cev->tg.det[pos].addbackR;

		for(csi=1;csi<NCSI;csi++)
		  if((cev->csiarray.h.HHP[csi/64]&(one<<csi%64))!=0)
		    {
		      ecsi=cev->csiarray.csi[csi].E/1000.; /* CsI energy in MeV */
		    
		      //printf("etig %f ecsi %f\n",etig,ecsi);
		      
		      /* corrected "first order" Doppler energy */
		      etig_corr=etig-A*ecsi;

		      /* CsI ring 1 */
		      if(csi>=1 && csi<=8)
			{
			  h_csi1[ring]->Fill(ecsi,etig_corr);
			  //printf("hcsi1[%d] pair: pos %2d col %1d ring %1d etig %f csi %2d ecsi %f\n",ring,pos,col,ring,etig,csi,ecsi);
			  
			}
		      /* CsI ring 2 */
		      else if(csi>=9 && csi<=24)
			{
			  h_csi2[ring]->Fill(ecsi,etig_corr);
			  //printf("hcsi2[%d] pair: pos %2d col %1d ring %1d etig %f csi %2d ecsi %f\n",ring,pos,col,ring,etig,csi,ecsi);
			}
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
  FILE *cluster;
  char title[132];
  char HistName1[132],HistName2[132];
  char DataFile[132];

  if(argc!=3)
    {
      printf("TigressCsIArray_EE_PlungerSum master_file_name A\n");
      exit(-1);
    }
  
  printf("Program sorts calibrated 2D histogram for TIGRESS/CSIARRAY timing \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

  read_master(argv[1],name);
  A=atof(argv[2]);

if(name->flag.cluster_file==1)
    {
      printf("Sorting ECalABSuppRingSum histograms for TIGRESS clovers and cores based upon the cluster file: %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("ERROR!!! I can't open input file %s\n",name->fname.cluster_file);
	  exit(-1);
	}
    }
 else
   {
     printf("ERROR!!! Cluster file not defined\n");
     exit(-1);
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
      /* CsI ring 1 */
      sprintf(HistName1,"CsI_1_TIGRESS_%d",i);
      h_csi1[i]=new TH2D(HistName1,HistName1,128,0,128,S2K,0,S4K);
      h_csi1[i]->Reset();
      /* CsI ring 2 */
      sprintf(HistName2,"CsI_2_TIGRESS_%d",i);
      h_csi2[i]=new TH2D(HistName2,HistName2,128,0,128,S2K,0,S4K);
      h_csi2[i]->Reset();
    }
  
  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      printf("Sorting data from file %s\n", name);
      sort(name);
    }

  fclose(cluster);

  sprintf(title,"EE_PlungerSum.root");
  TFile f(title, "recreate");

  /* write the histograms */
  for(int i=0;i<NRING;i++)
    {
      if(h_csi1[i]->GetEntries())
	{
	  h_csi1[i]->GetXaxis()->SetTitle("CsI Energy [MeV]");
	  h_csi1[i]->GetXaxis()->CenterTitle(true);
	  h_csi1[i]->GetYaxis()->SetTitle("Corrected TIGRESS Energy [keV]");
	  h_csi1[i]->GetYaxis()->CenterTitle(true);
	  h_csi1[i]->GetYaxis()->SetTitleOffset(1.5);
	  h_csi1[i]->SetOption("COLZ");
	  h_csi1[i]->Write();
	}
    }
 for(int i=0;i<NRING;i++)
    {
      if(h_csi2[i]->GetEntries())
	{
	  h_csi2[i]->GetXaxis()->SetTitle("CsI Energy [MeV]");
	  h_csi2[i]->GetXaxis()->CenterTitle(true);
	  h_csi2[i]->GetYaxis()->SetTitle("Corrected TIGRESS Energy [keV]");
	  h_csi2[i]->GetYaxis()->CenterTitle(true);
	  h_csi2[i]->GetYaxis()->SetTitleOffset(1.5);
	  h_csi2[i]->SetOption("COLZ");
	  h_csi2[i]->Write();
	}
    }

  return 0;

}

