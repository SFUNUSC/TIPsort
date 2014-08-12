#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
 
  Int_t d;
  WaveFormPar wpar;
  double ch;

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
     
      fit_waveform(d,waveform,par,&wpar,theApp);
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


	printf("------ Chisq/NDF: %10.4f\n",ch);		
	for(Int_t i=0;i<5;i++)
	  printf("Id %1d Amp. %10.3f  T %10.3f\n",i,(double)par->am[i],(double)par->t[i]);
	if(h!=NULL) delete h;		
	h=new TH1D("Waveform","Waveform",ptr->waveform_length,0,ptr->waveform_length);
	if(g!=NULL) delete g;
	g=new TH1D("Fit","Fit",ptr->waveform_length,0,ptr->waveform_length);
	if(c!=NULL) delete c;
	c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);
	for(Int_t i=0;i<ptr->waveform_length;i++)
	  {
	    h->Fill(i,waveform[i]);
	    g->Fill(i,CsI_FitFunction(i,par));
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

 if(argc!=8)
    {
      printf("\n ./wfit_show_lin_two_components midas_input_data_file_name channel trc tf ts min max");
      exit(-1);
    }
  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));
  chn=atoi(argv[2]);
  par->t[1]=atof(argv[3]);
  par->t[2]=atof(argv[4]);
  par->t[3]=atof(argv[5]);
  chmin=atof(argv[6]);
  chmax=atof(argv[7]);
  theApp=new TApplication("App", &ac, av);

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */

 


}
