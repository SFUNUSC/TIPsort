#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos1,pos2;
  int col1,col2;
  double t1,t2;
  int t;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_GRIFFIN(data,&cal_par->gr,&cev->gr);

  if(cev->gr.h.FT>0)
    for(pos1=1;pos1<NPOSTIGR;pos1++)
      if((cev->gr.h.THP&(1<<(pos1-1)))!=0)
  	if(cev->gr.det[pos1].hge.FT>0)
  	  for(col1=0;col1<NCOL;col1++)
  	    if((cev->gr.det[pos1].hge.THP&(1<<col1))!=0)
  	      if(cev->gr.det[pos1].ge[col1].h.FT>0)
  		if((cev->gr.det[pos1].ge[col1].h.THP&1)!=0)
  		  {
  		    t1=cev->gr.det[pos1].ge[col1].seg[0].T/cal_par->gr.contr_t;

		    for(pos2=pos1+1;pos2<NPOSTIGR;pos2++)
		      if((cev->gr.h.THP&(1<<(pos2-1)))!=0)
			if(cev->gr.det[pos2].hge.FT>0)
			  for(col2=0;col2<NCOL;col2++)
			    if((cev->gr.det[pos2].hge.THP&(1<<col2))!=0)
			      if(cev->gr.det[pos2].ge[col2].h.FT>0)
				if((cev->gr.det[pos2].ge[col2].h.THP&1)!=0)
				  {
				    t2=cev->gr.det[pos2].ge[col2].seg[0].T/cal_par->gr.contr_t;
				    if(t1!=t2)
				      {
					h->Fill(t1,t2);
					h->Fill(t2,t1);
				      }
				    else
				      h->Fill(t1,t2);

				    t=(int)(t1-t2)+S2K;

				    if(t>=0&&t<S4K)
				      hist[t]++;
				    else
				      hist[S4K-1]++;

				    t=(int)(t2-t1)+S2K;

				    if(t>=0&&t<S4K)
				      hist[t]++;
				    else
				      hist[S4K-1]++;

				    t=(int)(t1-t2)+S2K;
				    if(t==S2K)
				    hist[t]--;
				  }

  		  }

 
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{

  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  FILE* output;

  if(argc!=2)
    {
      printf("\n ./GRIFFIN_TGeGe master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts calibrated time histogram for GRIFFIN core coincidences \n");
  h = new TH2D("TGeGe","TGeGe",100,700,799,100,700,799);
  h->Reset();

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

  memset(hist,0,sizeof(hist));

  read_master(argv[1],name);

  if(name->flag.inp_data!=1)
    {
      printf("\nInput data file not defined\n");
      exit(EXIT_FAILURE);
    }

  if(name->flag.GRIFFIN_cal_par==1)
        {
          printf("\nGRIFFIN calibration read from the file:\n %s\n",name->fname.GRIFFIN_cal_par);
          initialize_GRIFFIN_calibration(&cal_par->gr,name->fname.GRIFFIN_cal_par);
	  
        }
      else
        {
          printf("\nGRIFFIN calibration parameters not defined\n");
          exit(EXIT_FAILURE);
        }



  sort(name);

  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("TGeGe", "TGeGe",10,10, 500, 500);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");

  if((output=fopen("TGeGe.spn","w"))==NULL)
    {
      printf("\nI can't open file TGeGe.spn\n");
      exit(EXIT_FAILURE);
    }
  fwrite(hist,S4K*sizeof(int),1,output);
  
  fclose(output);

  
  theApp->Run(kTRUE);

}

