#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
 
  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
      {
	wpar.baseline_range=CSI_BASELINE_RANGE;
	get_baseline(d,waveform,&wpar); 
	//printf("baseline = %.2f\n",wpar.baseline);
	
	if(h!=NULL) delete h;
	h=new TH1D("Waveform","Waveform",ptr->waveform_length,0,ptr->waveform_length);

	if(f!=NULL) delete f;
	f=new TF1("base","[0]",0,wpar.baseline_range);

	if(c!=NULL) delete c;
	c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);
	
	for(Int_t i=0;i<ptr->waveform_length;i++)
	  h->Fill(i,waveform[i]);
	
	h->Draw();
	f->SetParameter(0,wpar.baseline);
	f->SetLineColor(kRed);
	f->SetLineWidth(2);
	f->Draw("same");
	theApp->Run(kTRUE);
      }
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  char runFile[132];
  
  if(argc!=3)
    {
      printf("wfit_test_CsIBaseline midas_input_data channel\n");
      exit(-1);
    }

  printf("Program tests baseline fitting for CsI waveforms.\n");
  strcpy(runFile,argv[1]);

  chn=atoi(argv[2]);

  theApp=new TApplication("App", &argc, argv);

  sort_but_not_assemble(runFile);
}
