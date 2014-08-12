#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
 
  Int_t d;
  WaveFormPar wpar;
  

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
    {
  
      fit_waveform(d,waveform,par,&wpar,theApp);
 
      if(par->type==type)
	CsI_ShowFit(d,waveform,par,&wpar,theApp);
		
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
      printf("\n ./wfit_show_fit_type_with_RF midas_input_data_file_name channel trc tf ts tp type\n");
      exit(-1);
    }
  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));
  chn=atoi(argv[2]);
  par->t[1]=atof(argv[3]);
  par->t[2]=atof(argv[4]);
  par->t[3]=atof(argv[5]);
  par->t[4]=atof(argv[6]);
  type=atof(argv[7]);
  theApp=new TApplication("App", &ac, av);

/* do sorting */
  sort_but_not_assemble(argv[1]);
 /* display results */

 


}
