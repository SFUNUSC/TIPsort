#include "sort.h"

int analyze_data(raw_event *data)
{

 cal_event* cev;
  int pos,col,csi;
  double ttg,tcsi,tdiff;
  
  long long int one=1,none=-1,kill;
  int id,id_ge;
  long long int flag_ge,flag_csi,drop;
  int    flag_pos;

  double tmin=10E10;

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

  flag_pos=0;
  flag_ge=0;
  flag_csi=0;

  if(cev->tg.h.FT>0)
    if(cev->csiarray.h.FT>0)
      for(pos=1;pos<NPOSTIGR;pos++)
	if((cev->tg.h.THP&(1<<(pos-1)))!=0)
	  if(cev->tg.det[pos].hge.FT>0)
	    for(col=0;col<NCOL;col++)
	      if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
		if(cev->tg.det[pos].ge[col].h.FT>0)
		  if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
		    {
		      ttg=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;
		      //Finding tmin first looking at Tigress
		      if(ttg<tmin) 
			tmin=ttg;
		    }


  for(csi=1;csi<NCSI;csi++)
    if((cev->csiarray.h.THP&(one<<csi))!=0)
      {
	tcsi=cev->csiarray.csi[csi].T/cal_par->csiarray.contr_t;
	//Find tmin second looking at CsI
	if(tcsi<tmin) 
	  tmin=tcsi;
      }

  printf("tmin = %f\n",tmin);

 if(cev->tg.h.FT>0)
    if(cev->csiarray.h.FT>0)
      for(pos=1;pos<NPOSTIGR;pos++)
	if((cev->tg.h.THP&(1<<(pos-1)))!=0)
	  if(cev->tg.det[pos].hge.FT>0)
	    for(col=0;col<NCOL;col++)
	      if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
		if(cev->tg.det[pos].ge[col].h.FT>0)
		  if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
		    {
		      ttg=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;	    
		      tdiff=ttg-tmin;
		      printf("ttig = %f ----> tdiff_tigress = %f\n",ttg,tdiff);
		      h_tigTDiff->Fill(tdiff);

		      /* if(tdiff<=low) */
		      /* 	{ */
		      /* 	  id=pos-1; */
		      /* 	  id_ge=id*NCOL+col; */
		      /* 	  flag_ge|=(one<<id_ge); */
		      /* 	} */
		    }

 for(csi=1;csi<NCSI;csi++)
   if((cev->csiarray.h.THP&(one<<csi))!=0)
     {
       tcsi=cev->csiarray.csi[csi].T/cal_par->csiarray.contr_t;
       tdiff=tcsi-tmin;
       printf("tcsi = %f ----> tdiff_csi = %f\n",tcsi,tdiff);
       h_csiTDiff->Fill(tdiff);
       
       /* if(tdiff<=high) */
       /* 	 { */
       /* 	  flag_csi|=(one<<csi); */
       /* 	 } */
     }

  free(cev);
  getc(stdin);


  //We dont need to drop data or encode, so this all this is commented... can probably remove

  /* //drop TIGRESS data out of the time limits */
 /*   for(pos=1;pos<NPOSTIGR;pos++) */
 /*     { */
 /*       id=pos-1; */
 /*       for(col=0;col<NCOL;col++) */
 /* 	 {	 */
 /* 	   id_ge=id*NCOL+col; */
 /* 	   drop=(one<<id_ge); */
 /* 	   drop&=data->tg.g.GeHP; */
 /* 	   if(drop!=0) */
 /* 	     { */
 /* 	       drop&=flag_ge; */
 /* 	       if(drop==0) */
 /* 		 { */
 /* 		   //drop this crystal */
 /* 		   memset(&data->tg.det[pos].ge[col],0,sizeof(SegTIGR)); */
 /* 		   kill=none-(one<<col); */
 /* 		   data->tg.det[pos].h.GeHP&=kill; */
 /* 		   data->tg.det[pos].h.Gefold--; */
 /* 		   kill=none-(one<<id_ge); */
 /* 		   data->tg.g.GeHP&=kill; */
 /* 		   data->tg.g.Gefold--; */
 /* 		   data->tg.g.THP&=kill; */
 /* 		   data->tg.g.Tfold--; */
 /* 		 } */
 /* 	     } */
 /* 	 } */
 /*     } */

 /*   for(pos=1;pos<NPOSTIGR;pos++) */
 /*     { */
 /*       id=pos-1; */
 /*       if((data->tg.h.GeHP&(1<<id))!=0) */
 /* 	 if(data->tg.det[pos].h.Gefold<=0) */
 /* 	   { */
 /* 	     //drop this position */
 /* 	     memset(&data->tg.det[pos],0,sizeof(CssTIGR)); */
 /* 	     kill=none-(one<<id); */
 /* 	     data->tg.h.GeHP&=kill; */
 /* 	     data->tg.h.Gefold--; */
 /* 	     data->tg.g.PosHP&=kill; */
 /* 	     data->tg.g.Posfold--; */
 /* 	   } */
 /*     } */
   
 
 /*   if(data->tg.h.Gefold<=0) */
 /*     { */
 /*       memset(&data->tg,0,sizeof(Tigress)); */
 /*       kill=none-TIGRESS_BIT; */
 /*       data->h.setupHP&=kill; */
 /*     } */

 /*  //drop csi out of the time limits */
 /*  for(csi=1;csi<NCSI;csi++) */
 /*    if((data->csiarray.h.TSHP&(one<<csi))!=0) */
 /*      if((flag_csi&(one<<csi))==0) */
 /* 	{ */
 /* 	  memset(&data->csiarray.csi[csi],0,sizeof(channel)); */
 /* 	  memset(&data->csiarray.wfit[csi],0,sizeof(ShapePar)); */
 /* 	  memset(&data->csiarray.t0[csi],0,sizeof(double)); */
 /* 	  data->csiarray.h.Efold--; */
 /* 	  data->csiarray.h.Tfold--;	   */
 /* 	  data->csiarray.h.TSfold--; */
 /* 	  kill=none-(one<<csi); */
 /* 	  data->csiarray.h.TSHP&=kill; */
 /* 	  data->csiarray.h.EHP&=kill; */
 /* 	  data->csiarray.h.THP&=kill; */
 /* 	} */
  
 /*  if(data->csiarray.h.TSfold<=0) */
 /*    { */
 /*      kill=none-CsIArray_BIT;  */
 /*      data->h.setupHP&=kill; */
 /*      memset(&data->csiarray,0,sizeof(CsIArray)); */
 /*    }  */

 /*  if((data->h.setupHP&TIGRESS_BIT)==0) */
 /*    return SEPARATOR_DISCARD; */

 /*  if((data->h.setupHP&RF_BIT)==0) */
 /*    return SEPARATOR_DISCARD; */

 /* if((data->h.setupHP&CsIArray_BIT)==0) */
 /*    return SEPARATOR_DISCARD; */

 /* encode(data,output,enb); */

  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  char title[132];

  if(argc!=2)
    {
      printf("\n ./TigressCsIArray_TDiff master_file_name\n");
      exit(-1);
    }
  
  h_csiTDiff = new TH1D("Csiarray TDiff","Csiarray TDiff",S16K,-S8K,S8K);
  h_csiTDiff->Reset();

  h_tigTDiff = new TH1D("Tigress TDiff","Tigress TDiff",S16K,-S8K,S8K);
  h_tigTDiff->Reset();

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

 /* if(name->flag.out_data!=1) */
 /*    { */
 /*      printf("\nOutput data file not defined\n"); */
 /*      exit(EXIT_FAILURE); */
 /*    } */

 
 
 /*  if((output=fopen(name->fname.out_data,"w"))==NULL) */
 /*    { */
 /*      printf("\nI can't open output file %s for writing\n",name->fname.out_data); */
 /*      exit(-2); */
 /*    } */

  //Do we need this?
  memset(enb,0,sizeof(enb));
  enb[0]=BUFFER_TAG;
  enb[1]++;
  enb[1]++;

  sort(name);

  sprintf(title,"TigressCsIArray_TDiff.root");
  TFile f(title, "recreate");
  h_csiTDiff->GetXaxis()->SetTitle("CsI TDiff [10ns]");
  h_csiTDiff->GetXaxis()->CenterTitle(true);

  h_tigTDiff->GetXaxis()->SetTitle("Tigress TDiff [10ns]");
  h_tigTDiff->GetXaxis()->CenterTitle(true);

  h_csiTDiff->Write();
  h_tigTDiff->Write();

  //theApp=new TApplication("App", &argc, argv);
  //canvas = new TCanvas("TTCalDiff", "TTCalDiff",10,10, 500, 300);
  /* gPad->SetLogz(1); */
  /* g->SetLineColor(2); */
  /* h->Draw(); */
  /* g->Draw("same"); */
  /* theApp->Run(kTRUE); */

}

