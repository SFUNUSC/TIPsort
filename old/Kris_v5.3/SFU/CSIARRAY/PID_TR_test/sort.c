#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double chi,s,f,r,t;


  pos=det_id;
    if((data->csiarray.h.THP&(one<<pos))!=0)
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
		  if((data->h.setupHP&RF_BIT)!=0)
		    {
		      t-=data->rf.sin.t0;
		      
		      t+=S4K;
		      t/=16;
		      if(t<0)t=S32K-2;
		      if(t>S32K) t=S32K-3;
		    }
		  else
		    t=S32K-4;
		 
		  s=data->csiarray.wfit[pos].am[3];
		  f=data->csiarray.wfit[pos].am[2];
		  r=s/f*100;
		  //	  	  printf("%f %f\n",t,r);
		  h->Fill(t,100+r);
		  g->Fill(100+r,t);
		  if(r>S32K-4) r=S32K-4;
		  hist[(int)rint(r)]++;
		}
      }
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  TFile* f;

  if(argc!=7)
    {
      printf("\n ./CSIARRAY_PID_TR_test sfu_input_data_file_name idmin idmax chimin chimax det_id\n");
      exit(-1);
    }
  h = new TH2D("PID_a","PID_a",S1K,0,S1K,S1K/2,0,S1K/2);
  h->Reset();

  g = new TH2D("PID_b","PID_b",S1K/2,0,S1K/2,S1K,0,S1K);
  g->Reset();
 
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  det_id=atoi(argv[6]);
  printf("Program sorts PID histogram for the CSIARRAY \n");

  memset(hist,0,sizeof(hist));
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);

  if((output=fopen("CSIArray_PID_ratio.mca","w"))==NULL)
    {
      printf("\nI can't open file %s\n","CSIArray_PID_ratio.mca");
      exit(EXIT_FAILURE);
    }
 
    fwrite(hist,S32K*sizeof(int),1,output);

  fclose(output);
 
  f=new TFile("test.root","RECREATE");
  f->cd();
  h->Write();
  g->Write();
  f->Close();
}

