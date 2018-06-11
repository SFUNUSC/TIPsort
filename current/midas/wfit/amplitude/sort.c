#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  WaveFormPar wpar;
  double charge;
  double ampl;  
  int channel;

  channel=ptr->channel;

  if( (channel>=tig_low && channel<=tig_high ) || (channel>=csi_low && channel<=csi_high) )
    {
      if(channel>=tig_low && channel<=tig_high)
	wpar.baseline_range=T0RANGE;
      else if(channel>=csi_low && channel<=csi_high)
	wpar.baseline_range=CSI_BASELINE_RANGE;
      
      if(ptr->waveform_length!=0)
	{
	  get_baseline(wpar.baseline_range,waveform,&wpar);
	  get_tmax(ptr->waveform_length,waveform,&wpar);
	  
	  charge=(double)(ptr->charge)/125.;
	  ampl=wpar.max-wpar.baseline;
	  
	  h[channel]->Fill(ampl,charge);

	  /* summed histograms for TIGRESS and CsI */
	  if(channel>=tig_low && channel<=tig_high)
	    htig->Fill(ampl,charge);
	  else if(channel>=csi_low && channel<=csi_high)
	    hcsi->Fill(ampl,charge);
	}
    }
 
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  char HistName1[132];
  char title[132];

  if(argc!=6)
    {
      printf("wfit_amplitude midas_input_data tig_low tig_high csi_low csi_high\n");
      exit(-1);
    }

  printf("Program plots tmax-baseline vs charge for waveforms.\n");
  tig_low=atoi(argv[2]);
  tig_high=atoi(argv[3]);
  csi_low=atoi(argv[4]);
  csi_high=atoi(argv[5]);

  for(int i=0;i<S2K;i++)
    {
      sprintf(HistName1,"amplitude_%04d",i);
      h[i]=new TH2D(HistName1,HistName1,512,0,S8K,512,0,S32K);
      h[i]->Reset();
    }

  htig=new TH2D("TIGRESS Sum","TIGRESS Sum",512,0,S8K,512,0,S32K);
  htig->Reset();

  hcsi=new TH2D("CsI Sum","CsI Sum",512,0,S8K,512,0,S32K);
  hcsi->Reset();
  
  sort_but_not_assemble(argv[1]);

  sprintf(title,"amplitude.root");
  TFile f(title, "recreate");
  
  for(int i=0;i<S2K;i++)
    {
      /* only write if hitogram is not empty */
      if(h[i]->GetEntries())
	{
	  sprintf(HistName1, "amplitude_%04d",i);
	  h[i]->GetXaxis()->SetTitle("amplitude");
	  h[i]->GetXaxis()->CenterTitle(true);
	  h[i]->GetYaxis()->SetTitle("charge");
	  h[i]->GetYaxis()->CenterTitle(true);
	  h[i]->GetYaxis()->SetTitleOffset(1.5);
	  h[i]->SetOption("COLZ");
	  h[i]->Write();
	}
    }

  if(htig->GetEntries())
    {
      htig->GetXaxis()->SetTitle("amplitude");
      htig->GetXaxis()->CenterTitle(true);
      htig->GetYaxis()->SetTitle("charge");
      htig->GetYaxis()->CenterTitle(true);
      htig->GetYaxis()->SetTitleOffset(1.5);
      htig->SetOption("COLZ");
      htig->Write();
    }
  if(hcsi->GetEntries())
    {
      hcsi->GetXaxis()->SetTitle("amplitude");
      hcsi->GetXaxis()->CenterTitle(true);
      hcsi->GetYaxis()->SetTitle("charge");
      hcsi->GetYaxis()->CenterTitle(true);
      hcsi->GetYaxis()->SetTitleOffset(1.5);
      hcsi->SetOption("COLZ");
      hcsi->Write();  
    }
  
  return 0;
}
