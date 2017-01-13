#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos,col,csi;
  double ttg,tcsi,tdiff,tdiff_ttg;
  
  long long int one=1,none=-1,kill;
  int id,id_ge;
  long long int flag_ge,flag_csi,drop;
  //int    flag_pos;

  //double tmin=10E10;
  double tmin_ttg=10E10;
  double tmax=-10E10;

  /* printf("TIG bit: %d CsI bit: %d RF bit: %d\n",data->h.setupHP&TIGRESS_BIT,data->h.setupHP&CsIArray_BIT,data->h.setupHP&RF_BIT); */
  /* getc(stdin); */

  if((data->h.setupHP&TIGRESS_BIT)==0)
    return SEPARATOR_DISCARD;

  if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;

 if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;

   cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);

  //flag_pos=0;
  flag_ge=0;
  flag_csi=0;

  /* printf("tig fold %d csi fold %d\n",cev->tg.h.FT,cev->csiarray.h.FT); */
  /* getc(stdin); */
  if(cev->tg.h.FT>0)
    if(cev->csiarray.h.FT>0)
      {
	//Try timing with only CsI max
	//Extract max time from here
	for(csi=1;csi<NCSI;csi++)
	  if((cev->csiarray.h.THP&(one<<csi))!=0)
	    {
	      tcsi=cev->csiarray.csi[csi].T/cal_par->csiarray.contr_t;
	      
	      if(tcsi>tmax) 
		tmax=tcsi;
	      
	      /* if(tcsi<tmin)  */
	      /*   tmin=tcsi; */
	    }

	//printf("tmax_csi %.2f\n",tmax);
	
	//After finidng tmax for CsI, find ttg_min within the CsI window
	for(pos=1;pos<NPOSTIGR;pos++)
	  if((cev->tg.h.THP&(1<<(pos-1)))!=0)
	    if(cev->tg.det[pos].hge.FT>0)
	      for(col=0;col<NCOL;col++)
		if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
		  if(cev->tg.det[pos].ge[col].h.FT>0)
		    if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
		      {
			//get TIGRESS time
			ttg=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;
			
			//calculate tdiff from CsI max time
			tdiff=tmax-ttg;
			//tdiff=ttg-tmin;
			
			//if the time is within the CsI time window
			//and less than current TIGRESS time minimum,
			//set as TIGRESS time minimum --- tdiff should be >= 0!!! does not seem good for CsI fold 1 trigger
			//if(tdiff>0)
			    if(tdiff<=high)
			      if(ttg<tmin_ttg)
				tmin_ttg=ttg;
		      }
	
	//printf("tmin_ttg: %.2f\n",tmin_ttg);
	
	//Now that ttg_tmin is set, find good TIGRESS events
	for(pos=1;pos<NPOSTIGR;pos++)
	  if((cev->tg.h.THP&(1<<(pos-1)))!=0)
	    if(cev->tg.det[pos].hge.FT>0)
	      for(col=0;col<NCOL;col++)
		if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
		  if(cev->tg.det[pos].ge[col].h.FT>0)
		    if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
		      {
			//get TIGRESS event and calculate tdiff again
			ttg=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;
			tdiff=tmax-ttg;
			
		      //find TIGRESS time difference
			tdiff_ttg=ttg-tmin_ttg;
                        h->Fill(tdiff);

			
			//printf("ttg: %.2f tdiff: %.2f tdiff_ttg: %.2f\n",ttg,tdiff,tdiff_ttg);
			
			//if the time is within the CsI time window
			//and TIGRESS time is within the TIGRESS time window
			//flag the event as good. 
			if(tdiff>0) //CsI is last event check
			  if(tdiff<=high) //CsI window check
			    if(tdiff_ttg<=low) //TIGRESS part
			      {
				//printf(" EVENT KEPT\n");
                                g->Fill(tdiff);
				id=pos-1;
				id_ge=id*NCOL+col;
				flag_ge|=(one<<id_ge);
			      }
			
			
			/* if(tdiff>high) //CsI window fail */
			/*   { */
			/*     printf(" EVENT REJECTED -- outside CsI window\n"); */
		      	/* } */
			
			/* else if(tdiff<0) //CsI last event fail */
			/*   { */
			/*     printf(" EVENT REJECTED -- TIGRESS event after last CsI\n"); */
			/*   } */
			/* else if(tdiff_ttg>low) //CsI last event fail */
			/*   { */
			/*     printf(" EVENT REJECTED -- outside TIGRESS window\n"); */
			/*   } */
			
		      }
	
	//keep good CsI events
	for(csi=1;csi<NCSI;csi++)
	  if((cev->csiarray.h.THP&(one<<csi))!=0)
	    {
	      tcsi=cev->csiarray.csi[csi].T/cal_par->csiarray.contr_t;
	      
	      tdiff=tmax-tcsi;
	      //tdiff=tcsi-tmin;
	      //printf("tcsi: %.2f tdiff: %.2f\n",tcsi,tdiff);
	      
	      //CsI is in CsI window and later than first TIGRESS
	      if(tdiff<=high)
		      if(tcsi>tmin_ttg)
		        {
		          flag_csi|=(one<<csi);
		        }
	    }
	
	free(cev);
	//getc(stdin);
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
  
  //drop csi out of the time limits
  for(csi=1;csi<NCSI;csi++)
    if((data->csiarray.h.TSHP&(one<<csi))!=0)
      if((flag_csi&(one<<csi))==0)
	{
	  memset(&data->csiarray.csi[csi],0,sizeof(channel));
	  memset(&data->csiarray.wfit[csi],0,sizeof(ShapePar));
	  memset(&data->csiarray.t0[csi],0,sizeof(double));
	  data->csiarray.h.Efold--;
	  data->csiarray.h.Tfold--;	  
	  data->csiarray.h.TSfold--;
	  kill=none-(one<<csi);
	  data->csiarray.h.TSHP&=kill;
	  data->csiarray.h.EHP&=kill;
	  data->csiarray.h.THP&=kill;
	}
  
  if(data->csiarray.h.TSfold<=0)
    {
      kill=none-CsIArray_BIT; 
      data->h.setupHP&=kill;
      memset(&data->csiarray,0,sizeof(CsIArray));
    } 

  if((data->h.setupHP&TIGRESS_BIT)==0)
    return SEPARATOR_DISCARD;

  if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;

 if((data->h.setupHP&CsIArray_BIT)==0)
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
      printf("\n ./separate_TigressCsIArray_TTCalDiff master_file_name low high\n");
      exit(-1);
    }
  
  h = new TH1D("Tigress Csiarray Time","Tigress Csiarray Time",S16K,-S8K,S8K);
  h->Reset();

  g = new TH1D("Tigress Csiarray Gate","Tigress Csiarray Gate",S16K,-S8K,S8K);
  g->Reset();

  low=atof(argv[2]);
  high=atof(argv[3]);

  printf("Program sorts calibrated 2D histogram for TIGRESS/CSIARRAY timing \n");
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

  if(name->flag.CSIARRAY_cal_par==1)
        {
          printf("\nCSIARRAY calibration read from the file:\n %s\n",name->fname.CSIARRAY_cal_par);
          initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
	  
        }
      else
        {
          printf("\nCSIARRAY calibration parameters not defined\n");
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

