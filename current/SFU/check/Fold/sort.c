#include "sort.h"

int analyze_data(raw_event *data)
{
  switch(detectorSubsystem)
    {
    case 1:
      h->Fill(data->csiarray.h.TSfold);
      title="CsIFold";
      if(data->csiarray.h.TSfold<10)
      	foldHist[data->csiarray.h.TSfold]++;
      break;
    case 2:
      h->Fill(data->gr.h.Gefold);
      title="GriffinCloverFold";
      if(data->gr.h.Gefold<10)
      	foldHist[data->gr.h.Gefold]++;
      break;    
    case 3:
      h->Fill(data->tg.h.Gefold);
      title="TigressCloverFold";
      if(data->tg.h.Gefold<10)
      	foldHist[data->tg.h.Gefold]++;
      break;
    case 4:
      h->Fill(data->pinarray.h.TSfold);
      title="PINArrayFold";
      if(data->pinarray.h.TSfold<10)
      	foldHist[data->pinarray.h.TSfold]++;
      break;
    default:
      printf("Not a valid detector subsystem selection!\n");
      printf("Re-execute---- 1 for CsI, 2 for Griffin, 3 for Tigress, 4 for PINArray.\n");
      break;
    }
  return SEPARATOR_DISCARD;
}
/*=======================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;

  if(argc!=3)
    {
      printf("check_Fold SFU_data_file detector_subsystem_number\n");
      printf("Detector subsystem number required as third argument.\n");
      printf("---->1 for CsI, 2 for Griffin, 3 for Tigress, 4 for PINArray.<----\n");      
      exit(-1);
    }
  
  h = new TH1D("FoldHistogram","FoldHistogram",10,0,10);
  h->Reset();
  
  printf("Program sorts Fold histogram.\n");
  
  for(int i=0;i<10;i++)
  	foldHist[i]=0;
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  detectorSubsystem=atoi(argv[2]);
  sort(name); 

  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas(title,title,10,10, 500, 300);
  gPad->SetLogy(1);
  h->Draw();
  theApp->Run(kTRUE);
  
  printf("Fold distribution:\n");
  for(int i=0;i<10;i++)
  	printf("Fold %i events: %li\n",i,foldHist[i]);
}
