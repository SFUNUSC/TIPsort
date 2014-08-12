#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  double ePIN=0,eAddBack=0,suppFlag=0;
  int take=0;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_PINARRAY(data,&cal_par->pinarray,&cev->pinarray);

  if((cev->tg.h.FH>0) && (cev->pinarray.h.FH>0))
    for(pin=1;pin<NPIN;pin++)
      if((cev->pinarray.h.HHP&(one<<pin))!=0)
	{
	  ePIN=cev->pinarray.pin[pin].E/cal_par->pinarray.contr_e;;
	  take=0;
	  
	  for(pos=1;pos<NPOSTIGR;pos++)
	    if(((cev->tg.h.HHP&(one<<(pos-1)))!=0) && (cev->tg.det[pos].hge.FE>0) && (take==0))
	      if((cev->tg.h.AHP&(one<<(pos-1)))!=0)
		{
		  eAddBack = cev->tg.det[pos].addback.E/cal_par->tg.contr_e;  
		  suppFlag = cev->tg.det[pos].suppress;
		  col = cev->tg.det[pos].addbackC;
		  col += NCOL*suppFlag;

		  if((eAddBack>cal_par->tg.celow[pos][col]) && (eAddBack<cal_par->tg.cehigh[pos][col]))
		    {
		      take=1; //I don't want to double increment a diode if there were higher gamma folds!
		      h->Fill(ePIN,pin);
		      if((eAddBack>0) && (eAddBack<S32K)) histGamma[pos][col][(int)rint(eAddBack)]++;
		      else histGamma[pos][col][S32K-1000]++;

		      if((ePIN>0) && (ePIN<S32K)) histPIN[pin][(int)rint(ePIN)]++;
		      else histPIN[pin][S32K-1000]++;
		    }}}
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*===============================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  FILE *output,*cluster;
  char DataFile[132],mcaFile[132];
  TCanvas *canvas;
  TApplication *theApp;

  if(argc!=2)
    {
      printf("PINArray_TigECalLimSum master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("PIN_TigECalLimSum","PIN_TigECalLimSum",S8K,0,S32K-8,NPIN+1,0,NPIN);
  h->Reset();
  
  printf("Program sorts PINArray_ECalSum spectra gated on TigECalLim.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));

  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  memset(histPIN,0,sizeof(histPIN));
  memset(histGamma,0,sizeof(histGamma));
  read_master(argv[1],name);
  
  if(name->flag.cluster_file==1)
    {
      printf("Sorting data from %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("ERROR!!! I cannot open %s\n",name->fname.cluster_file);
	  exit(-2);
	}}
  else
    {
      printf("ERROR!!! Cluster file not defined!\n");
      exit(-1);
    }
  
  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calibration read from %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR!!! TIGRESS calibration parameters not defined!\n");
      exit(-1);
    }
  
  if(name->flag.PINARRAY_cal_par==1)
    {
      printf("PINARRAY calibration read from %s\n",name->fname.PINARRAY_cal_par);
      initialize_PINARRAY_calibration(&cal_par->pinarray,name->fname.PINARRAY_cal_par);
    }
  else
    {
      printf("ERROR!!! PINARRAY calibration parameters not defined!\n");
      exit(-1);
    }
  
  while(fscanf(cluster,"%s",DataFile)!=EOF)
    {
      strcpy(name->fname.inp_data,DataFile);
      sort(name);
    }
  fclose(cluster);
  
  if((output=fopen("PINArray_TigECalLimSum.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  for(pin=0;pin<NPIN;pin++) fwrite(histPIN[pin],S32K*sizeof(int),1,output);
  fclose(output);
  
  for(pos=1;pos<NPOSTIGR;pos++)
    {
      sprintf(mcaFile,"pos%1d%1d_ECalABSuppSum.mca",pos/10,pos-(pos/10)*10);
      if((output=fopen(mcaFile,"w"))==NULL)
	{
          printf("ERROR!!! I cannot open the mca file!\n");
          exit(EXIT_FAILURE);
        }
      
      for(col=0;col<NCOL;col++) fwrite(histGamma[pos][col],S32K*sizeof(int),1,output);
      fclose(output);
    }
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("PINArray_ECalSum_TigECalLim", "PINArray_ECalSum_TigECalLim",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  theApp->Run(kTRUE);
}
