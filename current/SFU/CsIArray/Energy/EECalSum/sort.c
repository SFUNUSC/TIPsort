#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  uint64_t one=1;
  int pos1,pos2;
  double e1,e2;
  int take1,take2;

  take1=0;
  take2=0;

  e1=0.;
  e2=0.;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);

  //printf("CsIArray FE = %d\n",cev->csiarray.h.FE);
  //getc(stdin);

  if(cev->csiarray.h.FE>0)
    for(pos1=1;pos1<NCSI;pos1++)
      {
	if((cev->csiarray.h.EHP[pos1/64]&(one<<pos1%64))!=0)
	  if(take1==0)
	  {
	    e1=cev->csiarray.csi[pos1].E/cal_par->csiarray.contr_e;
	    take1=1;
	    //printf("pos1 = %d e1 = %f\n",pos1,e1);
	    
	    for(pos2=pos1+1;pos2<NCSI;pos2++)
	      if((cev->csiarray.h.EHP[pos2/64]&(one<<pos2%64))!=0)
		if(take2==0)
		{
		  e2=cev->csiarray.csi[pos2].E/cal_par->csiarray.contr_e;
		  take2=1;
		  //printf("pos2 = %d e2 = %f\n",pos2,e2);
		}

	    /* printf("CsIArray e1 = %f e2= %f\n",e1,e2); */
	    /* printf("CsIArray e1+e2 = %f\n",e1+e2); */
	    /* getc(stdin); */
	    
	    if(e1>=0 && e1<S32K)
	      if(e2>=0 && e2<S32K)
		{
		  h_e1e2->Fill(e1,e2);
		  //symmetrize
		  h_e1e2->Fill(e2,e1);
		  h_e1->Fill(e1);
		  h_e2->Fill(e2);
		  h_esum->Fill(e1+e2);
		}
	    
	    if(e1<0 || e1>S32K)
	      if(e2<0 || e2>S32K)
					h_e1e2->Fill(S32K-1000,S32K-1000);
	    
	    if(e1<0 || e1>S32K)
	      h_e1->Fill(S32K-1000);
	    
	    if(e2<0 || e2>S32K)
	      h_e2->Fill(S32K-1000);
	  }
      }
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * cluster;
  char n[132];
  char title[132];
  input_names_type* name;

  if(argc!=2)
    {
      printf("CsIArray_ECal master_file_name\n");
      exit(-1);
    }
  
  h_e1e2 = new TH2D("CsIArray EECal","CsIArray EECal",S1K,0,S32K,S1K,0,S32K);
  h_e1e2->Reset();
  h_e1 = new TH1D("CsI E1Cal","CsI E1Cal",S4K,0,S32K-1);
  h_e1->Reset();
  h_e2 = new TH1D("CsI E2Cal","Tigress E2Cal",S4K,0,S32K-1);
  h_e2->Reset();
  h_esum = new TH1D("CsI ESum","Tigress ESum",S4K,0,S32K-1);
  h_esum->Reset();
  
  printf("Program sorts 2D EECal histograms for the CsIArray.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);

if(name->flag.cluster_file==1)
    {
      printf("\nSorting data from cluster file:\n %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("\nI can't open input file %s\n",name->fname.cluster_file);
	  exit(-2);
	}
    }
  else
    {
      printf("\nCluster file not defined\n");
      exit(-1);
    }

  if(name->flag.CSIARRAY_cal_par==1)
    {
      printf("CsIArray calpar read from: %s\n",name->fname.CSIARRAY_cal_par);
      initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
    }
  else
    {
      printf("ERROR! CsIArray calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  name->flag.inp_data=1; 
  while(fscanf(cluster,"%s",n)!=EOF)
    {
      strcpy(name->fname.inp_data,n);
      sort(name);
    }

  fclose(cluster);

  sprintf(title,"CsIArray_EECal.root");
  TFile f(title, "recreate");
  h_e1e2->GetXaxis()->SetTitle("E1 [keV]");
  h_e1e2->GetXaxis()->CenterTitle(true);
  h_e1e2->GetYaxis()->SetTitle("E2 [keV]");
  h_e1e2->GetYaxis()->CenterTitle(true);
  h_e1e2->GetYaxis()->SetTitleOffset(1.5);
  h_e1e2->SetOption("COLZ");
  gStyle->SetPalette(1);
  
  h_e1e2->Write();
  h_e1->Write();
  h_e2->Write();
  h_esum->Write();

}
