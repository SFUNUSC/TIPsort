#include "sort.h"

int analyze_data(raw_event *data)
{

  cal_event* cev;
  int pos,col;
  double e,t;
  long long int one=1;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_GRIFFIN(data,&cal_par->gr,&cev->gr);
  if(cev->gr.h.FH>0)
    for(pos=1;pos<NPOSGRIF;pos++)
      if((cev->gr.h.HHP&(one<<(pos-1)))!=0)
  	if(cev->gr.det[pos].hge.FH>0)
  	  for(col=0;col<NCOL;col++)
  	    if((cev->gr.det[pos].hge.HHP&(one<<col))!=0)
  	      if(cev->gr.det[pos].ge[col].h.FH>0)
  		if((cev->gr.det[pos].ge[col].h.HHP&1)!=0)
  		  {
		    t=cev->gr.det[pos].ge[col].seg[0].T/cal_par->gr.contr_t;
  		    e=cev->gr.det[pos].ge[col].seg[0].E/cal_par->gr.contr_e;

		    if(g->IsInside(t,e))
		      return SEPARATOR_KEEP;
		    else
		      return SEPARATOR_DISCARD;

			    
  		  }
  free(cev);
 


  return SEPARATOR_DISCARD;
}
/*====================================================================================*/

int main(int argc, char *argv[])
{

  input_names_type* name;
  TFile *f;

  if(argc!=2)
    {
      printf("\n ./separate_Griffin_TECal master_file_name\n");
      exit(-1);
    }

  printf("Program separates events based on Griffin RF timing\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

  read_master(argv[1],name);

  if(name->flag.inp_data!=1)
    {
      printf("\nInput data file not defined\n");
      exit(EXIT_FAILURE);
    }

 if(name->flag.out_data!=1)
    {
      printf("\nOutput data file not defined\n");
      exit(EXIT_FAILURE);
    }

  if(name->flag.GRIFFIN_cal_par==1)
        {
          printf("\nGRIFFIN calibration read from the file:\n %s\n",name->fname.GRIFFIN_cal_par);
          initialize_GRIFFIN_calibration(&cal_par->gr,name->fname.GRIFFIN_cal_par);
	  
        }
      else
        {
          printf("\nGRIFFIN calibration parameters not defined\n");
          exit(-1);
        }

 
  if(name->flag.root_file==1)
        {
          printf("\nUsing root file:\n %s\n",name->fname.root_file);
	  f=new TFile(name->fname.root_file,"READ");
	  
        }
      else
        {
          printf("\nRoot file not defined\n");
          exit(-1);
        }

  if(name->flag.root_gate==1)
    {
      printf("\nUsing root gate:\n %s\n",name->fname.root_gate_name);
      g = (TCutG *) gROOT->FindObject(name->fname.root_gate_name);
	  
        }
      else
        {
          printf("\nRoot gate not defined\n");
          exit(-1);
        }

  f->Close();

 
 
  name->flag.separate=1;

  sort(name); 
   
}
