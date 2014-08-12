#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
  int dt;

  if(data->h.timestamp_up_min==data->h.timestamp_up_max)
    {
      dt=data->h.timestamp_max-data->h.timestamp_min;
      h->Fill(dt);
    }
  else
    {
      printf("Time stamp inconsistency for trigger number %d\n",data->h.trig_num&0x7fffffff);
      printf("Time stamp up min. %12d 0x%8.8x\n",data->h.timestamp_up_min,data->h.timestamp_up_min);
      printf("Time stamp up max. %12d 0x%8.8x\n",data->h.timestamp_up_max,data->h.timestamp_up_max);
      printf("Time stamp    min. %12d 0x%8.8x\n",data->h.timestamp_min,data->h.timestamp_min);
      printf("Time stamp    max. %12d 0x%8.8x\n",data->h.timestamp_max,data->h.timestamp_max);
    }	  
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  TCanvas *c;
  TApplication *theApp;  
  if(argc!=3)
    {
      printf("\n ./assemble_TimeStampDifference  midas_input_data_file_name map_file_name\n");
      exit(-1);
    }
  h = new TH1D("DTs","DTs",128,0,128);
  h->Reset();

/* do sorting */
  sort_and_assemble(argv[1],argv[2]);
 /* display results */
  printf("\n"); 

printf("Program sorts time stamp difference for fragments in an assembled event \n");
 theApp=new TApplication("App", &argc, argv);
 c = new TCanvas("Time Stamp Difference", "Time Stamp Difference",10,10, 500, 700);
 gPad->SetLogy();
 h->Draw();
 theApp->Run(kTRUE);


  
}
