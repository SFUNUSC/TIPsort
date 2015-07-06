#include "sort.h"
/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  Int_t d,e;
  WaveFormPar wpar;
  //double ch;
  //int type;
  
  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
      {
        check_for_pileup(d,waveform,par,&wpar);
        if ((e=wpar.pileupflag) == pileup_flag) //check whether the user specified pileup condition and the one returned from the waveform analyzer agree 
          {
            print_fragment_info(ptr,S16K);
            display_CsI_and_TF(d,waveform,par,&wpar,theApp);
          }
      }
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

  char name[128];

  if(argc!=8)
    {
      printf("wfit_show_CsITF midas_input_data_file_name channel filter_dist averaging_samples fall_amount rise_amount pileup_flag\n");
      printf("Pileup rejection (trapezoidal filter) algorithm constants:\n");
      printf("filter_dist: how far between summing regions in the filter (in samples).\n");
      printf("averaging_samples: how many samples are in each summing region.\n");
      printf("fall_amount: how much the filter output should fall below the maximum before rising again.\n");
      printf("rise_amount: how much the filter output should rise to within the old maximum before triggering the pileup flag.\n");
      printf("pileup_flag = 0: Show only events without pileup.\n");
      printf("pileup_flag = 1: Show only events with pileup.\n");
      exit(-1);
    }

  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));

  strcpy(name,argv[1]);
  chn=atoi(argv[2]);
  par->filter_dist=atoi(argv[3]);
  par->averaging_samples=atoi(argv[4]);
  par->fall_amount=atoi(argv[5]);
  par->rise_amount=atoi(argv[6]);
  pileup_flag=atoi(argv[7]);

  theApp=new TApplication("App", &argc, argv);

/* do sorting */
  sort_but_not_assemble(name);
 /* display results */
}
