#include "sort.h"


/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{

  double v;
  if(ptr->channel==chn)
    {

	/*Initialize the FFT sum histogram, using the waveform length of the first valid fragment to 		determine the number of bins*/
      if((len=ptr->waveform_length)!=0)
	{
		if (firstfragment == true)
		  {
		    firstfragment = false;
		    if(FFTsum!=NULL) delete FFTsum;
		    FFTsum=new TH1D("FFT_Sum","FFT_Sum",len/2,0,50);
		    if(FFTsumfit!=NULL) delete FFTsumfit;
		    FFTsumfit=new TH1D("FFT_Sum_Fit","FFT_Sum_Fit",len/2,0,50);	
		  }

	/*Initialize histograms to hold the waveform and single fragment FFT data*/	
	if(h!=NULL) delete h;
	h=new TH1D("Waveform","Waveform",len,0,len);
	if(fourier_transform!=NULL) delete fourier_transform;
	fourier_transform = 0;

	/*Fill waveform histogram with waveform values*/
	for(Int_t i=0;i<len;i++)
		h->Fill(i,waveform[i]);


	/*calculat the fit histogram */
	v=h->Integral()/(double)len;
	if(g!=NULL) delete g;
	g=new TH1D("Fit","Fit",len,0,len);
	for(Int_t i=0;i<len;i++)
	  g->Fill(i,v);

	/*Calculate FFT of single fragment waveform*/
	if(fourier_transform!=NULL) delete fourier_transform;  
	fourier_transform = 0;
	TVirtualFFT::SetTransform(0);
	fourier_transform = h->FFT(fourier_transform, "MAG");

	/*Increment waveform FFT sum histogram with FFT values*/

	for(Int_t i=1;i<len/2;i++)
	  FFTsum->AddBinContent(i,fourier_transform->GetBinContent(i));

	/*Calculate FFT of single fragment fit*/
	if(fourier_transform!=NULL) delete fourier_transform;  
	fourier_transform = 0;
	TVirtualFFT::SetTransform(0);
	fourier_transform = g->FFT(fourier_transform, "MAG");

	/*Increment fit FFT sum histogram with FFT values*/

	for(Int_t i=1;i<len/2;i++)
	  FFTsumfit->AddBinContent(i,fourier_transform->GetBinContent(i));

	}
    }


  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{

	int ac;
	char* av[10];

	if(argc!=3)
	{
		printf("\n ./FFT_baseline_fit midas_input_data_file_name channel\n");
		exit(-1);
	}

	chn=atoi(argv[2]);


	firstfragment = true;

	theApp=new TApplication("App", &ac, av);
	printf("Program shows a sum of FFTs of fragments in the specified channel.\n");

	/* do sorting */
	sort_but_not_assemble(argv[1]);
	
	//FFTsum->Write(); //use if one wants to write histogram to the file specified in sort.h
	/* display results */
	if(c!=NULL) delete c;
	c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);
	c->SetLogy();

	FFTsum->GetXaxis()->SetTitle("[MHz]");
	FFTsum->GetYaxis()->SetTitle("Magnitude [arb]");
	FFTsum->Draw();
	FFTsumfit->SetLineColor(kRed);
	FFTsumfit->Draw("same");
	theApp->Run(kTRUE);
}
