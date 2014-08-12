#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos;
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  if(cev->tg.h.FE>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.EHP&(1<<(pos-1)))!=0)
	if(cev->tg.det[pos].addbackF>0)
	  if(cev->tg.det[pos].suppress==0)
	    h->Fill(pos);
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
      printf("\n ./Tigress_SuppRFAddBackHP master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts BGO suppressed Tigress addback hit pattern \n");

  h = new TH1D("Ge Position HP","Ge Position HP",NPOSTIGR+2,-1,NPOSTIGR+1);
  h->Reset();
  printf("Program sorts histogram of Tigress Ge Position Hit Patter \n");

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

  if(name->flag.TIGRESS_cal_par==1)
        {
          printf("\nTIGRESS calibration read from the file:\n %s\n",name->fname.TIGRESS_cal_par);
          initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
	  
        }
      else
        {
          printf("\nTIGRESS calibration parameters not defined\n");
          exit(EXIT_FAILURE);
        }

 
  sort(name);

 theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Ge Pos HP", "Ge Pos HP",10,10, 500, 300);
  gPad->SetLogy(1);

  h->Draw();
  
  theApp->Run(kTRUE);




}

