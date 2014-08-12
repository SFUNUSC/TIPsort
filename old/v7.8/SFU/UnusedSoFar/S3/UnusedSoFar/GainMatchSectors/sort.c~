#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int sec,ring;
  double ts,tr,es,er;
  int fold;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_S3sec(data,&cal_par->s3sec,&cev->s3);
  calibrate_S3ring(data,&cal_par->s3ring,&cev->s3);

  fold=0;
   if(cev->s3.sh.FH>0)
    for(sec=1;sec<NS3SEC;sec++)
      if((cev->s3.sh.HHP&(one<<sec))!=0)
   	{
	  ts=cev->s3.sec[sec].T;
	  if(ts>cal_par->s3sec.tlow[sec])
	    if(ts<cal_par->s3sec.thigh[sec])
	      if(cev->s3.rh.FH>0)
		for(ring=1;ring<NS3RING;ring++)
		  if((cev->s3.rh.HHP&(one<<ring))!=0)
		    {
		      tr=cev->s3.ring[ring].T;
		      if(tr>cal_par->s3ring.tlow[ring])
		      	if(tr<cal_par->s3ring.thigh[ring])
			  fold++;
		    }
   	}

 
    if(fold==1)
      {
	if(cev->s3.sh.FH>0)
	  for(sec=1;sec<NS3SEC;sec++)
	    if((cev->s3.sh.HHP&(one<<sec))!=0)
	      {
		ts=cev->s3.sec[sec].T;
		if(ts>cal_par->s3sec.tlow[sec])
		  if(ts<cal_par->s3sec.thigh[sec])
		    if(cev->s3.rh.FH>0)
		      for(ring=1;ring<NS3RING;ring++)
			if((cev->s3.rh.HHP&(one<<ring))!=0)
			  {
			    tr=cev->s3.ring[ring].T;
			    if(tr>cal_par->s3ring.tlow[ring])
			      if(tr<cal_par->s3ring.thigh[ring])
				{
				  er=cev->s3.ring[ring].E/cal_par->s3ring.contr_e;
				  h->Fill(er,ring);
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
  TCanvas *canvas;
  TApplication *theApp;

  if(argc!=2)
    {
      printf("\n ./S3_Pixel_Fold1 master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("Energy vs. Ring","Energy vs Ring 1",S16K,0,S16K,NS3RING,0,NS3RING);
  h->Reset();

  printf("Program sorts  histogram for pixel fold 1 S3 events \n");
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

  if(name->flag.S3sec_cal_par==1)
        {
          printf("\nS3 sectors calibration read from the file:\n %s\n",name->fname.S3sec_cal_par);
          initialize_S3sec_calibration(&cal_par->s3sec,name->fname.S3sec_cal_par);
	  
        }
      else
        {
          printf("\n S3 sector calibration parameters not defined\n");
          exit(EXIT_FAILURE);
        }

  if(name->flag.S3ring_cal_par==1)
        {
          printf("\nS3 rings calibration read from the file:\n %s\n",name->fname.S3ring_cal_par);
          initialize_S3ring_calibration(&cal_par->s3ring,name->fname.S3ring_cal_par);
	  
        }
      else
        {
          printf("\n S3 ring calibration parameters not defined\n");
          exit(EXIT_FAILURE);
        }

  
  sort(name);


   
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Pixel Fold 1", "Pixel Fold 1",10,10, 500, 300);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

