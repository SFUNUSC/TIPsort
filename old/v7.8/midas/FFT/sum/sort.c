
 #include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{
  int i;
  
  counter++;
  if(counter>number) return 0;
  if(ptr->channel==chan)
    {
      /*Initialize the FFT sum histogram, using the waveform length of the first valid
	fragment to determine the number of bins*/
      if(ptr->waveform_length==length)
        {
	  if (firstfragment == true)
	    {
	      firstfragment = false;
	      if(FFTsum!=NULL) delete FFTsum;
	      FFTsum=new TH1D("FFT_Sum","FFT_Sum",length/2,0,50);
	    }
	  
	  /*Initialize histograms to hold the waveform and single fragment FFT data*/        
	  if(h!=NULL) delete h;
	  h=new TH1D("FFT","FFT",length,0,length);
	  
	  /*Fill waveform histogram with waveform values*/
	  for(i=0;i<length;i++) h->Fill(i,waveform[i]);
	  
	  /*Calculate FFT of single fragment*/
	  if(fourier_transform!=NULL) delete fourier_transform;  
	  fourier_transform = 0;
	  
	  TVirtualFFT::SetTransform(0);
	  fourier_transform=h->FFT(fourier_transform, "MAG");

	  /*Increment FFT sum histogram with FFT values*/
          for(i=1;i<length/2;i++) FFTsum->AddBinContent(i,fourier_transform->GetBinContent(i));
	}}

  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  int ac=0;
  char* av[10]={0};
  
  if(argc!=5)
    {
      printf("\n ./FFT_sum midas_input_data channel waveform_length number_of_fragments\n");
      exit(-1);
    }
  chan=atoi(argv[2]);
  length=atoi(argv[3]);
  number=atoi(argv[4]);
  
  printf("Program shows a sum of FFTs of fragments in the specified channel.\n");
  theApp=new TApplication("App", &ac, av);
  sort_but_not_assemble(argv[1]);
        
  //FFTsum->Write(); //use if one wants to write histogram to the file specified in

  if(c!=NULL) delete c;
  c = new TCanvas("FFT", "FFT",10,10, 700, 500);
  c->SetLogy();
  
  FFTsum->GetXaxis()->SetTitle("[MHz]");
  FFTsum->GetYaxis()->SetTitle("Magnitude [arb]");
  FFTsum->Draw();
  theApp->Run(kTRUE);
}
