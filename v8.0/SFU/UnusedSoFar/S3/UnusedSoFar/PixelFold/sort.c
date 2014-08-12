#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int sec,ring;
  double ts,tr;
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

    if(fold>0)
    if(fold<S4K)
       {
       hist[fold]++;
       h->Fill(fold);
       }


    if(fold==disp)
      {
	printf("====== Pixel Fold %2d =======\n",fold);
	printf("Event trigger number %d\n",data->h.trig_num);
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
				printf("Pixel hit at ring %2d sector %2d ringE %10.2f secE %10.2f\n",ring,sec,cev->s3.ring[ring].E,cev->s3.sec[sec].E);
			  }
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

  if(argc!=3)
    {
      printf("\n ./S3_PixelFold master_file_name display_min\n");
      exit(-1);
    }
  disp=atoi(argv[2]);
  h = new TH1D("Pixel Fold","PixelFold",20,0,20);
  h->Reset();

  printf("Program sorts pixel fold histogram for the S3 \n");
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

  if((output=fopen("S3_pixel_fold.spn","w"))==NULL)
    {
      printf("\nI can't open file %s\n","S3_pixel_fold.spn");
      exit(EXIT_FAILURE);
    }
  
    fwrite(hist,S4K*sizeof(int),1,output);

  fclose(output);
   
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Cal. Energy", "Cal. Energy",10,10, 500, 300);
 
  h->Draw();
  
  theApp->Run(kTRUE);

}

