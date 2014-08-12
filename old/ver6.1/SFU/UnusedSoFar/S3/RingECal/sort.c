#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int ring;
  double ene;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_S3ring(data,&cal_par->s3ring,&cev->s3);

   if(cev->s3.rh.FE>0)
    for(ring=1;ring<NS3RING;ring++)
      if((cev->s3.rh.EHP&(one<<ring))!=0)
   	{
   	  ene=cev->s3.ring[ring].E/cal_par->s3ring.contr_e;
   	  if(ene>0)
   	    if(ene<S32K)
   	      hist[ring][(int)rint(ene)]++;
   	  h->Fill(ene,ring);
   	}
  free(cev);


  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;

  if(argc!=2)
    {
      printf("\n ./S3_RingECal master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("Cal. Energy","Cal. Energy",S32K,0,S32K-1,NS3RING+1,0,NS3RING);
  h->Reset();

  printf("Program sorts calibrated ring energy histogram for the S3 \n");
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

  if(name->flag.S3ring_cal_par==1)
        {
          printf("\nS3 ring calibration read from the file:\n %s\n",name->fname.S3ring_cal_par);
          initialize_S3ring_calibration(&cal_par->s3ring,name->fname.S3ring_cal_par);
	  
        }
      else
        {
          printf("\n S3 ring calibration parameters not defined\n");
          exit(EXIT_FAILURE);
        }


  //summarize_S3ring_calibration(&cal_par->s3ring,"s3ring.out");
  sort(name);

  if((output=fopen("S3_ring_cal_energy.mca","w"))==NULL)
    {
      printf("\nI can't open file %s\n","S3_ring_cal_energy.mca");
      exit(EXIT_FAILURE);
    }
  for(int ring=0;ring<NS3RING;ring++)
    fwrite(hist[ring],S32K*sizeof(int),1,output);

  fclose(output);
   
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Cal. Energy", "Cal. Energy",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

