#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{

  if(ptr->channel==chn)
    {
 
    }


  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

  int ac;
  char* av[10];

  TCanvas *c;  
  TApplication *theApp;

 if(argc!=3)
    {
      printf("\n ./rawSpectra_T10T90 midas_input_data_file_name channel\n");
      exit(-1);
    }

 chn=atoi(argv[2]);

 /* do sorting */
 sort_but_not_assemble(argv[1]);
 /* display results */
 printf("\n"); 
 
 printf("Program provides T10-T90 timing information on selected channel\n");

 theApp=new TApplication("App", &ac, av); 
}
