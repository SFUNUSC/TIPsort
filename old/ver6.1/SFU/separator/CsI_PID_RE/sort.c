#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double chi,s,f,r,e;

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
	    e=data->csiarray.wfit[pos].am[1];
	    s=data->csiarray.wfit[pos].am[3];
	    f=data->csiarray.wfit[pos].am[2];
	    r=s/f*100;
	    r+=100;
	    if(g->IsInside(e,r))
	      return SEPARATOR_KEEP;
	  }}
  return SEPARATOR_DISCARD;
}
/*====================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TFile *f;
  
  if(argc!=7)
    {
      printf("Sep_CsI_PID_RE master_file_name idmin idmax chimin chimax det_number\n");
      exit(-1);
    }
  
  printf("Program separates events based on CsI_PID_RE.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  read_master(argv[1],name);

  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  det_id=atoi(argv[6]);

  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.out_data!=1)
    {
      printf("ERROR!!! Output data file not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.root_file==1)
    {
      printf("Using root file: %s\n",name->fname.root_file);
      f=new TFile(name->fname.root_file,"READ");
    }
  else
    {
      printf("ERROR!!! Root file not defined!\n");
      exit(-1);
    }
  
  if(name->flag.root_gate==1)
    {
      printf("Using root gate: %s\n",name->fname.root_gate_name);
      g = (TCutG *) gROOT->FindObject(name->fname.root_gate_name);
    }
  else
    {
      printf("ERROR!!! Root gate not defined!\n");
      exit(-1);
    }
  
  f->Close();
  name->flag.separate=1;
  sort(name); 
}
