#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double chi,s,f,r,e;
  
  if((data->h.setupHP&RF_BIT)==0) 
    return SEPARATOR_DISCARD;

  if((data->h.setupHP&CsIArray_BIT)==0) 
    return SEPARATOR_DISCARD;
  
  if(data->csiarray.h.Tfold>0) /* Require good CsI time*/
    if(data->csiarray.h.Efold>0) /* Require good CsI energy */
      for(pos=1;pos<NCSI;pos++)
	if((data->csiarray.h.THP&(one<<pos))!=0)
	  {
	    type=data->csiarray.wfit[pos].type;
	    chi=data->csiarray.wfit[pos].chisq;
	    chi/=data->csiarray.wfit[pos].ndf;
	    if(type>=idmin && type<=idmax)
	      if(chi>=chimin && chi<=chimax)
		{
		  e=data->csiarray.wfit[pos].am[1];
		  s=data->csiarray.wfit[pos].am[3];
		  f=data->csiarray.wfit[pos].am[2];
		  if(f==0)
		    r=s*100;
		  else
		    r=s/f*100;
		  
		  r+=100;
		  
		  //printf("amp %10.3f slow %10.3f fast %10.3f pid %10.3f\n",e,s,f,r);
		  //getc(stdin);
		  
		  if(r>S32K-4) r=S32K-4;
		  
		  h[pos]->Fill(e,r);
		  h_e[pos]->Fill(e);
		  
		  //hist1[(int)rint(r)]++;
		  //hist2[(int)rint(e)]++;
		}
	  }
  return SEPARATOR_DISCARD;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  char HistName1[132],HistName2[132];

  if(argc!=6)
    {
      printf("CsIArray_PID_ER master_file_name idmin idmax chimin chimax\n");
      exit(-1);
    }
  
  for(pos=1;pos<NCSI;pos++)
    {
      sprintf(HistName1,"PID_ER_%02d",pos);
      h[pos] = new TH2D(HistName1,HistName1,S8K,0,S32K,S1K/2,0,S1K/2);
      h[pos]->Reset();
      sprintf(HistName2,"PID_E_%02d",pos);
      h_e[pos] = new TH1D(HistName2,HistName2,S8K,0,S32K);
      h_e[pos]->Reset();
    }
  
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);

  printf("Program sorts PID_ER histogram for the CsIArray \n");
  
  memset(hist1,0,sizeof(hist1));
  memset(hist2,0,sizeof(hist2));
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));

  read_master(argv[1],name);  

  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined!\n");
      exit(EXIT_FAILURE);
    }

  sort(name);

  if((output=fopen("CsIArray_PID_Ratio.mca","w"))==NULL)
    {
      printf("ERROR!!! I can't open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(hist1,S32K*sizeof(int),1,output);
  fclose(output);

  if((output=fopen("CsIArray_PID_Energy.mca","w"))==NULL)
    {
      printf("ERROR!!! I can't open the mca file!\n");
      exit(EXIT_FAILURE);
    }
  fwrite(hist2,S32K*sizeof(int),1,output);
  fclose(output);
  
  //theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("PID_ER", "PID_ER",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);

  TFile *f= new TFile(name->fname.root_output_file,"RECREATE");
  f->cd();
  for(pos=1;pos<NCSI;pos++)
    {
      sprintf(HistName1, "PID_ER_%02d", pos);
      sprintf(HistName2, "PID_E_%02d", pos);
      h[pos]->Draw("COLZ");
      h[pos]->GetXaxis()->SetTitle("Amplitude [arb.]");
      //h[pos]->GetXaxis()->CenterTitle(true);
      h[pos]->GetYaxis()->SetTitle("PID value [100*(1+A_{S}/A_{F})]");
      //h[pos]->GetYaxis()->CenterTitle(true);
      h[pos]->GetYaxis()->SetTitleOffset(1.3);
      h[pos]->Write(HistName1);
      h_e[pos]->Draw("");
      h_e[pos]->Write(HistName2);
    }
  f->Close();
  //theApp->Run(kTRUE); 
}
