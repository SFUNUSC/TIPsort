#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos;
  double e;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_PINARRAY(data,&cal_par->pinarray,&cev->pinarray);
  if(cev->pinarray.h.FE>0)
    for(pos=1;pos<NPIN;pos++)
      if((cev->pinarray.h.EHP&(one<<pos))!=0)
	{
	  e=cev->pinarray.pin[pos].E;
	  if(e>cal_par->pinarray.elow[pos])
	    if(e<cal_par->pinarray.ehigh[pos])
	      {
		e/=cal_par->pinarray.contr_e;
		h->Fill(e,pos);
		if(e>0)
		  if(e<S4K-1)
		    hist[pos][(int)rint(e)]++;
	      }
	}
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output,*cluster;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  char n[132];

  if(argc!=2)
    {
      printf("\n ./PINARRAY_ECalLimCluster master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("Cal. Energy","Cal. Energy",S4K,0,S4K-1,NPIN+1,0,NPIN);
  h->Reset();

  printf("Program sorts calibrated energy histogram for the PINARRAY \n");
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


  if(name->flag.PINARRAY_cal_par==1)
        {
          printf("\nPINARRAY calibration read from the file:\n %s\n",name->fname.PINARRAY_cal_par);
          initialize_PINARRAY_calibration(&cal_par->pinarray,name->fname.PINARRAY_cal_par);
	  
        }
      else
        {
          printf("\nPINARRAY calibration parameters not defined\n");
          exit(EXIT_FAILURE);
        }


  name->flag.inp_data=1; 
  while(fscanf(cluster,"%s",n)!=EOF)
    {
      strcpy(name->fname.inp_data,n);
      sort(name);
    }
  fclose(cluster);

 
  if((output=fopen("PINArray_cal_ene.spn","w"))==NULL)
    {
      printf("\nI can't open file %s\n","PINArray_cal_ene.spn");
      exit(EXIT_FAILURE);
    }
  for(int pos=0;pos<NPIN;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);

  fclose(output);
 
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Cal. Energy", "Cal. Energy",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

