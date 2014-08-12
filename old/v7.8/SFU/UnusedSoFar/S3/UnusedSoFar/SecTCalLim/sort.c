#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int sec;
  double t,time;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_S3sec(data,&cal_par->s3sec,&cev->s3);
  if(cev->s3.sh.FT>0)
    for(sec=1;sec<NS3SEC;sec++)
      if((cev->s3.sh.THP&(one<<sec))!=0)
	{
	  t=cev->s3.sec[sec].T;
	  if(t>cal_par->s3sec.tlow[sec])
	    if(t<cal_par->s3sec.thigh[sec])
	      {
		time=t/cal_par->s3sec.contr_t;
		hist[sec][(int)rint(time)]++;
		h->Fill(time,sec);
	      }
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
      printf("\n ./S3_SecTCalLim master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("Cal. Time","Cal. Time",S32K,0,S32K-1,NS3SEC+1,0,NS3SEC);
  h->Reset();

  printf("Program sorts calibrated time histogram for the S3 sectors \n");
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



  sort(name);

  if((output=fopen("S3sec_cal_time.mca","w"))==NULL)
    {
      printf("\nI can't open file %s\n","S3sec_cal_time.mca");
      exit(EXIT_FAILURE);
    }
  for(int sec=0;sec<NS3SEC;sec++)
    fwrite(hist[sec],S32K*sizeof(int),1,output);

  fclose(output);
 
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Cal Time", "Cal Time",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

