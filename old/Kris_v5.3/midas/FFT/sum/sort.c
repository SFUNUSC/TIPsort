#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{


  if(ptr->channel==chn)
    {

	/*Initialize the FFT sum histogram, using the waveform length of the first valid fragment to 		determine the number of bins*/
      if((len=ptr->waveform_length)>numb)
	{
		if (firstfragment == true){
		firstfragment = false;
		if(FFTsum!=NULL) delete FFTsum;
		FFTsum=new TH1D("FFT_Sum","FFT_Sum",numb/2,0,50);
	}

	/*Initialize histograms to hold the waveform and single fragment FFT data*/	
	if(h!=NULL) delete h;
	h=new TH1D("FFT","FFT",numb,0,numb);


	/*Fill waveform histogram with waveform values*/
	for(Int_t i=0;i<numb;i++)
		h->Fill(i,waveform[i]);

	/*Calculate FFT of single fragment*/
	if(fourier_transform!=NULL) delete fourier_transform;  
       	fourier_transform = 0;

       	TVirtualFFT::SetTransform(0);
	fourier_transform=h->FFT(fourier_transform, "MAG");

	/*Increment FFT sum histogram with FFT values*/


	  for(Int_t i=1;i<numb/2;i++)
	    FFTsum->AddBinContent(i,fourier_transform->GetBinContent(i));

	}
    }


  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

	int ac;
	char* av[10];

	if(argc!=4)
	{
		printf("\n ./FFT_sum midas_input_data_file_name channel number_of_samples\n");
		exit(-1);
	}
	chn=atoi(argv[2]);
	numb=atoi(argv[3]);
      
	firstfragment = true;

	theApp=new TApplication("App", &ac, av);
	printf("Program shows a sum of FFTs of fragments in the specified channel.\n");

	/* do sorting */
	sort_but_not_assemble(argv[1]);
	
	//FFTsum->Write(); //use if one wants to write histogram to the file specified in sort.h
	/* display results */
	if(c!=NULL) delete c;
	c = new TCanvas("FFT", "FFT",10,10, 700, 500);
	c->SetLogy();

	FFTsum->GetXaxis()->SetTitle("[MHz]");
	FFTsum->GetYaxis()->SetTitle("Magnitude [arb]");
	FFTsum->Draw();
	theApp->Run(kTRUE);
}
