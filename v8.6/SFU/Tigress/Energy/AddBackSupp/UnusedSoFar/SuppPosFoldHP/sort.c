#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int fold=0;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);

  if(cev->tg.h.FH>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.HHP&(1<<(pos-1)))!=0)
	if((cev->tg.det[pos].addbackF>0) && (cev->tg.det[pos].suppress==1))
	  {
	    fold++;
	    h_pos->Fill(pos);
	  }

  h_fold->Fill(fold);
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*===========================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *c1,*c2;
  TApplication *theApp;

  if(argc!=2)
    {
      printf("Tigress_ABSuppPosFoldHP master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts Tigress position and fold hit patterns for addback AND suppressed events.\n");
  
  h_pos = new TH1D("ABSuppPosHP","ABSuppPosHP",NPOSTIGR+2,-1,NPOSTIGR+1);
  h_pos->Reset();
  h_fold = new TH1D("ABSuppFoldHP","ABSuppFoldHP",NPOSTIGR+2,-1,NPOSTIGR+1);
  h_fold->Reset();

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  read_master(argv[1],name);
  
  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calibration read from %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR!!! TIGRESS calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }

  sort(name);
  
  theApp=new TApplication("App", &argc, argv);
  c1 = new TCanvas("ABSuppPosHP", "ABSuppPosHP",10,10, 500, 300);
  h_pos->Draw();
  c2 = new TCanvas("ABSuppFoldHP", "ABSuppFoldHP",10,10, 500, 300);
  c2->SetLogy();
  h_fold->Draw("");
  theApp->Run(kTRUE);
}
