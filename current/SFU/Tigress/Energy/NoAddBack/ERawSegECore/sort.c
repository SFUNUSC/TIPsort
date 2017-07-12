#include "sort.h"

int analyze_data(raw_event *data)
{
	cal_event* cev;
  int pos,col,sgm,specNum;
  double e,eseg;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  if(cev->tg.h.FE>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.EHP&(1<<(pos-1)))!=0)
				if(cev->tg.det[pos].hge.FE>0)
					for(col=0;col<NCOL;col++)
					  if((cev->tg.det[pos].hge.EHP&(1<<col))!=0)
					    if(cev->tg.det[pos].ge[col].h.FE>0)
								if((cev->tg.det[pos].ge[col].h.EHP&1)!=0)
									{
										e=cev->tg.det[pos].ge[col].seg[0].E/cal_par->tg.contr_e; //core energy
										for(sgm=1;sgm<9;sgm++)//loop over real segments
											if((data->tg.det[pos].ge[col].h.EHP&(1<<sgm))!=0)
											//if((data->tg.det[pos].ge[col].h.EHP&1)!=0)
												{
													specNum=(pos-1)*NPOSTIGR + col*NCOL + (sgm-1);
													eseg=data->tg.det[pos].ge[col].seg[sgm].charge; //segment energy
												
													if(e>=0 && e<S32K)
														if(eseg>=0 && eseg<S32K)
															h[specNum]->Fill(e,eseg);//segment on y axis
														
													//printf("e: %f, eseg: %f\n",e,eseg);
													//getc(stdin);
												}
									}
								
	free(cev);
  return SEPARATOR_DISCARD;
}
/*=======================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  char HistName[132];
  int pos,col,sgm,specNum;

  if(argc!=2)
    {
      printf("Tigress_ERawSegECore master_file_name\n");
      printf("Program sorts raw energy data for Tigress segments vs calibrated core energies.\n");
      exit(-1);
    }
  
  printf("Program sorts raw energy data for Tigress segments vs calibrated core energies.\n");

	for(pos=1;pos<NPOSTIGR;pos++)
		for(col=0;col<NCOL;col++)
			for(sgm=1;sgm<9;sgm++)
				{
					specNum=(pos-1)*NPOSTIGR + col*NCOL + (sgm-1);
				  sprintf(HistName,"ESegECore_pos%02d_col%1d_seg%1d",pos,col,sgm);
				  h[specNum] = new TH2D(HistName,HistName,S1K,0,S32K,S1K,0,S32K);
				  h[specNum]->Reset();
				}
				
	name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  read_master(argv[1],name);  

  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined!\n");
      exit(EXIT_FAILURE);
    }

	if(name->flag.TIGRESS_cal_par==1)
    {
      printf("\nTIGRESS calibration read from the file:\n %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("\nTIGRESS calibration parameters not defined\n");
      exit(-1);
    }

  sort(name);

  //theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("ESegECore","ESegECore",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);

  TFile *f= new TFile(name->fname.root_output_file,"RECREATE");
  f->cd();
  for(pos=1;pos<NPOSTIGR;pos++)
		for(col=0;col<NCOL;col++)
			for(sgm=1;sgm<9;sgm++)
				{
					specNum=(pos-1)*NPOSTIGR + col*NCOL + (sgm-1);
				  sprintf(HistName,"ESegECore_pos%02d_col%1d_seg%1d",pos,col,sgm);
				  h[specNum]->Draw("COLZ");
				  h[specNum]->Write(HistName);
				}
  f->Close();
  //theApp->Run(kTRUE); 

}
