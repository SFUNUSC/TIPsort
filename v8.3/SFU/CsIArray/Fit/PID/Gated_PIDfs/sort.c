#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int type;
  double chi,s,f,r,t;
  int aGateFlag=0;
  int pGateFlag=0;

  if((data->h.setupHP&RF_BIT)==0) return SEPARATOR_DISCARD;
  if((data->h.setupHP&CsIArray_BIT)==0) return SEPARATOR_DISCARD;
  
  for(pos=1;pos<NCSI;pos++)
    if(positionGateFlag[pos]==1)
      if((data->csiarray.h.THP&(one<<pos))!=0)
	{
	  type=data->csiarray.wfit[pos].type;
	  chi=data->csiarray.wfit[pos].chisq;
	  chi/=data->csiarray.wfit[pos].ndf;
	  
	  if((type>=idmin) && (type<=idmax))
	    if((chi>=chimin) && (chi<=chimax))
	      {
		t=16.*data->csiarray.wfit[pos].t[0]; 
		t-=data->rf.sin.t0;
		
		t+=S8K;
		if(t<0)t=S32K-2000;
		if(t>S32K) t=S32K-3000;
		
		t/=16;
		s=data->csiarray.wfit[pos].am[3];
		f=data->csiarray.wfit[pos].am[2];
		r=s/f*100;
		r+=100;
		
		if(aGate[pos]->IsInside(t,r)) aGateFlag++;
		if(pGate[pos]->IsInside(t,r)) pGateFlag++;
		
		//check if there is proton or alpha; discard if not
		if((pGateFlag==0)&&(aGateFlag)==0) notInGate_h[pos]->Fill(t,r);//return SEPARATOR_DISCARD;
		
		//use explicit conditions
		if((aGateFlag==1)&&(pGateFlag==1)) ap_h[pos]->Fill(t,r);
		if((aGateFlag==1)&&(pGateFlag==0))  a_h[pos]->Fill(t,r);
		if((aGateFlag==0)&&(pGateFlag==1))  p_h[pos]->Fill(t,r);
	      }}
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *canvas;
  //TApplication *theApp;
  TFile *m,*n;
  FILE *gateNameFile;
  char aGateName[132],pGateName[132],HistName[132],detector[132];
  int det;

  if(argc!=6)
    {
      printf("CsIArray_Gated_PIDtr master_file_name idmin idmax chimin chimax\n");
      exit(-1);
    }

  for(pos=1;pos<NCSI;pos++)
    {
      sprintf(HistName,"PID_TR_%02d_ap",pos);
      ap_h[pos] = new TH2D(HistName,HistName,S1K,0,S1K,S1K/2,0,S1K/2);
      ap_h[pos]->Reset();

      sprintf(HistName,"PID_TR_%02d_a",pos);
      a_h[pos] = new TH2D(HistName,HistName,S1K,0,S1K,S1K/2,0,S1K/2);
      a_h[pos]->Reset();

      sprintf(HistName,"PID_TR_%02d_p",pos);
      p_h[pos] = new TH2D(HistName,HistName,S1K,0,S1K,S1K/2,0,S1K/2);
      p_h[pos]->Reset();

      sprintf(HistName,"PID_TR_%02d_notInGate",pos);
      notInGate_h[pos] = new TH2D(HistName,HistName,S1K,0,S1K,S1K/2,0,S1K/2);
      notInGate_h[pos]->Reset();
    }
  
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  
  printf("Program sorts in-gate PID histogram for the CsIArray.\n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  read_master(argv[1],name);
  
  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.root_output_file==1)
    {
      printf("Root output file: %s\n",name->fname.root_output_file);
      m=new TFile(name->fname.root_output_file,"RECREATE");
    }
  else
    {
      printf("ERROR!!! Root output file not defined!\n");
      exit(-1);
    }
  
  if(name->flag.root_gate_file==1)
    {
      printf("Using root gate file: %s\n",name->fname.root_gate_file);
      n=new TFile(name->fname.root_gate_file,"READ");
    }
  else
    {
      printf("ERROR!!! Root gate file not defined!\n");
      exit(-1);
    }
  
  if(name->flag.gate_name_file==1)
    {
      printf("Using gate name file: %s\n",name->fname.gate_name_file);
      gateNameFile=fopen(name->fname.gate_name_file,"r");
      
      while(fscanf(gateNameFile,"%s %s %s",detector,aGateName,pGateName)!=EOF)
	for(pos=1;pos<NCSI;pos++)
	  {
	    det=atoi(detector);
	    if(det==pos)
	      {
		aGate[pos] = (TCutG *) gROOT->FindObject(aGateName);
		pGate[pos] = (TCutG *) gROOT->FindObject(pGateName);
		printf("The gates for detector %d are %s and %s\n",pos,aGateName,pGateName);
		positionGateFlag[pos]++;
	      }}
      fclose(gateNameFile);
    }
  else
    {
      printf("ERROR!!! Gate name file not defined!\n");
      exit(-1);
    }
  
  n->Close();
  
  sort(name);
  
  //theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("PID_TR", "PID_TR",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  
  m->cd();
  for(pos=1;pos<NCSI;pos++)
    if(positionGateFlag[pos]==1)
      {
	sprintf(HistName, "PID_TR_%02d_ap", pos);
	ap_h[pos]->Draw("COLZ");
	ap_h[pos]->Write(HistName);
	
	sprintf(HistName, "PID_TR_%02d_a", pos);
	a_h[pos]->Draw("COLZ");
	a_h[pos]->Write(HistName);
	
	sprintf(HistName, "PID_TR_%02d_p", pos);
	p_h[pos]->Draw("COLZ");
	p_h[pos]->Write(HistName);
	
	sprintf(HistName, "PID_TR_%02d_notInGate", pos);
	notInGate_h[pos]->Draw("COLZ");
	notInGate_h[pos]->Write(HistName);
      }
  m->Close();
  //theApp->Run(kTRUE);
}
