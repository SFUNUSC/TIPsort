#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int sec;
  unsigned long long int one=1;
  double e,ch;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_S3ring(data,&cal_par->s3ring,&cev->s3);
  calibrate_S3sec(data,&cal_par->s3sec,&cev->s3);
 
  if(cev->s3.rh.HHP==(one<<ring))
    {
      e=cev->s3.ring[ring].E;
      if(data->s3.sh.Efold==1)
	for(sec=1;sec<NS3SEC;sec++)
	  if(data->s3.sh.EHP==(one<<sec))
	    {
	      ch=data->s3.sec[sec].charge;
	      s0[sec]+=1;
	      s1[sec]+=ch;
	      s2[sec]+=ch*ch;
	      v0[sec]+=e;
	      v1[sec]+=e*ch;
	      v2[sec]+=e*e;
	      if(sec==15)
		h->Fill(cev->s3.ring[ring].E/cal_par->s3ring.contr_e,cev->s3.sec[sec].E/cal_par->s3sec.contr_e);
	      //	      printf("%10.5f %10.5f\n", cev->s3.ring[ring].E, cev->s3.sec[sec].E);
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
  int sec;
  double a,b,c;

  if(argc!=3)
    {
      printf("\n ./S3_GainMatchSectors master_file_name reference_ring\n");
      exit(-1);
    }
  

  printf("Program gain matches sectors to a reference calibrated ringS3 events \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

  memset(s0,0,sizeof(s0));
  memset(s1,0,sizeof(s1));
  memset(s2,0,sizeof(s2));
  memset(v0,0,sizeof(v0));
  memset(v1,0,sizeof(v1));
  memset(v2,0,sizeof(v2));
  
  h = new TH2D("E/E","E/E",S4K,0,S16K,S4K,0,S16K);
  h->Reset();

  ring=atoi(argv[2]);
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

  for(sec=1;sec<NS3SEC;sec++)
    {

      c=s0[sec]*s2[sec]-s1[sec]*s1[sec];
      a=v1[sec]*s0[sec]-v0[sec]*s1[sec];
      a/=c;
      b=v0[sec]*s2[sec]-s1[sec]*v1[sec];
      b/=c;
      c=v2[sec]-b*v0[sec]-a*v1[sec];
      c/=(s0[sec]-2);
      printf("%2d %8.5f %8.5f %8.5f\n",sec,b,a,c);
    }


   
 theApp=new TApplication("App", &argc, argv);
 canvas = new TCanvas("E/E", "E/E",10,10, 500, 300);
 gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);


}

