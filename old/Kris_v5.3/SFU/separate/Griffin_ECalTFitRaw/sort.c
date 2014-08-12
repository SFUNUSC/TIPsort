#include "sort.h"

int analyze_data(raw_event *data)
{

  cal_event* cev;
  int pos,col;
  double e,t;
  long long int one=1,none=-1,kill;
  int id;
  int take,take_this;

  if((data->h.setupHP&GRIFFIN_BIT)==0)
    {
      encode(data,output,enb);
      return SEPARATOR_DISCARD;
    }

  if((data->h.setupHP&RF_BIT)==0)
    {
      encode(data,output,enb);
      return SEPARATOR_DISCARD;
    }
  //  printf("*****************************************************************************************\n");
  // display_raw_event(data);  

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_GRIFFIN(data,&cal_par->gr,&cev->gr);
 

 
  take=0;
  if(cev->gr.h.FE>0)
    for(pos=1;pos<NPOSGRIF;pos++)
      if((cev->gr.h.EHP&(one<<(pos-1)))!=0)
  	if(cev->gr.det[pos].hge.FE>0)
  	  for(col=0;col<NCOL;col++)
  	    if((cev->gr.det[pos].hge.EHP&(one<<col))!=0)
  	      if(cev->gr.det[pos].ge[col].h.FE>0)
  		if((cev->gr.det[pos].ge[col].h.EHP&one)!=0)
  		  {
		    e=cev->gr.det[pos].ge[col].seg[0].E/cal_par->gr.contr_e;
		    t=data->gr.det[pos].ge[col].t0[0];	  
		    take_this=0;
		    if(t>0.)
		      {
			t*=16;
			t-=data->rf.sin.t0;
			t/=16;
			t+=512;
		      }
		    else
		      t=128;			
			
		    if(g->IsInside(t,e))
		      {
			take_this=1;
			take=1;
		      }
		      
	
		    if(take_this==0)
		      {			  
			memset(&data->gr.det[pos].ge[col].h,0,sizeof(header));
			for(int seg=0;seg<NSEGGRIF;seg++)
			  {
			    memset(&data->gr.det[pos].ge[col].seg[seg],0,sizeof(channel));
			    memset(&data->gr.det[pos].ge[col].t0[seg],0,sizeof(header));
			  }
			
			kill=none-(one<<col);
			data->gr.det[pos].h.GeHP&=kill;
			data->gr.det[pos].h.Gefold--;
			id=(pos-1)*NCOL+col;
			kill=none-(one<<id);
			data->gr.g.GeHP&=kill;
			data->gr.g.Gefold--;
			data->gr.g.THP&=kill;
			data->gr.g.Tfold--;			  
			    
			id=pos-1;
			kill=none-(one<<id);			  
			if(data->gr.det[pos].h.Gefold==0)
			  {								
			    data->gr.h.GeHP&=kill;
			    data->gr.h.Gefold--;
			    data->gr.g.PosHP&=kill;
			    data->gr.g.Posfold--;
			  }	      
		      }
		  }

    
    
    if(data->gr.g.Posfold==0)
      {
	kill=none-GRIFFIN_BIT;
	data->h.setupHP&=kill;
	memset(&data->gr,0,sizeof(Griffin));
      }

    
    free(cev);
 
  
    encode(data,output,enb);

 
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

 
  if(name->flag.root_gate_file==1)
        {
          printf("\nUsing root file:\n %s\n",name->fname.root_gate_file);
	  f=new TFile(name->fname.root_gate_file,"READ");
	  
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

 
 
  name->flag.separate=0;

  if((output=fopen(name->fname.out_data,"w"))==NULL)
    {
      printf("\nI can't open output file %s for writing\n",name->fname.out_data);
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
