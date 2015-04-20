#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
 
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_S3ring(data,&cal_par->s3ring,&cev->s3);
  calibrate_S3sec(data,&cal_par->s3sec,&cev->s3);
 
  if(cev->s3.rh.HHP==(one<<ring))
    if(cev->s3.sh.HHP==(one<<sec))
      {
	h->Fill(cev->s3.ring[ring].E/cal_par->s3ring.contr_e,cev->s3.sec[sec].E/cal_par->s3sec.contr_e);
	if(cev->s3.ring[ring].E>rl)
	  if(cev->s3.ring[ring].E<rh)
	    if(cev->s3.sec[sec].E>sl)
	      if(cev->s3.sec[sec].E<sh)
		{
		  printf("\nTrigger in gate is %8d\n",data->h.trig_num);
		
		}
	

      }
  free(cev);


  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  TCanvas *canvas;
  TApplication *theApp;
  input_names_type* name;

  if(argc!=8)
    {
      printf("\n ./S3_PixelFold1_EE_Trig master_file_name ring sector ring_e_low ring_e_high sec_e_low sec_e_high\n");
      exit(-1);
    }
  

  printf("Program plots ring energy vs sector energy for pixel fold 1 events in the S3");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));


  
  h = new TH2D("E_r-E_s","E_r-E_s",S4K,0,S8K,S4K,0,S8K);
  h->Reset();

  ring=atoi(argv[2]);
  sec=atoi(argv[3]);
  rl=atoi(argv[4]);
  rh=atoi(argv[5]);
  sl=atoi(argv[6]);
  sh=atoi(argv[7]);

  read_master(argv[1],name);
  if(name->flag.inp_data!=1)
    {
      printf("\nInput data file not defined\n");
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
 if(name->flag.S3sec_cal_par==1)
        {
          printf("\nS3 sectors calibration read from the file:\n %s\n",name->fname.S3sec_cal_par);
          initialize_S3sec_calibration(&cal_par->s3sec,name->fname.S3sec_cal_par
);
	  
        }
      else
        {
          printf("\n S3 sector calibration parameters not defined\n");
          exit(EXIT_FAILURE);
        }

  sort(name);


   
 theApp=new TApplication("App", &argc, argv);
 canvas = new TCanvas("E_r-E_s", "E_r-E_s",10,10, 500, 300);
 gStyle->SetPalette(1);
 h->Draw("COLZ");
  
 theApp->Run(kTRUE);


}

