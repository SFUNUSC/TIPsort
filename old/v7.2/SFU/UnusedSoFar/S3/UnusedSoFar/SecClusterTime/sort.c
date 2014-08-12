#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int sec;
  int fold,scfold,ref;
  int i,j;
  double ti,tj,t;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_S3sec(data,&cal_par->s3sec,&cev->s3);
  fold=0;

   if(cev->s3.sh.FH>0)
     {
       for(sec=1;sec<NS3SEC;sec++)
	 if((cev->s3.sh.HHP&(one<<sec))!=0)
	   {
	     fold++;
	     scfold=0;
	     ref=sec;
	     while((cev->s3.sh.HHP&(one<<sec))!=0)
	       {
		 sec++;
		 scfold++;
	       }

	     if(scfold>1)
	       {
		 /* printf("Sector cluster %2d of %2d sectors\n",fold,scfold); */
		 /* printf("Event trigger number %12d\n",data->h.trig_num); */
		 /* printf("Sector hitpattern: %8.8x\n  ",cev->s3.sh.HHP); */
		 /* for(int xsec=NS3SEC-1;xsec>=0;xsec--) */
		 /*   if((cev->s3.sh.HHP&(one<<xsec))==0) */
		 /*     printf("0"); */
		 /*   else */
		 /*     printf("1"); */
		 /* printf("\n"); */
		 for(i=ref;i<ref+scfold;i++)
		   {
		     ti=cev->s3.sec[i].T;
		     for(j=i+1;j<ref+scfold;j++)
		       {
			 tj=cev->s3.sec[j].T;
			 t=ti-tj+S2K;
			 h->Fill(t);
		       }
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
      printf("\n ./S3_SecClusterTime master_file_name\n");
      exit(-1);
    }
 
  h = new TH1D("Sector Cluster Time Difference","Sector Cluster Time Difference",S4K,0,S4K);
  h->Reset();

  printf("Program sorts time differences for multihit sector clusters for the S3 \n");
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


   
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Cluster Fold", "Cluster Fold",10,10, 500, 300);
 
  h->Draw();
  
  theApp->Run(kTRUE);

}

