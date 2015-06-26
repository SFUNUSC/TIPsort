#include "sort.h"
#include <iostream>
using namespace std;

int analyze_data(raw_event *data)
{
  int pos,col,tttt;
  double t,tt,ttt;

  if(data->tg.h.Gefold>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((data->tg.h.GeHP&(1<<(pos-1)))!=0)
	if(data->tg.det[pos].h.Gefold>0)
	  for(col=0;col<NCOL;col++)
	    if((data->tg.det[pos].h.GeHP&(1<<col))!=0)
	      if(data->tg.det[pos].ge[col].h.Tfold>0)
		if((data->tg.det[pos].ge[col].h.THP&1)!=0)
		  {
		    //if(pos==1 && col==0)
		    if(pos!=-1 && col!=5)
		      {
			//The bitmask is redundant here since it has already
			//been applied in tig-format.c. I checked by generating output
			//with the mask in place and removed. "t" didn't change.
			t=data->tg.det[pos].ge[col].seg[0].cfd&0x00ffffff;
			//tt=t;
			//printf("The time is %f \n",t);
			//Multiplying by 16 in decimal is like a left bitshift by 1 in hex
			//truly, there is no such thing, but is shifts and adds 0 to the right
			//so what this is really the same as multiplying by 10 in decimal
			t-=(data->tg.det[pos].ge[col].seg[0].timestamp*16)&0x00ffffff;
			tt=t;
			if((data->h.setupHP&RF_BIT)!=0)
			  {
			    ttt=data->rf.sin.t0;
			    ttt*=0.625;
			    g->Fill(ttt);
			    //Code for unwrapping RF
			    //if(ttt<(84.841*0.25)) ttt+=(84.841*0.75);
			    //else ttt-=(84.841*.25);
			    //t-=(int)data->rf.sin.t0;
			    t-=data->rf.sin.t0;
			    t*=0.625;
			    t+=S16K;
			    tt*=0.625;
			    tt+=S16K;
			    if(t<0)t=S32K-2;
			    if(t>S32K) t=S32K-3;
			  }
			else
			  t=S32K-4;
			if(t>=0 && t<S32K)
			  {
			    hist[pos][col][(int)rint(t)]++;
			    //tttt=(int)tt+S16K;
			    if(tttt>0 && tttt<S32K)
			      hist[pos][col+1][tttt]++;
			    h->Fill(t);
			    k->Fill(tt);
			    //h->Fill(ttt,tt);
			    //h->Fill(ttt,data->h.setupHP);
			  }}
		  }
  return SEPARATOR_DISCARD;
}
/*==========================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  char n[132];
  int pos,col;
  TApplication *theApp;

  //h = new TH2D("","",720,-20,120,100,0,99);
  h = new TH1D("","",30000,0,30000);
  k = new TH1D("","",30000,0,30000);
  //h = new TH2D("","",720,-20,120,20000,-10000,10000);
  //h = new TH1D("","",20000,0,20000);
  h->Reset();
  k->Reset();
  g = new TH1D("","",720,-20,120);
  g->Reset();

  if(argc!=2)
    {
      printf("Tigress_TRaw sfu_input_data_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts raw time histogram for Tigress cores \n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  memset(hist,0,sizeof(hist));
  
  sort(name);
  
  for(pos=1;pos<NPOSTIGR;pos++)
    {
      sprintf(n,"pos%1d%1d_raw_core_time.mca",pos/10,pos-(pos/10)*10);
      if((output=fopen(n,"w"))==NULL)
	{
	  printf("\nI can't open file %s\n",n);
	  exit(EXIT_FAILURE);
	}
      
      for(col=0;col<NCOL;col++)
	fwrite(hist[pos][col],S32K*sizeof(int),1,output);
	      
      fclose(output);
    }

  theApp=new TApplication("App", &argc, argv);
  TCanvas *c1 = new TCanvas("TRaw", "TRaw",100,100, 1000, 600);
  gStyle->SetPalette(1);
  //gPad->SetLogz(1);
  TPad *c1_1 = new TPad("c1_1","c1_1",0,0,1,0.5);
  c1_1->Draw();
  c1_1->cd();
  h->Draw();
  //black
  h->SetLineColor(1);
  k->Draw("same");
  //red
  k->SetLineColor(2);
  c1_1->Modified();
  c1->cd();
  TPad *c1_2 = new TPad("c1_2","c1_2",0,0.5,1,1);
  c1_2->Draw();
  c1_2->cd();
  g->Draw();
  c1_2->Modified();
  c1->cd();
  theApp->Run(kTRUE);
}

