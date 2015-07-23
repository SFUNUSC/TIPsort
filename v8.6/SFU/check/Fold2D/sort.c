#include "sort.h"

int analyze_data(raw_event *data)
{
  if(detectorSubsystemx==1)
    {
      if(detectorSubsystemy==1)
        h->Fill(data->csiarray.h.TSfold,data->csiarray.h.TSfold);
      else if(detectorSubsystemy==2)
        h->Fill(data->csiarray.h.TSfold,data->gr.h.Gefold);
      else if(detectorSubsystemy==3)
        h->Fill(data->csiarray.h.TSfold,data->tg.h.Gefold);
      else if(detectorSubsystemy==4)
        h->Fill(data->csiarray.h.TSfold,data->pinarray.h.TSfold);
    }
  else if(detectorSubsystemx==2)
    {
      if(detectorSubsystemy==1)
        h->Fill(data->gr.h.Gefold,data->csiarray.h.TSfold);
      else if(detectorSubsystemy==2)
        h->Fill(data->gr.h.Gefold,data->gr.h.Gefold);
      else if(detectorSubsystemy==3)
        h->Fill(data->gr.h.Gefold,data->tg.h.Gefold);
      else if(detectorSubsystemy==4)
        h->Fill(data->gr.h.Gefold,data->pinarray.h.TSfold);
    }
  else if(detectorSubsystemx==3)
    {
      if(detectorSubsystemy==1)
        h->Fill(data->tg.h.Gefold,data->csiarray.h.TSfold);
      else if(detectorSubsystemy==2)
        h->Fill(data->tg.h.Gefold,data->gr.h.Gefold);
      else if(detectorSubsystemy==3)
        h->Fill(data->tg.h.Gefold,data->tg.h.Gefold);
      else if(detectorSubsystemy==4)
        h->Fill(data->tg.h.Gefold,data->pinarray.h.TSfold);
    }
  else if(detectorSubsystemx==4)
    {
      if(detectorSubsystemy==1)
        h->Fill(data->pinarray.h.TSfold,data->csiarray.h.TSfold);
      else if(detectorSubsystemy==2)
        h->Fill(data->pinarray.h.TSfold,data->gr.h.Gefold);
      else if(detectorSubsystemy==3)
        h->Fill(data->pinarray.h.TSfold,data->tg.h.Gefold);
      else if(detectorSubsystemy==4)
        h->Fill(data->pinarray.h.TSfold,data->pinarray.h.TSfold);
    }

  if ((detectorSubsystemx<0)||(detectorSubsystemx>4)||(detectorSubsystemy<0)||(detectorSubsystemy>4))
    {
      printf("Not a valid detector subsystem selection!\n");
      printf("Re-execute---- 1 for CsI, 2 for Griffin, 3 for Tigress, 4 for PINArray.\n");
    }

  return SEPARATOR_DISCARD;
}
/*=======================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  char title[132];


  if(argc!=4)
    {
      printf("check_CsIFold2D SFU_data_file detector_subsystem_x detector_subsystem_y\n");
      printf("Detector subsystem number required for 3rd/4th arguments.\n");
      printf("---->1 for CsI, 2 for Griffin, 3 for Tigress, 4 for PINArray.<----\n");      
      exit(-1);
    }
  
  h = new TH2D("FoldHistogram","FoldHistogram",10,0,10,10,0,10);
  h->Reset();
  
  printf("Program sorts 2D fold histogram.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  detectorSubsystemx=atoi(argv[2]);
  detectorSubsystemy=atoi(argv[3]);
  sort(name); 
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas(title,title,10,10, 500, 300);
  if(detectorSubsystemx==1)
    h->GetXaxis()->SetTitle("CsI Fold");
  else if(detectorSubsystemx==2)
    h->GetXaxis()->SetTitle("Griffin Clover Fold");
  else if(detectorSubsystemx==3)
    h->GetXaxis()->SetTitle("Tigress Clover Fold");
  else if(detectorSubsystemx==4)
    h->GetXaxis()->SetTitle("PIN Array Fold");
  else
    h->GetXaxis()->SetTitle("x Fold");
  h->GetXaxis()->CenterTitle(true);
  if(detectorSubsystemy==1)
    h->GetYaxis()->SetTitle("CsI Fold");
  else if(detectorSubsystemy==2)
    h->GetYaxis()->SetTitle("Griffin Clover Fold");
  else if(detectorSubsystemy==3)
    h->GetYaxis()->SetTitle("Tigress Clover Fold");
  else if(detectorSubsystemy==4)
    h->GetYaxis()->SetTitle("PIN Array Fold");
  else
    h->GetYaxis()->SetTitle("y Fold");
  h->GetYaxis()->CenterTitle(true);
  h->SetOption("COLZ");

  h->Draw();
  theApp->Run(kTRUE);
}
