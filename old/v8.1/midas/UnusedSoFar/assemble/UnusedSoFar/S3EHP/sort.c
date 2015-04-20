#include "sort.h"

/*================================================================*/
int analyze_assembled_event(raw_event* data)
{
  int r,s;

      if(data->s3.sh.Efold>0)
	 if(data->s3.rh.Efold>0)
	    for(r=1;r<NS3RING;r++)
	      if((data->s3.rh.EHP&(1<<r))!=0)
		for(s=1;s<NS3SEC;s++)
		  if((data->s3.sh.EHP&(1<<s))!=0)
		    if(data->s3.ring[r].charge>threshold)
		      if(data->s3.sec[s].charge>threshold)
			{
			  S3EHP->Fill(s,r+4);
			  S3S->Fill(s);
			  S3R->Fill(r);
			}

   
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  TCanvas *c,*cs,*cr;
  TApplication *theApp;


  if(argc!=4)
    {
      printf("\n ./assemble_S3EHP midas_input_data_file_name  map threshold\n");
      exit(-1);
    }

 threshold=atof(argv[3]);
  /* initialize histograms */
    S3EHP=new TH2D("S3EHP","S3EHP",32,1,33,33,0,32);
    S3EHP->Reset();
    S3S=new TH1D("S3SHP","S3SEHP",40,0,39);
    S3S->Reset();
    S3R=new TH1D("S3RHP","S3REHP",40,0,39);
    S3R->Reset();

/* do sorting */
  sort_and_assemble(argv[1],argv[2]);
 /* display results */
  printf("\n"); 


       printf("Program sorts energy hit patterns for the S3 detector \n");

      theApp=new TApplication("App", &argc, argv);
 
      cs = new TCanvas("SectorEnergyHitPattern", "SectorEnergyHitPattern",10,10, 500, 500);
      S3S->Draw();
      theApp->Run(kTRUE);

      cr = new TCanvas("RingEnergyHitPattern", "RingEnergyHitPattern",10,10, 500, 500);
      S3R->Draw();
      theApp->Run(kTRUE);

      c = new TCanvas("EnergyHitPattern", "EnergyHitPattern",10,10, 500, 500);
      gPad->SetLogz();
      gStyle->SetPalette(1);
      S3EHP->Draw("lego2.pol");

      theApp->Run(kTRUE);


  
}
