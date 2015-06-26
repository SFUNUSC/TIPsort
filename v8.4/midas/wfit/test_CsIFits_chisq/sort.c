#include "sort.h"
/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
  int trigger;
  int type;

  trigger=ptr->trigger_num&0x0fffffff;
  
  /* if(trigger<=counter) */
  /* { */
    if( (ptr->channel) >= chn_low)  
      if( (ptr->channel) <= chn_high)
	if((d=ptr->waveform_length)!=0)
	  {
	    fit_CsI_waveform(d,waveform,par,&wpar);
	    /* print_WavePar(&wpar); */
	    /* print_ShapePar(par); */
	    /* getc(stdin); */
	    type=par->type;
	    /* printf("type %d\n",type); */
	    /* getc(stdin); */
	    if(type>=type_low)
	      if(type<=type_high)
		{
		  h->Fill(par->chisq/par->ndf);
		}
	  }
  /* } */
  /* else */
  /*   { */
  /*     //exit(1); */
  /*     return(1); */
  /*   } */
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  h=NULL;
  char runFile[132];
  char title[132];
  
  if(argc!=10)
    {
      printf("wfit_test_CsIFitChiSq midas_input_data channel_low channel_high tRC tF tS tGamma type_low type_high\n");
      exit(-1);
    }

  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));

  printf("Program tests chi square for CsI waveform fits.\n");
  strcpy(runFile,argv[1]);

  chn_low=atoi(argv[2]);
  chn_high=atoi(argv[3]);

  par->t[1]=atof(argv[4]); //set tRC
  par->t[2]=atof(argv[5]); //set tF
  par->t[3]=atof(argv[6]); //set tS
  par->t[4]=atof(argv[7]); //set tGamma
  /* counter=atoi(argv[8]); */
  type_low=atoi(argv[8]);
  type_high=atoi(argv[9]);

  h=new TH1D("Waveform","Waveform",1000000,-10,1000000-10);
  h->Reset();
  
  sort_but_not_assemble(runFile);
  
  sprintf(title,"fit_chisq.root");
  TFile f(title, "recreate");
  
  /* save results */
  if(h->GetEntries())
    {
      h->GetXaxis()->SetTitle("chisq");
      h->GetXaxis()->CenterTitle(true);
      h->GetYaxis()->SetTitle("Counts");
      h->GetYaxis()->CenterTitle(true);
      h->GetYaxis()->SetTitleOffset(1.5);
      h->SetOption("COLZ");
      h->Write();
    }
}
