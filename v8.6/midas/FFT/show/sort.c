#include "sort.h"
/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{
  int time, i;
  
  if(ptr->channel==chan)
    {
      time=ptr->cfd&0x00ffffff;
      time-=(ptr->timestamp*16)&0x00ffffff;
      printf("=========================================================\n");
      printf("Trigger  number : %8d 0x%8.8x\n",ptr->trigger_num&0x0fffffff,ptr->trigger_num);
      printf("           Port : %8d 0x%8.8x\n",ptr->port,ptr->port);
      printf("          Tig10 : %8d 0x%8.8x\n",ptr->tig10,ptr->tig10);
      printf("      Collector : %8d 0x%8.8x\n",ptr->collector,ptr->collector);
      printf("        Channel : %8d 0x%8.8x\n",ptr->channel,ptr->channel);
      printf("         Charge : %8d 0x%8.8x\n",ptr->charge,ptr->charge);
      printf("            CFD : %8d 0x%8.8x\n",ptr->cfd,ptr->cfd);
      printf("            LED : %8d 0x%8.8x\n",ptr->led,ptr->led);
      printf("     Time Stamp : %8d 0x%8.8x\n",ptr->timestamp,ptr->timestamp);
      printf("  Time Stamp Up : %8d 0x%8.8x\n",ptr->timestamp_up,ptr->timestamp_up);
      printf("           Time : %8d 0x%8.8x\n",time+S16K, time+S16K);
      printf("Waveform length : %8d 0x%8.8x\n",ptr->waveform_length,ptr->waveform_length);
      
      if(ptr->waveform_length==length)
  	{
  	  /*Initialize histogram and fill it with waveform values*/			  
	  if(h!=NULL) delete h;
  	  h=new TH1D("Waveform","Waveform",ptr->waveform_length,0,ptr->waveform_length-1);
	  
  	  for(i=0;i<ptr->waveform_length;i++) h->Fill(i,waveform[i]);
	  
  	  /*Calculate FFT of single fragment and place in a histogram*/
  	  if(fourier_transform!=NULL) delete fourier_transform;
  	  fourier_transform=0;
  	  TVirtualFFT::SetTransform(0);
  	  fourier_transform = h->FFT(fourier_transform, "MAG");
	  
	  fourier_transform->SetTitle("Magnitude of the FFT");
	  /*Use this line to adjust x-axis range of FFT*/
	  fourier_transform->GetXaxis()->SetRange(1,(ptr->waveform_length));
	  //fourier_transform->Write(); //use if one wants to write histogram to the file specified in sort.h                                                                        
	  if(c!=NULL) delete c;
	  c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);
	  fourier_transform->Draw();
	  theApp->Run(kTRUE);
	}}
    return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  int ac=0;
  char* av[10]={0};

  if(argc!=4)
    {
      printf("FFT_show midas_input_data channel waveform_length\n");
      exit(-1);
    }
  chan=atoi(argv[2]);
  length=atoi(argv[3]);

  theApp=new TApplication("App", &ac, av);
  printf("Program shows FFTs of fragments in the specified channel.\n");
  sort_but_not_assemble(argv[1]);
}
