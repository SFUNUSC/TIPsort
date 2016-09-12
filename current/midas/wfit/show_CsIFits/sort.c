#include "sort.h"
/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  Int_t d;
  WaveFormPar wpar;
  double ch;
  int type;
  
  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
      {
	fit_CsI_waveform(d,waveform,par,&wpar);
	ch=par->chisq/par->ndf;
	
	type=par->type;
	//printf("type %d\n",type);
	//getc(stdin);
	
	if(ch>=chmin)
	  if(ch<=chmax)
	    if(type>=type_low)
	      if(type<=type_high)
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


 if(argc!=11)
    {
      printf("wfit_show_CsIFits midas_input_data_file_name channel tRC tF tS tGamma chisq_min chisq_max type_low type_high\n");
      printf("type 1 = 2 component, type 2 = slow only 3=PIN hit\n");
      exit(-1);
    }

  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));

  strcpy(name,argv[1]);
  chn=atoi(argv[2]);
  par->t[1]=atof(argv[3]); //set tRC
  par->t[2]=atof(argv[4]); //set tF
  par->t[3]=atof(argv[5]); //set tS
  par->t[4]=atof(argv[6]); //set tGamma
  chmin=atof(argv[7]);
  chmax=atof(argv[8]);
  type_low=atoi(argv[9]);
  type_high=atoi(argv[10]);

  theApp=new TApplication("App", &argc, argv);

/* do sorting */
  sort_but_not_assemble(name);
 /* display results */
}
