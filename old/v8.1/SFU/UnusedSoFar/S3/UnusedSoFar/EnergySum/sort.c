#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int sec,ring;
  double ts,tr;
  double e,es,er,ene;
 
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_S3sec(data,&cal_par->s3sec,&cev->s3);
  calibrate_S3ring(data,&cal_par->s3ring,&cev->s3);

  es=0;
  if(cev->s3.sh.FH>0)
    for(sec=1;sec<NS3SEC;sec++)
      if((cev->s3.sh.HHP&(one<<sec))!=0)
	{
	  ts=cev->s3.sec[sec].T;
	  if(ts>cal_par->s3sec.tlow[sec])
	    if(ts<cal_par->s3sec.thigh[sec])
	      {
		es+=cev->s3.sec[sec].E;
	      }
	}
  e=es;

  ene=es/cal_par->s3sec.contr_e;
  if(ene>0)
    if(ene<S32K)
      {
	hist[0][(int)rint(ene)]++;
	hs->Fill(ene);
      }


  er=0;
  if(cev->s3.rh.FH>0)
    for(ring=1;ring<NS3RING;ring++)
      if((cev->s3.rh.HHP&(one<<ring))!=0)
	{
	  tr=cev->s3.ring[ring].T;
	  if(tr>cal_par->s3ring.tlow[ring])
	    if(tr<cal_par->s3ring.thigh[ring])
	      {
		er+=cev->s3.ring[ring].E;
	      }
	}
  e+=er;

  ene=er/cal_par->s3ring.contr_e;
  if(ene>0)
    if(ene<S32K)
      {
	hist[1][(int)rint(ene)]++;
	hr->Fill(ene);
      }

  ene=e/(cal_par->s3ring.contr_e+cal_par->s3sec.contr_e);
  if(ene>0)
    if(ene<S32K)
      {
	hist[2][(int)rint(ene)]++;
	hr->Fill(ene);
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
      printf("\n ./S3_EnergySum master_file_name\n");
      exit(-1);
    }
  
  hr = new TH1D("Ring Energy Sum","Ring Energy Sum",S32K,0,S32K);
  hr->Reset();

  hs = new TH1D("Sector Energy Sum","Sector Energy Sum",S32K,0,S32K);
  hs->Reset();

  h = new TH1D("Total Energy Sum","Total Energy Sum",S32K,0,S32K);
  h->Reset();

  printf("Program sorts energy sum histograms for the S3 \n");
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

  if((output=fopen("S3_energy_sum.mca","w"))==NULL)
    {
      printf("\nI can't open file %s\n","S3_energy_sum.mca");
      exit(EXIT_FAILURE);
    }
  
  for(int i=0;i<3;i++)
    fwrite(hist[i],S32K*sizeof(int),1,output);

  fclose(output);
   
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Cal. Energy", "Cal. Energy",10,10, 500, 300);
 
  hr->Draw();
  hs->Draw("same");
  h->Draw("same");
  theApp->Run(kTRUE);

}

