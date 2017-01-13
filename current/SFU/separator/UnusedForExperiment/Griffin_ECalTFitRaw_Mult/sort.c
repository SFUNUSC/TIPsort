#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos,col,id,take=0;
  double e,t;
  long long int one=1,none=-1,kill;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_GRIFFIN(data,&cal_par->gr,&cev->gr);
  
  if((data->h.setupHP&GRIFFIN_BIT)==0)
    {
      encode(data,output[i],enb);
      return SEPARATOR_DISCARD;
    }
  
  if((data->h.setupHP&RF_BIT)==0)
    {
      encode(data,output[i],enb);
      return SEPARATOR_DISCARD;
    }
  
  if(cev->gr.h.FE>0)
    for(pos=1;pos<NPOSGRIF;pos++)
      if((cev->gr.h.EHP&(one<<(pos-1)))!=0)
	if(cev->gr.det[pos].hge.FE>0)
	  for(col=0;col<NCOL;col++)
	    if((cev->gr.det[pos].hge.EHP&(one<<col))!=0)
	      if(cev->gr.det[pos].ge[col].h.FE>0)
		if((cev->gr.det[pos].ge[col].h.EHP&one)!=0)
		  {
		    take=0;  		  
		    e=cev->gr.det[pos].ge[col].seg[0].E/cal_par->gr.contr_e;
		    t=data->gr.det[pos].ge[col].t0[0]*16;	  
		    if(t>0.)
		      {
			t-=data->rf.sin.t0;
			t/=1.6;
			t+=512;
		      }
		    else t=512-4;			
		    
		    if(Gate[i]->IsInside(t,e)) take=1;
		    
		    if(take==0)
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
			  }}}
  
  if(data->gr.g.Posfold==0)
    {
      kill=none-GRIFFIN_BIT;
      data->h.setupHP&=kill;
      memset(&data->gr,0,sizeof(Griffin));
    }

  encode(data,output[i],enb);
  free(cev);
  return SEPARATOR_DISCARD; 
}
/*=========================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TFile *f;
  FILE *gateNameFile;
  char GateName[132],GateNumber[132];
  char OutputFile[256];
  int num=0;
  
  if(argc!=2)
    {
      printf("Sep_Griffin_ECalTFitRaw master_file_name\n");
      exit(-1);
    }
  
  printf("Program separates events based on Griffin ECalTFitRaw.\n");
  
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
  
  if(name->flag.GRIFFIN_cal_par==1)
    {
      printf("GRIFFIN calibration read from %s\n",name->fname.GRIFFIN_cal_par);
      initialize_GRIFFIN_calibration(&cal_par->gr,name->fname.GRIFFIN_cal_par);
    }
  else
    {
      printf("ERROR!!! GRIFFIN calibration parameters not defined!\n");
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
      printf("Using gate name file: %s\n",name->fname.gate_name_file);
      gateNameFile=fopen(name->fname.gate_name_file,"r");
      
      while(fscanf(gateNameFile,"%s %s",GateNumber,GateName)!=EOF)
	{
	  num=atoi(GateNumber);
	  Gate[num] = (TCutG *) gROOT->FindObject(GateName);
	  printf("Gate number %d is %s.\n",num,GateName);
	  if(num>numMax) numMax=num;
	}
      fclose(gateNameFile);
    }
  else
    {
      printf("ERROR!!! Root gate not defined!\n");
      exit(-1);
    }
  
  f->Close();
  name->flag.separate=0;
  
  if(name->flag.out_data_list==1)
    {
      out=fopen(name->fname.out_data_list,"r");
      while(fscanf(out,"%s %s",GateNumber,OutputFile)!=EOF)
	{
	  num=atoi(GateNumber);
	  output[num]=fopen(OutputFile,"w");
	  printf("The output file for gate number %d is:\n%s\n",num,OutputFile);
	}
      fclose(out);
    }
  else
    {
      printf("ERROR!!! List file for output files cannot be read!\n");
      exit(EXIT_FAILURE);
    }
  
  memset(enb,0,sizeof(enb));
  enb[0]=BUFFER_TAG;
  enb[1]++;
  enb[1]++;
  
  for(i=1;i<=numMax;i++)
    {
      sort(name); 
      
			//save the last buffer which will be dropped otherwise
			//if enb[1]==2 then the buffer contains no data, only the header
			if(enb[1]>2)
				fwrite(enb,sizeof(int),BUFFSIZE,output);
    }}
