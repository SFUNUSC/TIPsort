#include "sort.h"
/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
 
  Int_t d;
  WaveFormPar wpar;
  double e,t;

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      //    printf("charge %d\n",ptr->charge);
      memset(&wpar,0,sizeof(WaveFormPar));
      wpar.baseline_range=TIFFIN_BASELINE_RANGE;
      get_sig2noise(d,waveform,&wpar);
      get_t10(d,waveform,&wpar);
      get_t90(d,waveform,&wpar);
      if(wpar.t10_flag==1)
	if(wpar.t90_flag==1)
	  {

	    t=wpar.t90-wpar.t10;
	    e=ptr->charge/100+a*t;
	    h->Fill(e);
	    r->Fill(t);
	    if(e>=0 && e<S32K) hist1[(int)rint(e)]++; 
	    er->Fill(e,t);

	  }
    }

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  FILE* output;
  int ac;
  char *av[10],name[132];
  TCanvas *c0,*c,*cr;
  TApplication *theApp;

  
  if(argc!=4)
    {
      printf("\n ./wfit_TIFFIN_E2, midas_input_data_file_name, channel a. \n This program creates an energy spectrum and a 2D plot of energy against risetime. \n");
      exit(-1);
    }
  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));
  memset(hist1,0,sizeof(hist1));

  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));
 
  chn=atoi(argv[2]);
  a=atof(argv[3]);

  h=new TH1D("Energy","Energy;Energy (DAQ Units);Counts",S4K,0,S8K);
  h->Reset();  
  h->GetYaxis()->CenterTitle();
  h->GetYaxis()->SetTitleOffset(1.25);
  h->GetXaxis()->CenterTitle();
  h->GetXaxis()->SetTitleOffset(1.25);


  r=new TH1D("Risetime","Risetime;Risetime (samples);Counts",S1K,0,S1K);
  r->Reset();  
  r->GetYaxis()->CenterTitle();
  r->GetYaxis()->SetTitleOffset(1.25);
  r->GetXaxis()->CenterTitle();
  r->GetXaxis()->SetTitleOffset(1.25);

  er=new TH2D("Energy against Risetime","E/T;Energy (DAQ Units); Risetime (10 ns Samples)",S4K,0,S8K,S1K,0,S1K);
  er->Reset();
  er->GetYaxis()->CenterTitle();
  er->GetYaxis()->SetTitleOffset(1.25);
  er->GetXaxis()->CenterTitle();
  er->GetXaxis()->SetTitleOffset(1.25);
 		
  /* do sorting */
  sort_but_not_assemble(argv[1]);  //current/midas/state-machine/sort_but_not_assemble.c calls analyze_fragment (above)

  /* display results */
  theApp=new TApplication("App", &ac, av);
 
  c0 = new TCanvas("Energy","Energy",10,10, 700, 500);
  c0->cd();
  h->Draw();

 
  cr = new TCanvas("Risetime","Risetime",10,10, 700, 500);
  cr->cd();
  r->Draw();

  c = new TCanvas("Energy against risetime", "E/R",10,10, 700, 500);
  c->cd();
  gStyle->SetPalette(1);
  er->Draw("COLZ");



  theApp->Run(kTRUE);

  /* sprintf(name,"%s","E.mca"); */
  /* if((output=fopen(name,"w"))==NULL) */
  /*   { */
  /*     printf("ERROR! I cannot open the mca file!\n"); */
  /*     exit(EXIT_FAILURE); */
  /*   } */
  /* fwrite(hist1,S32K*sizeof(int),1,output); */
  /* fclose(output); */

}
