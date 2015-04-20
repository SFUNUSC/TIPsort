#include "sort.h"
/*================================================================*/
Double_t myfunction(int i,ShapePar par)
{
 
  Double_t x,s;


  x=i-par.t[0];
  if(x<=0) 
    return par.am[0];
  else
    {   
      s=par.am[0];
      s+=par.am[1]*exp(-x/par.t[1]);
      for(int i=2;i<5;i++)
	s-=par.am[i]*exp(-x/par.t[i]);
      return s;
    }
}
/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
 
  int    d;  
  double ch;
  ShapePar par;
  WaveFormPar wpar;

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
      {
	memcpy(&par,&p,sizeof(ShapePar));
	ch=fit_waveform(d,waveform,&par,&wpar);
       
	if(ch>=chmin)
	  if(ch<=chmax)
	    {
	      
	      print_fragment_info(ptr,S16K);
	      printf("   Waveform    b: %10.1f\n",wpar.b);
	      printf("   Waveform  max: %10.1f\n",wpar.max);
	      printf("   Waveform tmax: %10.4f\n",wpar.tmax);
	      printf("   Waveform  t10: %10.4f\n",wpar.t10);
	      printf("   Waveform  t90: %10.4f\n",wpar.t90);
	      printf(" Risetime T10T90: %10.4f\n",wpar.t10t90);
	      printf("-------   Fit ID: %1d\n",par.type);
	      printf("---Min Chisq/NDF: %10.4f\n",ch);
	      for(Int_t i=0;i<5;i++)
		  printf("Id %1d Amp. %10.3f  T %10.3f\n",i,par.am[i],par.t[i]);
	

		if(h!=NULL) delete h;		
		h=new TH1D("Waveform","Waveform",ptr->waveform_length,0,ptr->waveform_length);
		if(g!=NULL) delete g;
		g=new TH1D("Fit","Fit",ptr->waveform_length,0,ptr->waveform_length);
		if(c!=NULL) delete c;
		c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);
		for(Int_t i=0;i<ptr->waveform_length;i++)
		  {
		    h->Fill(i,waveform[i]);
		    g->Fill(i,myfunction(i,par));
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

 if(argc!=9)
    {
      printf("\n ./wfit_lin_chisq midas_input_data_file_name channel trc tf ts tr min max");
      exit(-1);
    }

 chn=atoi(argv[2]);

 p.t[0]=0;
 p.t[1]=atof(argv[3]);
 p.t[2]=atof(argv[4]);
 p.t[3]=atof(argv[5]);
 p.t[4]=atof(argv[6]);
 
  chmin=atof(argv[7]);
  chmax=atof(argv[8]);
  theApp=new TApplication("App", &ac, av);

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */

 


}
