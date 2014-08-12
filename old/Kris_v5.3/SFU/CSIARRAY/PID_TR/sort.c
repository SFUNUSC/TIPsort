#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double chi,s,f,r,t;

  if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;

  if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;

  for(pos=1;pos<NCSI;pos++)
    if((data->csiarray.h.TSHP&(one<<pos))!=0)
      {
	type=data->csiarray.wfit[pos].type;
	chi=data->csiarray.wfit[pos].chisq;
	chi/=data->csiarray.wfit[pos].ndf;
	if(type>=idmin)
	  if(type<=idmax)
	    if(chi>=chimin)
	      if(chi<=chimax)
		{
		  t=16.*data->csiarray.wfit[pos].t[0]; 
		  t-=data->rf.sin.t0;		      
		  t+=S4K;
		  t/=16;
		  s=data->csiarray.wfit[pos].am[3];
		  f=data->csiarray.wfit[pos].am[2];
		  r=s/f*100;
		  h[pos]->Fill(t,100+r);
		}
      }

  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{

  input_names_type* name;
  Int_t pos;
  TFile *out;
  char id[132];
  if(argc!=6)
    {
      printf("\n ./CSIARRAY_PID_TR sfu_input_data_file_name idmin idmax chimin chimax\n");
      exit(-1);
    }

  for(pos=1;pos<NCSI;pos++)
    {
      sprintf(id,"PID_TR_%2d",pos);
      h[pos] = new TH2D(id,id,S1K,0,S1K,S1K/2,0,S1K/2);
      h[pos]->Reset();
    }

  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  
  printf("Program sorts PID histogram for the CSIARRAY \n");

 
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);

  out=new TFile("PID_TR.root","RECREATE");
  out->cd();
  for(pos=1;pos<NCSI;pos++)
    {
      h[pos]->Write();
    }
  out->Close();
}

