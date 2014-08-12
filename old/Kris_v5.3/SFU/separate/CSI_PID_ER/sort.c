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
		  if(g->IsInside(e,r))
		    return SEPARATOR_KEEP;

		}
      }

  return SEPARATOR_DISCARD;
}
/*====================================================================================*/

int main(int argc, char *argv[])
{

  input_names_type* name;
  TFile *f;

  if(argc!=9)
    {
      printf("\n ./separate_CSI_PID_ER sfu_input_data_file_name sfu_output_data_file_name idmin idmax chimin chimax root_gate_file gate_name\n");
      exit(-1);
    }

  printf("Program separates events with CsI array type 1 fits\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  strcpy(name->fname.out_data,argv[2]);
  idmin=atoi(argv[3]);
  idmax=atoi(argv[4]);
  chimin=atof(argv[5]);
  chimax=atof(argv[6]);
  f=new TFile(argv[7],"READ");
  g = (TCutG *) gROOT->FindObject(argv[8]);
  f->Close();
  name->flag.separate=1;

  sort(name); 
   
}
