#include "sort.h"

int analyze_data(raw_event *data)
{

 
  h->Fill(data->tg.h.Gefold);

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
      printf("\n ./Tigress_GePosFold sfu_input_data_file_name\n");
      exit(-1);
    }
  
  
  h = new TH1D("Ge Position Fold","Ge Position Fold",NPOSTIGR+1,0,NPOSTIGR);
  h->Reset();

  printf("Program sorts histogram of Tigress Ge Position Fold \n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name); 

  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("TrigBits", "TrigBits",10,10, 500, 300);
  gPad->SetLogy(1);
  h->Draw();
  
  theApp->Run(kTRUE);

   
}
