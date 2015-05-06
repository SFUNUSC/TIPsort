#include "sort.h"
/*=======================================================*/
int choose(int n,int k)
{
  /* a little recursive function to calculate
     the binomial coefficient n choose k.*/ 
  if(k==0)
    return 1;
  if(k>n)
    return 0;

  return (n*choose(n-1,k-1))/k;
}
/*=======================================================*/
int analyze_data(raw_event *data)
{
  switch(detectorSubsystem)
    {
    case 1:
      h->Fill(data->csiarray.h.TSfold);
      title="CsIFold";
      break;
    case 2:
      h->Fill(data->gr.h.Gefold);
      title="GriffinCloverFold";
      break;    
    case 3:
      h->Fill(data->tg.h.Gefold);
      h1->Fill(data->tg.h.Gefold,choose(data->tg.h.Gefold,2));
      title="TigressCloverFold";
      break;
    case 4:
      h->Fill(data->pinarray.h.TSfold);
      title="PINArrayFold";
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
  FILE *cluster;
  char DataFile[132],FileName[132];

  printf("Detector subsystem number required as third argument.\n");
  printf("---->1 for CsI, 2 for Griffin, 3 for Tigress, 4 for PINArray.<----\n");
  printf("Press <enter> to continue...\n");
  getc(stdin);

  if(argc!=3)
    {
      printf("check_FoldSum cluster_file_name detector_subsystem_number\n");
      exit(-1);
    }

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.cluster_file,argv[1]);
  detectorSubsystem=atoi(argv[2]);

  h = new TH1D("Fold histogram","Fold histogram",10,0,10);
  h->Reset();
  h1 = new TH1D("Number of pairs","Number of pairs",10,0,10);
  h1->Reset();

  printf("Program sorts fold histogram.\n");

  printf("Sorting ECalABSuppSum histograms for TIGRESS clovers and cores based upon the cluster file: %s\n",name->fname.cluster_file);

  if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
    {
      printf("ERROR!!! I can't open input file %s\n",name->fname.cluster_file);
      exit(-2);
    }

  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting data from file %s\n", name);
      sort(name);
    }

  sprintf(FileName,"Fold.root");
  TFile f(FileName,"recreate");
  h->Write();
  h1->Write();
}
