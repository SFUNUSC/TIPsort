#include "sort.h"
/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  Int_t d;
  WaveFormPar wpar;
  double ch;
  int type;
  
  //manually disable channel 1043... somehow triggers with thresh=1000!!!
  if( (ptr->channel>=chn_low) && (ptr->channel<=chn_high) && (ptr->channel!=1043) )
    {
      //printf("channel number %d\n",ptr->channel);
      if((d=ptr->waveform_length)!=0)
      {
	//printf("d %d\n",d);
	fit_CsI_waveform(d,waveform,par,&wpar);
	ch=par->chisq/par->ndf;
	
	type=par->type;
	//printf("chisq/ndf %10.3f type %d\n",ch,par->type);
	
	if(ch>=chisq_min)
	  if(ch<=chisq_max)
	    {
	      h->Fill(type);
	    }
      }
    }
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  int ac;
  char *av[10];

 if(argc!=10)
    {
      printf("wfit_show_CsIFit_type midas_input_data_file_name channel_low channel_high tRC tF tS tGamma chisq_min chisq_max\n");
      exit(-1);
    }

  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));

  h=new TH1D("CsI fit type","CsI fit type",512,-256,255);
  h->Reset();

  chn_low=atoi(argv[2]);
  chn_high=atoi(argv[3]);
  par->t[1]=atof(argv[4]); //set tRC
  par->t[2]=atof(argv[5]); //set tF
  par->t[3]=atof(argv[6]); //set tS
  par->t[4]=atof(argv[7]); //set tGamma
  chisq_min=atof(argv[8]);
  chisq_max=atof(argv[9]);

/* do sorting */
  sort_but_not_assemble(argv[1]);

 /* display results */
  theApp=new TApplication("App", &ac, av);
  c = new TCanvas("CsI fit type", "CsI fit type",10,10,700,500);
  gPad->SetLogy();
  h->GetXaxis()->SetTitle("Fit type");
  h->GetYaxis()->SetTitle("Counts");
  h->Draw();
  theApp->Run(kTRUE);
}
