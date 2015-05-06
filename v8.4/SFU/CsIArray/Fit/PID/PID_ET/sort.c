#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double chi,t,e;
  
  pos=det_id;
  //for(pos=1;pos<NCSI;pos++)
  if((data->csiarray.h.THP&(one<<pos))!=0)
    {
      type=data->csiarray.wfit[pos].type;
      chi=data->csiarray.wfit[pos].chisq;
      chi/=data->csiarray.wfit[pos].ndf;
      
      if(type>=idmin && type<=idmax)
	if(chi>=chimin && chi<=chimax)
	  {
	    e=data->csiarray.csi[pos].charge;
	    //e=data->csiarray.wfit[pos].am[1];
	    t=16.*data->csiarray.wfit[pos].t[0];
	    
	    if((data->h.setupHP&RF_BIT)!=0)
              {
                t-=data->rf.sin.t0;
                t+=S8K;
		
                if(t<0)    t=S32K-1000;
                if(t>S32K) t=S32K-2000;
              }
            else
              t=S32K-3000;

            t/=16;
	    h->Fill(e,t);
	    
	    if(t>S32K-3000)   t=S32K-4000;
	    if(e<0 || e>S32K) e=S32K-4000;

	    hist1[(int)rint(t)]++;
	    hist2[(int)rint(e)]++;
	  }}
  
  return SEPARATOR_DISCARD;
}
/*==============================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  
  if(argc!=7)
    {
      printf("CsIArray_PID_TR sfu_input_data idmin idmax chimin chimax det_number\n");
      exit(-1);
    }

  h = new TH2D("PID","PID",S2K,0,S32K,S1K,0,S1K);
  h->Reset();
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  det_id=atof(argv[6]);

  printf("Program sorts CsIArray_PID_ET with DAQ E and TFitRaw.\n");
  
  memset(hist1,0,sizeof(hist1));
  memset(hist2,0,sizeof(hist2));
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  
  sort(name);
  
  if((output=fopen("CsIArray_PID_time.mca","w"))==NULL)
    {
      printf("ERROR!!! I can't open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  //for(int pos=0;pos<NCSI;pos++)
  fwrite(hist1,S32K*sizeof(int),1,output);
  fclose(output);
  
  if((output=fopen("CsIArray_PID_energy.mca","w"))==NULL)
    {
      printf("ERROR!!! I can't open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  //for(int pos=0;pos<NCSI;pos++)
  fwrite(hist2,S32K*sizeof(int),1,output);
  fclose(output);

  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("PID_ET", "PID_ET",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);
}
