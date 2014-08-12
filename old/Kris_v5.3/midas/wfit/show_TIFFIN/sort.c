#include "sort.h"
/*================================================================*/
Double_t myfunction(Int_t i,WaveFormPar* wpar)
{
  double x;

  x=i/1000.;
  if(i>=par->t[0]) 
    return par->am[2]*x*x+par->am[1]*x+par->am[0];
  else				     
    return wpar->s1*x+wpar->s0;

}
/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
 
  Int_t d,i;
  WaveFormPar wpar;
  double ch;

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
      fit_TIFFIN_waveform(d,waveform,par,&wpar);
      ch=par->chisq/par->ndf;
      /* if(ch>=chmin) */
      /*   if(ch<=chmax) */
      {
	
	print_fragment_info(ptr,S16K);
	printf(" b    : %10.2f\n",wpar.b);
	printf(" bsq  : %10.2f\n",wpar.bsq);
	printf(" max  : %10.2f\n",(double)wpar.max);
	printf("tmax  : %10.2f\n",(double)wpar.tmax);
	printf("temin : %10.2f\n",(double)wpar.temin);
	printf("temax : %10.2f\n",(double)wpar.temax);
	printf("t0    : %10.2f\n",(double)wpar.t00);


	printf("------ Chisq/NDF: %10.4f\n",ch);		
	i=0;
	printf("Id %1d Amp. Sig. %10.3f  Amp. Bas. %10.3f\n",i,(double)par->am[0],(double)wpar.s0);
	i=1;
	printf("Id %1d Amp. Sig. %10.3f  Amp. Bas. %10.3f\n",i,(double)par->am[1],(double)wpar.s1);
	i=2;
	printf("Id %1d Amp. Sig. %10.3f  Amp. Bas. %10.3f\n",i,(double)par->am[2],(double)wpar.s2);
	if(h!=NULL) delete h;		
	h=new TH1D("Waveform","Waveform",ptr->waveform_length,0,ptr->waveform_length);
	if(g!=NULL) delete g;
	g=new TH1D("Fit","Fit",ptr->waveform_length,0,ptr->waveform_length);
	if(c!=NULL) delete c;
	c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);
	for(Int_t i=0;i<ptr->waveform_length;i++)
	  {
	    h->Fill(i,waveform[i]);
	    g->Fill(i,myfunction(i,&wpar));
	  }
	h->Draw();
	g->SetLineColor(kRed);
	g->Draw("same");
	theApp->Run(kTRUE);
	
      }
	
    }

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

  
 
  int ac;
  char *av[10];

 if(argc!=3)
    {
      printf("\n ./wfit_show_TIFFIN midas_input_data_file_name channel");
      exit(-1);
    }
  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));
  chn=atoi(argv[2]);
 
  theApp=new TApplication("App", &ac, av);

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */



}
