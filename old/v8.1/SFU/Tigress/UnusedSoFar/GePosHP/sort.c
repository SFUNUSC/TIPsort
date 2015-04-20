#include "sort.h"

int analyze_data(raw_event *data)
{

  for(int id=0;id<NPOSTIGR-1;id++)
    if((data->tg.h.GeHP&(1<<id))!=0)
      h->Fill(id+1);

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
      printf("\n ./Tigress_GePosHP sfu_input_data_file_name\n");
      exit(-1);
    }
  
  
  h = new TH1D("Ge Position HP","Ge Position HP",NPOSTIGR+2,-1,NPOSTIGR+1);
  h->Reset();
  printf("Program sorts histogram of Tigress Ge Position Hit Patter \n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name); 

  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Ge Pos HP", "Ge Pos HP",10,10, 500, 300);
  gPad->SetLogy(1);

  h->Draw();
  
  theApp->Run(kTRUE);

   
}
