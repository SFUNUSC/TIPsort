#include "sort.h"
/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
 
  Int_t d;
  WaveFormPar wpar;
  double ch,ch_ex;
  
  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
      {
	fit_waveform_with_RF(d,waveform,par,&wpar);   
	ch=par->chisq/par->ndf;
	ch_ex=par->chisq_ex/par->ndf_ex;
	  if(ch_ex>10*ch)
	    {	      
	      print_fragment_info(ptr,S16K);
	      CsI_ShowFitWithRF(d,waveform,par,&wpar,theApp);	      
	    }
    }

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

  int ac;
  char *av[10];


  theApp=new TApplication("App", &ac, av);
 
 if(argc!=8)
    {
      printf("\n ./wfit_show_bad_ex_with_RF midas_input_data_file_name channel trc tf ts tp trf\n");
      exit(-1);
    }
  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));
  chn=atoi(argv[2]);
  par->t[1]=atof(argv[3]);
  par->t[2]=atof(argv[4]);
  par->t[3]=atof(argv[5]);
  par->t[4]=atof(argv[6]);
  par->rf[0]=atof(argv[7]);
 
/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */

 


}
