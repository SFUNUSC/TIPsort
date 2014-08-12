#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  double ttg,tring;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_S3ring(data,&cal_par->s3ring,&cev->s3);

  if(cev->tg.h.FT>0 && cev->s3.rh.FT>0)
    for(pos=1;pos<NPOS;pos++)
      if((cev->tg.h.THP&(1<<(pos-1)))!=0)
	if(cev->tg.det[pos].hge.FT>0)
	  for(col=0;col<NCOL;col++)
	    if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
	      if(cev->tg.det[pos].ge[col].h.FT>0)
		if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
		  {
		      ttg=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;

		      for(ring=1;ring<NS3RING;ring++)
			if((cev->s3.rh.THP&(one<<ring))!=0)
			  {
			    tring=cev->s3.ring[ring].T/cal_par->s3ring.contr_t;
			    h[ring]->Fill(ttg,tring);
			  }}
  free(cev);
  return SEPARATOR_DISCARD;
}
/*=======================================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *cluster;
  input_names_type* name;
  char title1[64],title2[64],DataFile[256];

  if(argc!=2)
    {
      printf("TigressS3_RingTCalSum master_file_name\n");
      exit(-1);
    }
  
  for(ring=1;ring<NS3RING;ring++)
    {
      sprintf(title1,"TigressS3 Ring%.2d Gated",ring);
      sprintf(title2,"TigressS3 Ring%.2d Gated",ring);
      h[ring] = new TH2D(title1,title2,S4K,0,S4K-1,S4K,0,S4K-1);
      h[ring]->Reset();
    }
  
  printf("Program sorts 2D TCalSum spectra for Tigress (x) and S3 rings (y).\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);
  
  if(name->flag.cluster_file==1)
    {
      printf("Sorting 2D TCalSum spectra for Tigress and S3 rings from: %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("ERROR! I can't open input file %s\n",name->fname.cluster_file);
	  exit(-2);
	}}
  else
    {
      printf("ERROR! Cluster file not defined\n");
      exit(-1);
    }
  
  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("Tigress cal par read from: %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR! TIGRESS calibration parameters not defined\n");
      exit(-1);
    }
  
  if(name->flag.S3ring_cal_par==1)
    {
      printf("S3 ring cal par read from: %s\n",name->fname.S3ring_cal_par);
      initialize_S3ring_calibration(&cal_par->s3ring,name->fname.S3ring_cal_par);
    }
  else
    {
      printf("ERROR! S3 ring calibration parameters not defined\n");
      exit(EXIT_FAILURE);
    }

  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting data from file %s\n",DataFile);
      sort(name);
    }
  
  for(ring=1;ring<NS3RING;ring++)
    {
      sprintf(title1,"TigressS3Ring%.2d_TCal.root",ring);
      TFile f(title1, "new");
      h[ring]->GetXaxis()->SetTitle("Tigress Rel. TCal [ns]");
      h[ring]->GetXaxis()->CenterTitle(true);
      h[ring]->GetYaxis()->SetTitle("S3 Rel. TCal [ns]");
      h[ring]->GetYaxis()->CenterTitle(true);
      h[ring]->GetYaxis()->SetTitleOffset(1.5);
      h[ring]->SetOption("COLZ");
      gStyle->SetPalette(1);

      h[ring]->Write();
    }}

