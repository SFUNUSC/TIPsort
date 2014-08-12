#include "sort.h"
double get_tdcfd(ShapePar *p)
{
  double t,dt;
  par.td=del;
  par.t0=p->t[0];
  par.t1=p->t[2];
  par.t2=p->t[3];
  par.p1=p->am[2];
  par.p2=p->am[3];
  par.l1=1./par.t1;
  par.l2=1./par.t2;
  par.e1=exp(par.td/par.t1);
  par.e2=exp(par.td/par.t2);
  par.r=par.p1/(par.p1+par.p2);
  par.z=(par.l1*par.e1*(1.-par.e2)-par.l2*par.e2*(1.-par.e1))/(par.l1*(1.-par.e2)-par.l2*(1.-par.e1));
  par.q1=(par.e1-par.z)/(1-par.z);
  par.q2=(par.e2-par.z)/(1-par.z);
  par.a1=-par.r*par.q1;
  par.a2=-(1-par.r)*par.q2;
  
  /* printf("t1 %10.6f t2 %10.6f\n",par.t1,par.t2); */
  /* printf("p1 %10.6f p2 %10.6f\n",par.p1,par.p2); */
  /* printf("e1 %10.6f e2 %10.6f\n",par.e1,par.e2); */
  /* printf(" z %10.6f  r %10.6f\n",par.z,par.r); */
  /* printf("q1 %10.6f q2 %10.6f\n",par.q1,par.q2); */
  /* printf("a1 %10.6f a2 %10.6f\n",par.a1,par.a2); */
  
  t=f(0.);
  dt=1000;
  while(dt>0.1)
    {
      dt=-f(t)/fp(t);
      t+=dt;
      //printf("t %10.6f dt %10.6f f %10.6f\n",t,dt,f(t));
    }
  //  getc(stdin);
  return par.t0+t;

}
int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double t,chi;

 

  if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;    

 if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;    

  for(pos=1;pos<NCSI;pos++)
    if((data->csiarray.h.THP&(one<<pos))!=0)
      {
	type=data->csiarray.wfit[pos].type;
	chi=data->csiarray.wfit[pos].chisq;
	chi/=data->csiarray.wfit[pos].ndf;
	if(type==2)
	    if(chi>=chimin)
	      if(chi<=chimax)
		{
		  t=16*get_tdcfd(&data->csiarray.wfit[pos]);
		  t-=data->rf.sin.t0;
		  
		  t+=S4K;
		  t/=16;
		  if(t<0)t=S32K-4;
		  if(t>S32K) t=S32K-3;
   
		  hist[pos][(int)rint(t)]++;
		  h->Fill(t,pos);
		  
		}
      }
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;


  if(argc!=5)
    {
      printf("\n ./CSIARRAY_TDCFD sfu_input_data_file_name del chimin chimax\n");
      exit(-1);
    }
  h = new TH2D("Raw Time","Raw Time",S1K,0,S1K-1,NCSI+1,0,NCSI);
  h->Reset();
  del=atof(argv[2]);
  chimin=atof(argv[3]);
  chimax=atof(argv[4]);
  printf("Program sorts digital CFD histograms for the CSIARRAY \n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);

  if((output=fopen("CSIArray_TDCFD.mca","w"))==NULL)
    {
      printf("\nI can't open file %s\n","CSIArray_TDCFD.mca");
      exit(EXIT_FAILURE);
    }
  for(int pos=0;pos<NCSI;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);

  fclose(output);
 theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Digital CFD", "Digital CFD",10,10, 500, 300);
  //  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  h->Draw("COLZ");
  
  theApp->Run(kTRUE);

}

