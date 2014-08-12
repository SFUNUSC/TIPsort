#include "sort.h"

int analyze_data(raw_event *data)
{
 unsigned long long int one=1;
  int pos,type;
  double chi,s,f,r,e;


  for(pos=1;pos<NCSI;pos++)
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
		  e=data->csiarray.wfit[pos].am[1];
		  s=data->csiarray.wfit[pos].am[3];
		  f=data->csiarray.wfit[pos].am[2];
		  r=s/f*100;
		  r+=100;

		  if(g->IsInside(e,r))  counts++;

		}
      }
  
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/

int main(int argc, char *argv[])
{

  FILE *cluster;
  input_names_type* name;
  TFile *f;
  char n[132];

  if(argc!=8)
    {
      printf("\n ./CSIARRAY_InPIDGateCluster cluster_file_name idmin idmax chimin chimax root_gate_file gate_name\n");
      exit(-1);
    }

  printf("Program separates events with CsI array type 1 fits\n");
  
  counts=0;
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
 
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  f=new TFile(argv[6],"READ");
  g = (TCutG *) gROOT->FindObject(argv[7]);
  f->Close();
  if((cluster=fopen(argv[1],"r"))==NULL)
	    {
	      printf("\nI can't open input file %s\n",argv[1]);
	      exit(-2);
	    }

 
  while(fscanf(cluster,"%s",n)!=EOF)
    {
      strcpy(name->fname.inp_data,n);
      sort(name);
    }
  fclose(cluster);

 
   
  printf("Number of counts in the gate is %ld\n",counts);
}
