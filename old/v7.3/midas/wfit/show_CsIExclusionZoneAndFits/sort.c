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
      fit_CsI_waveform(d,waveform,par,&wpar);
      ch=par->chisq/par->ndf;

      if(ch>=chmin)
        if(ch<=chmax)
      {	
	print_fragment_info(ptr,S16K);
	show_CsI_exclusion_zone_and_fit(d,waveform,par,&wpar,theApp);
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
      printf("wfit_show_CsIExclusionZoneAndFits midas_input_data_file_name channel tRC tF tS tGamma min max");
      exit(-1);
    }

  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));

  chn=atoi(argv[2]);
  par->t[1]=atof(argv[3]); //set tRC
  par->t[2]=atof(argv[4]); //set tF
  par->t[3]=atof(argv[5]); //set tS
  par->t[4]=atof(argv[6]); //set tGamma
  chmin=atof(argv[7]);
  chmax=atof(argv[8]);

  theApp=new TApplication("App", &ac, av);

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */
}
