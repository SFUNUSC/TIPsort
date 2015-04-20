#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos,type,col,csi;
  double tgr,tcsi,chi;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_GRIFFIN(data,&cal_par->gr,&cev->gr);
  
  if(cev->gr.h.FT>0)
    for(pos=1;pos<NPOSGRIF;pos++)
      if((cev->gr.h.THP&(1<<(pos-1)))!=0)
	if(cev->gr.det[pos].hge.FT>0)
	  for(col=0;col<NCOL;col++)
	    if((cev->gr.det[pos].hge.THP&(1<<col))!=0)
	      if(cev->gr.det[pos].ge[col].h.FT>0)
		if((cev->gr.det[pos].ge[col].h.THP&1)!=0)
		  {
		    //RF already subtracted for Ge timing
		    tgr=cev->gr.det[pos].ge[col].seg[0].T/cal_par->gr.contr_t;
		    
		    for(csi=1;csi<NCSI;csi++)
		      if((data->csiarray.h.THP&(one<<csi))!=0)
			{
			  type=data->csiarray.wfit[csi].type;
			  chi=data->csiarray.wfit[csi].chisq;
			  chi/=data->csiarray.wfit[csi].ndf;
			  if(type>=idmin && type<=idmax)
			    if(chi>=chimin && chi<=chimax)
			      {
				tcsi=16.*data->csiarray.wfit[csi].t[0]; 
				if((data->h.setupHP&RF_BIT)!=0)
				  {
				    tcsi-=data->rf.sin.t0;
				    
				    tcsi+=S4K;
				    tcsi/=16;
				    if(tcsi<0)tcsi=S32K-2;
				    if(tcsi>S32K) tcsi=S32K-3;
				  }
				else
				  tcsi=S32K-4;
				
				if(g->IsInside(tgr,tcsi))
				  return SEPARATOR_KEEP;
				else
				  return SEPARATOR_DISCARD;
			      }}}

  free(cev);
  return SEPARATOR_DISCARD;
}
/*===============================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TFile *f;
  
  if(argc!=6)
    {
      printf("Sep_CsITFitRaw_GriffinTCal master_file_name idmin idmax chimin chimax\n");
      exit(-1);
    }
  
  printf("Program separates events based on Griffin/CsI timing\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);
  
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
  
  if(name->flag.GRIFFIN_cal_par==1)
    {
      printf("GRIFFIN calibration read from: %s\n",name->fname.GRIFFIN_cal_par);
      initialize_GRIFFIN_calibration(&cal_par->gr,name->fname.GRIFFIN_cal_par);
    }
  else
    {
      printf("GRIFFIN calibration parameters not defined\n");
      exit(-1);
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
  

  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  
  name->flag.separate=1;
  
  sort(name); 
}
