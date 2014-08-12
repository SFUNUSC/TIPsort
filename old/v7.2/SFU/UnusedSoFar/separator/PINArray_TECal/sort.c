#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1,none=-1,PINflag=0,kill;
  int pos,col,id;
  double e,t;

  if((data->h.setupHP&PINArray_BIT)==0)
    {
      encode(data,output,enb);
      return SEPARATOR_DISCARD;
    }

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_PINARRAY(data,&cal_par->pinarray,&cev->pinarray);
  
  //if pinarray has at least one position with good calibrated energy and time.
  if(cev->pinarray.h.FH>0)
    //loop through pinarray positions
    for(pos=1;pos<NPIN;pos++)
      //if this position had a good calibrated energy and time
      if((cev->pinarray.h.HHP&(one<<pos))!=0)
	{
	  t=cev->pinarray.pin[pos].T/cal_par->pinarray.contr_t;
	  e=cev->pinarray.pin[pos].E/cal_par->pinarray.contr_e;
	  if(Gate->IsInside(t,e)) PINflag|=(one<<pos);
	}
  
  //now loop back through pinarray positions and drop fragments with PINflag of 0
  for(pos=1;pos<NPIN;pos++)
    //only look at those positions that had a good calibrated energy and time
    if((cev->pinarray.h.HHP&(one<<pos))!=0)
      //if this position is not flagged in PINflag
      if((PINflag&(one<<pos))==0)
	{
	  memset(&data->pinarray.pin[pos],0,sizeof(channel));
	  memset(&data->pinarray.t0[pos],0,sizeof(double));
	  //decriment the folds
	  data->pinarray.h.Efold--;
	  data->pinarray.h.Tfold--;
	  data->pinarray.h.TSfold--;
	  //this gives a binary hit pattern of all 1's except in position 'pos'
	  kill=none-(one<<pos);
	  //the action of 'kill' hit pattern on the existing patterns will remove 'pos'
	  data->pinarray.h.TSHP&=kill;
	  data->pinarray.h.EHP&=kill;
	  data->pinarray.h.THP&=kill;
	}
  //remove PINArray_BIT if the only pos in the event was removed above.
  if(data->pinarray.h.TSfold<=0)
    {
      return SEPARATOR_DISCARD;
      /* kill=none-PINArray_BIT; */
      /* data->h.setupHP&=kill; */
      /* memset(&data->pinarray,0,sizeof(PINArray)); */

      /* //If no pinarray positions were in the prompt gate, then the  */
      /* //gamma ray in the prompt gate is false. Need to remove this. */
      /* if(cev->tg.h.FH>0) */
      /* 	for(pos=1;pos<NPOSTIGR;pos++) */
      /* 	  { */
      /* 	    if(((cev->tg.h.HHP&(one<<(pos-1)))!=0) && (cev->tg.det[pos].hge.FH>0)) */
      /* 	      for(col=0;col<NCOL;col++) */
      /* 		if(((cev->tg.det[pos].hge.HHP&(one<<col))!=0) && (cev->tg.det[pos].ge[col].h.FH>0)) */
      /* 		  if((cev->tg.det[pos].ge[col].h.HHP&one)!=0) */
      /* 		    { */
      /* 		      memset(&data->tg.det[pos].ge[col].h,0,sizeof(header)); */
      /* 		      for(int seg=0;seg<NSEGTIGR;seg++) */
      /* 			{ */
      /* 			  memset(&data->tg.det[pos].ge[col].seg[seg],0,sizeof(channel)); */
      /* 			  memset(&data->tg.det[pos].ge[col].t0[seg],0,sizeof(header)); */
      /* 			} */
      /* 		      kill=none-(one<<col); */
      /* 		      data->tg.det[pos].h.GeHP&=kill; data->tg.det[pos].h.Gefold--; */
		      
      /* 		      id=(pos-1)*NCOL+col; */
      /* 		      kill=none-(one<<id); */
      /* 		      data->tg.g.GeHP&=kill; data->tg.g.Gefold--; */
      /* 		      data->tg.g.THP&=kill; data->tg.g.Tfold--; */
		      
      /* 		      id=pos-1; */
      /* 		      kill=none-(one<<id); */
      /* 		      if(data->tg.det[pos].h.Gefold==0) */
      /* 			{ */
      /* 			  data->tg.h.GeHP&=kill; data->tg.h.Gefold--; */
      /* 			  data->tg.g.PosHP&=kill; data->tg.g.Posfold--; */
      /* 			}} */
	    
      /* 	    if(data->tg.g.Posfold==0) */
      /* 	      { */
      /* 		kill=none-TIGRESS_BIT; */
      /* 		data->h.setupHP&=kill; */
      /* 		memset(&data->tg,0,sizeof(Tigress)); */
      /* 	      }}} */
    }

  //encoding the new, shortened, event
  encode(data,output,enb);
  return SEPARATOR_DISCARD;
}
/*=================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TFile *f;
  FILE *gateNameFile;
  char gateName[132];
  
  if(argc!=2)
    {
      printf("Sep_PINArrayTECal master_file_name\n");
      exit(-1);
    }
  
  printf("Program separates events based on PINArray_TECal.\n");
  
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
  
  if(name->flag.PINARRAY_cal_par==1)
    {
      printf("PINArray calibration read from %s\n",name->fname.PINARRAY_cal_par);
      initialize_PINARRAY_calibration(&cal_par->pinarray,name->fname.PINARRAY_cal_par);
    }
  else
    {
      printf("ERROR!!! PINArray calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }

  if(name->flag.root_gate_file==1)
    {
      printf("Using root gate file: %s\n",name->fname.root_gate_file);
      f=new TFile(name->fname.root_gate_file,"READ");
    }
  else
    {
      printf("ERROR!!! Root gate file not defined!\n");
      exit(-1);
    }
  
  if(name->flag.gate_name_file==1)
    {
      gateNameFile=fopen(name->fname.gate_name_file,"r");
      printf("Using gate name file: %s\n",name->fname.gate_name_file);

      fscanf(gateNameFile,"%s",gateName);
      Gate = (TCutG *) gROOT->FindObject(gateName);
      fclose(gateNameFile);
    }
  else
    {
      printf("ERROR!!! Root gate not defined!\n");
      exit(-1);
    }
  f->Close();

  if((output=fopen(name->fname.out_data,"w"))==NULL)
    {
      printf("ERROR!!! Cannot open output file %s!\n",name->fname.out_data);
      exit(-2);
    }
  else
    {
      printf("Separated data written to file %s\n",name->fname.out_data);
      memset(enb,0,sizeof(enb));
      enb[0]=BUFFER_TAG;
      enb[1]++;
      enb[1]++;
    }
    
  //We never "SEPARATOR_KEEP", just encode so this isn't needed.
  name->flag.separate=0;
  sort(name); 
  //save the last buffer
  fwrite(enb,sizeof(int),BUFFSIZE,output);
}
