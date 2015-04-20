#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
 
 Int_t d;
 ShapePar par;
 WaveFormPar wpar;
 Double_t ch;

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      memcpy(&par,&p,sizeof(ShapePar));
      fit_CsI_waveform(d,waveform,&par,&wpar);
      ch=par.chisq/par.ndf;
      //   printf("ch %10.4f\n",ch);
     h->Fill(ch);
	
    }

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

  TCanvas *c;  
  TApplication *theApp;
  int ac;
  char *av[10];

 if(argc!=7)
    {
      printf("\n ./wfit_lin_chisq midas_input_data_file_name channel trc tf ts tr \n");
      exit(-1);
    }
 chn=atoi(argv[2]);

 p.t[0]=0;
 p.t[1]=atof(argv[3]);
 p.t[2]=atof(argv[4]);
 p.t[3]=atof(argv[5]);
 p.t[4]=atof(argv[6]);
 p.ndf=1;
 p.chisq=BADCHISQ;
 theApp=new TApplication("App", &ac, av);
 h=new TH1D("Chisq","Chisq",10000,0,1000);

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */

  printf("Program shows chisq for  waveform fit\n");
  c = new TCanvas("Chisq", "Chisq",10,10, 700, 500);
  gPad->SetLogy();
  h->GetXaxis()->SetTitle("Chisq");
  h->GetYaxis()->SetTitle("Counts");
  h->Draw();
  theApp->Run(kTRUE);


}
