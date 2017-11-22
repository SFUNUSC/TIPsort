#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos,col,csi;
  double tdiff;
  double thit;
  bool first_hit;
  
  int64_t one=1,none=-1,kill;
  int id,id_ge;
  long long int flag_ge,drop;
  int64_t flag_csi[4];

  double ttg1=10E10;
  double tcsi1=-10E10;

  /* printf("init bits:\n  Tigress: %d\n  CsI: %d\n  RF: %d\n",data->h.setupHP&TIGRESS_BIT,data->h.setupHP&CsIArray_BIT,data->h.setupHP&RF_BIT); */
  /* getc(stdin); */

  if((data->h.setupHP&TIGRESS_BIT)==0)
    return SEPARATOR_DISCARD;

  //if((data->h.setupHP&RF_BIT)==0)
  //  return SEPARATOR_DISCARD;

  if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);

  flag_ge=0;
  memset(flag_csi,0,sizeof(flag_csi));

  //Given any number of TIGRESS detector hits during the event, find the time at which 
  //the first and second hits occur.
  first_hit = true;
  if(cev->tg.h.FT>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.THP&(1<<(pos-1)))!=0)
	      if(cev->tg.det[pos].hge.FT>0)
	        for(col=0;col<NCOL;col++)
	          if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
	            if(cev->tg.det[pos].ge[col].h.FT>0)
		            if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
		              {
                    thit=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;
                    if(abs(thit-tigTVal)<=gate_length)//check whether hit is near central value
                    	{
				                if (first_hit == true)
				                  {
				                    first_hit = false;
				                    ttg1=thit;
				                  }
				                else if (ttg1>thit)
				                  {
				                    ttg1=thit; //assign the time of the first hit
				                  }
                      }
                  }

  //Given any number of CsI detector hits during the event, find the time at which 
  //the first and second hits occur.
  first_hit = true;
  if(cev->csiarray.h.FT>0)
    for(pos=1;pos<NCSI;pos++) //look at each CsI position
      if((cev->csiarray.h.THP[pos/64]&(one<<pos%64))!=0) //is there a hit in the detector?
        {
          thit=cev->csiarray.csi[pos].T/cal_par->csiarray.contr_t;
          if(abs(thit-csiTVal)<=gate_length)//check whether hit is near central value
          	{
				      if (first_hit == true)
				        {
				          first_hit = false;
				          tcsi1=thit;
				        }
				      else if (tcsi1>thit)
				        {
				          tcsi1=thit; //assign the time of the first hit
				        }
            }
        }

  tdiff=tcsi1-ttg1; //time difference between the first particle and first gamma
  /* printf("tidff %f tlow %f thigh %f \n",tdiff,low,high); */

  h->Fill(tdiff);
  if( ((corr==1)&&(tdiff>=low)&&(tdiff<=high)) || ((corr==0)&&( (tdiff<low)||(tdiff>high) ) ) )
    {
      /* printf("GOOD tidff %f tlow %f thigh %f \n",tdiff,low,high); */
      g->Fill(tdiff);
      if(cev->csiarray.h.FT>0)
        for(csi=1;csi<NCSI;csi++) //look at each CsI position
        	{
		        if((cev->csiarray.h.THP[csi/64]&(one<<csi%64))!=0) //is there a hit in the detector?
		        	{
		        		thit=cev->csiarray.csi[csi].T/cal_par->csiarray.contr_t;
		        		if(abs(thit-csiTVal)<=gate_length)//check whether hit is near central value
		          		flag_csi[csi/64]|=(one<<csi%64); //flag the hit for preservation
		          }
          }
      if(cev->tg.h.FT>0)
        for(pos=1;pos<NPOSTIGR;pos++) //look at each Tigress position
        	{
		        if((cev->tg.h.THP&(1<<(pos-1)))!=0)
		          if(cev->tg.det[pos].hge.FT>0)
		            for(col=0;col<NCOL;col++)
		              if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
		                if(cev->tg.det[pos].ge[col].h.FT>0)
											if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
												{
													thit=cev->tg.det[pos].ge[col].seg[0].T/cal_par->tg.contr_t;
													if(abs(thit-tigTVal)<=gate_length)//check whether hit is near central value
														{
															id=pos-1;
															id_ge=id*NCOL+col;
															flag_ge|=(one<<id_ge); //flag the hit for preservation
														}
												}
					}
    }

  // cev malloc
  // disasterous memory leak!
  free(cev); 
  
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
    if((data->csiarray.h.TSHP[csi/64]&(one<<csi%64))!=0)
      if((flag_csi[csi/64]&(one<<csi%64))==0)
	      {
	        memset(&data->csiarray.csi[csi],0,sizeof(channel));
	        memset(&data->csiarray.wfit[csi],0,sizeof(ShapePar));
	        memset(&data->csiarray.t0[csi],0,sizeof(double));
	        data->csiarray.h.Efold--;
	        data->csiarray.h.Tfold--;	  
	        data->csiarray.h.TSfold--;
	        kill=none-(one<<csi%64);
	        data->csiarray.h.TSHP[csi/64]&=kill;
	        data->csiarray.h.EHP[csi/64]&=kill;
	        data->csiarray.h.THP[csi/64]&=kill;
	      }
  
  if(data->csiarray.h.TSfold<=0)
    {
      kill=none-CsIArray_BIT; 
      data->h.setupHP&=kill;
      memset(&data->csiarray,0,sizeof(CsIArray));
    } 

  /* printf("final bits:\n  Tigress: %d\n  CsI: %d\n  RF: %d\n",data->h.setupHP&TIGRESS_BIT,data->h.setupHP&CsIArray_BIT,data->h.setupHP&RF_BIT); */

  if((data->h.setupHP&TIGRESS_BIT)==0)
    return SEPARATOR_DISCARD;
  
  /* if((data->h.setupHP&RF_BIT)==0) */
   /*   return SEPARATOR_DISCARD; */
  
  if((data->h.setupHP&CsIArray_BIT)==0)
      return SEPARATOR_DISCARD;
      
  /* printf("pre encode:\n  Tigress: %d\n  CsI: %d\n  RF: %d\n",data->h.setupHP&TIGRESS_BIT,data->h.setupHP&CsIArray_BIT,data->h.setupHP&RF_BIT); */
  /* getc(stdin); */
  
  encode(data,output,enb);
  
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  int plot_option=0;

  if((argc!=6)&&(argc!=7))
    {
      printf("\n ./separate_TigressCsIArray_TTCalFirstHitDiffCentralValue master_file_name low high Tigress_val CsI_val\n");
      printf("\n Sorts events where the first gamma and the first particle detected come within the time window defined by 'low' and 'high' with respect to each other.\n");
      printf("\n Only data around the central time value specified in the parameters 'Tigress_val' and 'CsI_val' will be kept, with width specified by the difference between 'high' and 'low'.\n");
      printf("\n If 'low' is larger than 'high', the program will sort uncorrelated data (events outside the time gate).\n");
      printf("\n Add '1' as a 7th argument to show a plot of the gate (not required).\n");
      exit(-1);
    }
    
  h = new TH1D("Tigress Csiarray Time","Tigress Csiarray Time",S16K,-S8K,S8K);
  h->Reset();

  g = new TH1D("Tigress Csiarray Gate","Tigress Csiarray Gate",S16K,-S8K,S8K);
  g->Reset();

  low=atof(argv[2]);
  high=atof(argv[3]);
  
  printf("Program sorts data separated on Tigress-CsIArray timing.\n");
  corr=1;
	if(low>high)
		{
			//swap values
			double tmp=low;
			low=high;
			high=tmp;
			//set correlation to not be used
			corr=0;
			printf("Uncorrelated data will be separated.\n");
		}
  
  tigTVal=atoi(argv[4]);
  csiTVal=atoi(argv[5]);
  gate_length=abs(high-low)/2.;
  
  if(argc==7)
    plot_option=atoi(argv[6]);
  if(plot_option==1)
  	printf("Timing gate will be plotted.\n");

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

  if(plot_option==1)
    {
      theApp=new TApplication("App", &argc, argv);
      canvas = new TCanvas("TTCalDiff", "TTCalDiff",10,10, 500, 300);
      gPad->SetLogz(1); 
      g->SetLineColor(2);
      h->Draw();
      g->Draw("same");
      theApp->Run(kTRUE);
    }
  free(name);
  free(cal_par);
 
}

