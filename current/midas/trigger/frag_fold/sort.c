#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
  //printf("trig %d counter %d fold %d\n",data->h.trig_num,counter,data->h.frag_fold);
  //getc(stdin);

  if(data->h.trig_num<=counter)
    {
      if((data->h.trig_num%1000)==0)
	printf("events analyzed %d\r",data->h.trig_num);

      //printf("trigger number %d fragment fold %d\n",data->h.trig_num,data->h.frag_fold);
      //getc(stdin);

      h->Fill(data->h.frag_fold);
      return 0;
    }

  else
    {
      theApp=new TApplication("App", &ac, av);
      c = new TCanvas("Fold", "Fold",10,10, 700, 500);
      gPad->SetLogy();
      h->GetXaxis()->SetTitle("Fragment fold");
      h->GetYaxis()->SetTitle("Counts");
      h->Draw();
      theApp->Run(kTRUE);
      
      exit(1);
      return 1;
    }
}
/*================================================================*/
int main(int argc, char *argv[])
{
  if(argc!=4)
    {
      printf("\n ./trig_frag_fold midas_input_data_file_name map_file_name counter\n");
      exit(-1);
    }

  counter=atoi(argv[3]);

  h=new TH1D("Fold","Fold",256,0,256);
  h->Reset();
/* do sorting */
  sort_and_assemble(argv[1],argv[2]);

 /* display results */
  printf("\n"); 
  printf("Program displays a histogram of fragment fold associated with a given trigger\n");



}
