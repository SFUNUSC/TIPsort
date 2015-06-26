#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
 
  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
      {
	fit_CsI_waveform(d,waveform,par,&wpar);

	//print_WavePar(&wpar);
	//print_ShapePar(par);
	//getc(stdin);

	h->Fill(par->chisq/par->ndf);
      }

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  h=NULL;
  char runFile[132];
  
  if(argc!=7)
    {
      printf("wfit_test_CsIFitChiSq midas_input_data channel tRC tF tS tGamma\n");
      exit(-1);
    }

  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));

  printf("Program tests chi square for CsI waveform fits.\n");
  strcpy(runFile,argv[1]);

  chn=atoi(argv[2]);

  par->t[1]=atof(argv[3]); //set tRC
  par->t[2]=atof(argv[4]); //set tF
  par->t[3]=atof(argv[5]); //set tS
  par->t[4]=atof(argv[6]); //set tGamma

  if((h=(TH1D*)gROOT->FindObject("Waveform"))==NULL)	
    h=new TH1D("Waveform","Waveform",16000,-1000,14999);
  else
    h->Reset();

  theApp=new TApplication("App", &argc, argv);
  
  sort_but_not_assemble(runFile);

  c = new TCanvas("Constant Baseline", "Constant Baseline",10,10, 700, 500);
  h->Draw();
  
  theApp->Run(kTRUE);
}