//This program keeps events of which TIGRESS_time-PIN_time falls within [low, high]
//Author: Frank Wu
//Date: May 3, 2017
////////////////////////////////////////////////////////////////////////////////////////////
#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos = 0;
  int col = 0;
  double ttg = 0;
  double tpin = 0;
  double tdiff = 0;
  
  long long int one=1,none=-1,kill = 0;
  int id = 0,id_ge = 0;
  long long int flag_ge = 0,flag_pin = 0,drop = 0;
  //int    flag_pos;
  
  double tmin=10E10;
  //double tmin_ttg=10E10;
  double tmax=-10E10;
    
  if((data->h.setupHP&TIGRESS_BIT)==0)
    return SEPARATOR_DISCARD;
  
  if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;
  
  if((data->h.setupHP&PINArray_BIT)==0)
    return SEPARATOR_DISCARD;

  cev = (cal_event*)malloc(sizeof(cal_event));
  memset(cev, 0, sizeof(cal_event));
  calibrate_TIGRESS(data, &cal_par->tg, &cev->tg);
  calibrate_PINARRAY(data, &cal_par->pinarray, &cev->pinarray);
  
  flag_ge=0;
  flag_pin=0;
  
  if(cev->tg.h.FT>0)
    if(cev->pinarray.h.FT>0)
      {
	//Extract min time of PIN here
	for(int pin=1;pin<NPIN;pin++)
	  if((cev->pinarray.h.THP&(one<<pin))!=0)
	    {
	      tpin=cev->pinarray.pin[pin].T/cal_par->pinarray.contr_t;
	      
	      if(tpin<tmin)
		{
		  tmin=tpin;
		}
	    }
	//Extract max time of TIGRESS here
	for(pos=1;pos<NPOSTIGR;pos++)
	  if((cev->tg.h.THP&(1<<(pos-1)))!=0)
	    if(cev->tg.det[pos].hge.FT>0)
	      for(col=0;col<NCOL;col++)
		if((cev->tg.det[pos].hge.THP&(1<<col))!=0) //here May1
		  if(cev->tg.det[pos].ge[col].h.FT>0)
		    if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
		      {
			//get TIGRESS time
			ttg=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;
			
			if(ttg>tmax)
			  tmax=ttg;
		      }
	//Now that tmax and tmin are set, find good TIGRESS events
	for(pos=1;pos<NPOSTIGR;pos++)
	  if((cev->tg.h.THP&(1<<(pos-1)))!=0)
	    if(cev->tg.det[pos].hge.FT>0)
	      for(col=0;col<NCOL;col++)
		if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
		  if(cev->tg.det[pos].ge[col].h.FT>0)
		    if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
		      {
			//compare each TIGRESS time with tmin (tmin is from PIN)
			ttg=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;
			tdiff=ttg - tmin;
                        h->Fill(tdiff);
			//keep good TIGRESS events:
			if(tdiff<=high)
			  if(tdiff>=low)
			    {
			      g->Fill(tdiff);
			      id=pos-1;
			      id_ge=id*NCOL+col;
			      flag_ge|=(one<<id_ge);
			    }
		      }
	//keep good PIN events
	for(int pin=1;pin<NPIN;pin++)
	  if((cev->pinarray.h.THP&(one<<pin))!=0)
	    {
	      //compare each PIN time with tmax (set by TIGRESS)
	      tpin=cev->pinarray.pin[pin].T/cal_par->pinarray.contr_t;	      
	      tdiff=tmax - tpin;
	      //Keep good PIN events:
	      if(tdiff<=high)
		if(tdiff >= low)
		  {
		    flag_pin|=(one<<pin);
		  }
	    }
	
	free(cev);
      }
