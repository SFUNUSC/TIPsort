#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos,col,pin;
  double ttg,tpin,etg,epin;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_PINARRAY(data,&cal_par->pinarray,&cev->pinarray);

  if(cev->tg.h.FH>0)
    if(cev->pinarray.h.FH>0)
      for(pos=1;pos<NPOS;pos++)
	if((cev->tg.h.HHP&(1<<(pos-1)))!=0)
	  if(cev->tg.det[pos].hge.FH>0)
	    for(col=0;col<NCOL;col++)
	      if((cev->tg.det[pos].hge.HHP&(1<<col))!=0)
		if(cev->tg.det[pos].ge[col].h.FH>0)
		  if((cev->tg.det[pos].ge[col].h.HHP&1)!=0)
		    {
		      ttg=cev->tg.det[pos].ge[col].seg[0].T;
		      etg=cev->tg.det[pos].ge[col].seg[0].E;
		      if(etg>cal_par->tg.celow[pos][col])
			if(etg<cal_par->tg.cehigh[pos][col])
			  if(ttg>cal_par->tg.ctlow[pos][col])
			    if(ttg<cal_par->tg.cthigh[pos][col])
			      for(pin=1;pin<NPIN;pin++)
				if((cev->pinarray.h.HHP&(one<<pin))!=0)
				  {
				    tpin=cev->pinarray.pin[pin].T;
				    epin=cev->pinarray.pin[pin].E;
				    if(epin>cal_par->pinarray.elow[pin])
				      if(epin<cal_par->pinarray.ehigh[pin])
					if(tpin>cal_par->pinarray.tlow[pin])
					  if(tpin<cal_par->pinarray.thigh[pin])
					    {
					      ttg/=cal_par->tg.contr_t;
					      tpin/=cal_par->pinarray.contr_t;
					      h->Fill(ttg,tpin);
					    }
				  }
		    }
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE *cluster;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  char n[132];

  if(argc!=2)
    {
      printf("\n ./Tigress_PINARRAY_TTCalLimCluster  master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("Tigress Pinarray Time","Tigress Pinarray Time",S4K,0,S4K-1,S4K,0,S4K-1);
  h->Reset();

  printf("Program sorts calibrated 2D histogram for TIGRESS/PINARRAY timing \n");
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


  if(name->flag.TIGRESS_cal_par==1)
        {
          printf("\nTIGRESS calibration read from the file:\n %s\n",name->fname.TIGRESS_cal_par);
          initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
	  
        }
      else
        {
          printf("\nTIGRESS calibration parameters not defined\n");
          exit(-1);
        }

  if(name->flag.PINARRAY_cal_par==1)
        {
          printf("\nPINARRAY calibration read from the file:\n %s\n",name->fname.PINARRAY_cal_par);
          initialize_PINARRAY_calibration(&cal_par->pinarray,name->fname.PINARRAY_cal_par);
	  
        }
      else
        {
          printf("\nPINARRAY calibration parameters not defined\n");
          exit(-1);
        }

   name->flag.inp_data=1; 
   while(fscanf(cluster,"%s",n)!=EOF)
     {
       strcpy(name->fname.inp_data,n);
       sort(name);
     }
   fclose(cluster);

  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Cal. Time-Energy", "Cal. Time-Energy",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

