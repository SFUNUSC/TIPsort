#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos,col,id,take,take_this;
  double e,t;
  long long int one=1,none=-1,kill;
  
  if((data->h.setupHP&TIGRESS_BIT)==0)
    {
      encode(data,output,enb);
      return SEPARATOR_DISCARD;
    }
  
  if((data->h.setupHP&RF_BIT)==0)
    {
      encode(data,output,enb);
      return SEPARATOR_DISCARD;
    }
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  take=0;
  if(cev->tg.h.FH>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.HHP&(one<<(pos-1)))!=0)
  	if(cev->tg.det[pos].hge.FH>0)
  	  for(col=0;col<NCOL;col++)
  	    if((cev->tg.det[pos].hge.HHP&(one<<col))!=0)
  	      if(cev->tg.det[pos].ge[col].h.FH>0)
  		if((cev->tg.det[pos].ge[col].h.HHP&one)!=0)
  		  {
		    take_this=0;
		    e=cev->tg.det[pos].ge[col].seg[0].E/cal_par->tg.contr_e;
		    t=data->tg.det[pos].ge[col].t0[0]*16;	  
		    if(t>0.)
		      {
			t-=data->rf.sin.t0;
			t*=0.625;
			t+=512;
		      }
		    else t=512-4;			
		    
		    if(g->IsInside(t,e))
		      {
			take_this=1;
			take=1;
		      }
		    
		    if(take_this==0)
		      {			  
			memset(&data->tg.det[pos].ge[col].h,0,sizeof(header));
			for(int seg=0;seg<NSEGTIGR;seg++)
			  {
			    memset(&data->tg.det[pos].ge[col].seg[seg],0,sizeof(channel));
			    memset(&data->tg.det[pos].ge[col].t0[seg],0,sizeof(header));
			  }
			
			kill=none-(one<<col);
			data->tg.det[pos].h.GeHP&=kill; data->tg.det[pos].h.Gefold--;
			
			id=(pos-1)*NCOL+col;
			kill=none-(one<<id);
			data->tg.g.GeHP&=kill; data->tg.g.Gefold--;
			data->tg.g.THP&=kill; data->tg.g.Tfold--;			  
			
			id=pos-1;
			kill=none-(one<<id);			  
			if(data->tg.det[pos].h.Gefold==0)
			  {								
			    data->tg.h.GeHP&=kill; data->tg.h.Gefold--;
			    data->tg.g.PosHP&=kill; data->tg.g.Posfold--;
			  }}}
  
  if(data->tg.g.Posfold==0)
    {
      kill=none-TIGRESS_BIT;
      data->h.setupHP&=kill;
      memset(&data->tg,0,sizeof(Tigress));
    }
  
  free(cev);
  encode(data,output,enb);
  return SEPARATOR_DISCARD;
}
/*=========================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TFile *f;
  FILE *gateNameFile;
  char gateName[132];

  if(argc!=2)
    {
      printf("Sep_Tigress_ECalTFitRaw master_file_name\n");
      exit(-1);
    }
  
  printf("Program separates events based on Tigress ECalTFitRaw.\n");
  
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
  
  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calibration read from %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR!!! TIGRESS calibration parameters not defined!\n");
      exit(-1);
    }
  
  if(name->flag.root_gate_file==1)
    {
      printf("Using root gate file %s\n",name->fname.root_gate_file);
      f=new TFile(name->fname.root_gate_file,"READ");
    }
  else
    {
      printf("ERROR!!! Root file not defined!\n");
      exit(-1);
    }
  
  if(name->flag.gate_name_file==1)
    {
      gateNameFile=fopen(name->fname.gate_name_file,"r");
      printf("Using gate name file: %s\n",name->fname.gate_name_file);
      
      fscanf(gateNameFile,"%s",gateName);
      g = (TCutG *) gROOT->FindObject(gateName);
      fclose(gateNameFile);
    }
  else
    {
      printf("ERROR!!! Root gate not defined!\n");
      exit(-1);
    }

  f->Close();
  name->flag.separate=0;
  
  if((output=fopen(name->fname.out_data,"w"))==NULL)
    {
      printf("ERROR!!! I can't open the output file for writing!\n");
      exit(-2);
    }
  memset(enb,0,sizeof(enb));
  enb[0]=BUFFER_TAG;
  enb[1]++;
  enb[1]++;
  
  sort(name); 
  // save the last buffer
  fwrite(enb,sizeof(int),BUFFSIZE,output);
}
