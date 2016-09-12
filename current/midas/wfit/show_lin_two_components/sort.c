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
	wpar.baseline_range=CSI_BASELINE_RANGE;
	get_exclusion_zone_for_CsI(d,waveform,&wpar);
	get_shape(4,d,waveform,par,&wpar);
	ch=par->chisq/par->ndf;
	//printf("here?\n");
	//getc(stdin);
	
	if(ch>=chmin)
	  if(ch<=chmax)
	    {	
	      print_fragment_info(ptr,S16K);
	      show_CsI_Fit(d,waveform,par,&wpar,theApp);
	    }
    }
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
// int ac;
// char *av[10];

  char name[128];

 if(argc!=8)
    {
      printf("wfit_show_lin_two_components midas_input_data_file_name channel tRC tF tS min max");
      exit(-1);
    }

  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));

  strcpy(name,argv[1]);
  chn=atoi(argv[2]);
  par->t[1]=atof(argv[3]); //set tRC
  par->t[2]=atof(argv[4]); //set tF
  par->t[3]=atof(argv[5]); //set tS
  chmin=atof(argv[6]);
  chmax=atof(argv[7]);

  theApp=new TApplication("App", &argc, argv);

/* do sorting */
  sort_but_not_assemble(name);
 /* display results */
}
