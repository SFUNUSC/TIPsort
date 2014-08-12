#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int pos,type;
  double chi,s,f,r,t;

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
	    t=16.*data->csiarray.wfit[pos].t[0]; 
	    if((data->h.setupHP&RF_BIT)!=0)
	      {
		t-=data->rf.sin.t0;
		
		t+=S8K;
		if(t<0)t=S32K-2;
		if(t>S32K) t=S32K-3;
	      }
	    else
	      t=S32K-4;
	    t/=16;
	    s=data->csiarray.wfit[pos].am[3];
	    f=data->csiarray.wfit[pos].am[2];
	    r=s/f*100;
	    r+=100;
	    if(g->IsInside(t,r))
	      return SEPARATOR_KEEP;
	    else
	      return SEPARATOR_DISCARD;
	  }}
  return SEPARATOR_DISCARD;
}
/*=======================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TFile *f;
  
  if(argc!=7)
    {
      printf("Sep_CsI_PID_RT master_file_name idmin idmax chimin chimax det_number\n");
      exit(-1);
    }
  
  printf("Program separates events based on CsI_PID_RT.\n");
  
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