//drop TIGRESS data out of the time limits
  for(pos=1;pos<NPOSTIGR;pos++)
    {
      id=pos-1;
      for(col=0;col<NCOL;col++)
	{	
	  id_ge=id*NCOL+col;
	  drop=(one<<id_ge);
	  drop&=data->tg.g.GeHP;
	  if(drop!=0)
	    {
	      drop&=flag_ge;
	      if(drop==0)
		{
		  //drop this crystal
		  memset(&data->tg.det[pos].ge[col],0,sizeof(SegTIGR));
		  kill=none-(one<<col);
		  data->tg.det[pos].h.GeHP&=kill;
		  data->tg.det[pos].h.Gefold--;
		  kill=none-(one<<id_ge);
		  data->tg.g.GeHP&=kill;
		  data->tg.g.Gefold--;
		  data->tg.g.THP&=kill;
		  data->tg.g.Tfold--;
		}
	    }
	}
    }
  
  for(pos=1;pos<NPOSTIGR;pos++)
    {
      id=pos-1;
      if((data->tg.h.GeHP&(1<<id))!=0)
	if(data->tg.det[pos].h.Gefold<=0)
	  {
	    //drop this position
	    memset(&data->tg.det[pos],0,sizeof(CssTIGR));
	    kill=none-(one<<id);
	    data->tg.h.GeHP&=kill;
	    data->tg.h.Gefold--;
	    data->tg.g.PosHP&=kill;
	    data->tg.g.Posfold--;
	  }
    }
  
  
  if(data->tg.h.Gefold<=0)
    {
      memset(&data->tg,0,sizeof(Tigress));
      kill=none-TIGRESS_BIT;
      data->h.setupHP&=kill;
    }
  
  //drop PIN out of the time limits
  for(int pin=1;pin<NPIN;pin++)
    if((data->pinarray.h.TSHP&(one<<pin))!=0)
      if((flag_pin&(one<<pin))==0)
	{
	  memset(&data->pinarray.pin[pin],0,sizeof(channel));
	  memset(&data->pinarray.wfit[pin],0,sizeof(ShapePar));
	  // memset(&data->csiarray.t0[pin],0,sizeof(double)); //FW: Don't need this because PIN already hasnt t0
	  data->pinarray.h.Efold--;
	  data->pinarray.h.Tfold--;	  
	  data->pinarray.h.TSfold--;
	  kill=none-(one<<pin);
	  data->pinarray.h.TSHP&=kill;
	  data->pinarray.h.EHP&=kill;
	  data->pinarray.h.THP&=kill;
	}
  
  if(data->pinarray.h.TSfold<=0)
    {
      kill=none-PINArray_BIT; 
      data->h.setupHP&=kill;
      memset(&data->pinarray,0,sizeof(PINArray));
    } 

  if((data->h.setupHP&TIGRESS_BIT)==0)
    return SEPARATOR_DISCARD;
  
  if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;

 if((data->h.setupHP&PINArray_BIT)==0)
    return SEPARATOR_DISCARD;

  encode(data,output,enb);
  
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;

  if(argc!=4)
    {
      printf("\n ./separate_TigressPinArray_TTCalDiff master_file_name low high\n");
      exit(-1);
    }
  
  h = new TH1D("Tigress Pinarray Time","Tigress Pinarray Time",S16K,-S8K,S8K);
  h->Reset();

  g = new TH1D("Tigress Pinarray Gate","Tigress Pinarray Gate",S16K,-S8K,S8K);
  g->Reset();

  low=atof(argv[2]);
  high=atof(argv[3]);

  printf("Program sorts calibrated 2D histogram for TIGRESS/PINARRAY timing \n");
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

  if(name->flag.TIGRESS_cal_par==1)
        {
          printf("\nTIGRESS calibration read from the file:\n %s\n",name->fname.TIGRESS_cal_par);
          initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
	  
        }
      else
        {
          printf("\nTIGRESS calibration parameters not defined\n");
          exit(-1);
        }

  if(name->flag.PINARRAY_cal_par==1)
        {
          printf("\nPINARRAY calibration read from the file:\n %s\n",name->fname.PINARRAY_cal_par);
          initialize_PINARRAY_calibration(&cal_par->pinarray,name->fname.PINARRAY_cal_par);
	  
        }
      else
        {
          printf("\nPINARRAY calibration parameters not defined\n");
          exit(-1);
        }

 if(name->flag.out_data!=1)
    {
      printf("\nOutput data file not defined\n");
      exit(EXIT_FAILURE);
    }

 
 
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
  
	//save the last buffer which will be dropped otherwise
	//if enb[1]==2 then the buffer contains no data, only the header
	if(enb[1]>2)
		fwrite(enb,sizeof(int),BUFFSIZE,output);

  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("TTCalDiff", "TTCalDiff",10,10, 500, 300);
  gPad->SetLogz(1); 
  g->SetLineColor(2);
  h->Draw();
  g->Draw("same");
  theApp->Run(kTRUE);
 
  free(name);
  free(cal_par);
 
}			
