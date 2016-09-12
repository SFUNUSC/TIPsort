#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos,col,csi;
  double eAddBack=0;
  int colAddBack;
  double ecsi;
  
  long long int one=1,none=-1,kill;
  int id,id_ge;
  long long int flag_ge,flag_csi,drop;

  if((data->h.setupHP&TIGRESS_BIT)==0)
    return SEPARATOR_DISCARD;

  if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;

 if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;

 
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  memset(beam_p,0,sizeof(beam_p));
  memset(gamma_dir,0,sizeof(gamma_dir));
  
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);

	//get beam momentum from calibration (value specified in parameter file)
	beam_p[2]=cal_par->csiarray.pp;
	
  flag_ge=0;
  flag_csi=0;
  
  //check the Ge fold
  if(cev->tg.h.FH>0)
	  //look through each Tigress position
	  for(pos=1;pos<NPOSTIGR;pos++)
      {
        //check if the position is in the hit pattern
        if((cev->tg.h.HHP&(1<<(pos-1)))!=0) 
          //check the energy fold
          if(cev->tg.det[pos].hge.FH>0)
            //check if the position is in the addback hit pattern
            if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
              {

                eAddBack = cev->tg.det[pos].addback.E/cal_par->tg.contr_e;  
                colAddBack = cev->tg.det[pos].addbackC;
                
                if(eAddBack>=0 && eAddBack<S32K)
                  {
                    //construct vectors
                    for(int ind=0;ind<3;ind++)
                      gamma_dir[ind]=cal_par->tg.tpos_xyz[pos][colAddBack][ind];
                    vecMag=sqrt(gamma_dir[0]*gamma_dir[0] + gamma_dir[1]*gamma_dir[1] + gamma_dir[2]*gamma_dir[2]);
                    for(int ind=0;ind<3;ind++)
                      gamma_dir[ind]=gamma_dir[ind]/vecMag;//make unit vector
                    
                    memset(part_p,0,sizeof(part_p));
                    if(cev->csiarray.h.FH>0)
                      for(csi=1;csi<NCSI;csi++)
                        if((cev->csiarray.h.HHP&(one<<csi))!=0)
                          {
                            for(int ind=0;ind<3;ind++)
                              part_p[csi][ind]=cal_par->csiarray.cpos_xyz[csi][ind];
                            vecMag=sqrt(part_p[csi][0]*part_p[csi][0] + part_p[csi][1]*part_p[csi][1] + part_p[csi][2]*part_p[csi][2]);
                            ecsi=cev->csiarray.csi[csi].E/1000.; /* CsI energy in MeV */
                            for(int ind=0;ind<3;ind++)
                              part_p[csi][ind]=part_p[csi][ind]*(sqrt(2*ecsi*cal_par->csiarray.mp))/vecMag;//make vector proper length for momentum
                          }
              
                    //subtract detected particle momenta from beam momentum to get residual momentum
                    for(int ind=0;ind<3;ind++)
                      res_p[ind]=beam_p[ind];
                    for(csi=1;csi<NCSI;csi++)
                      for(int ind=0;ind<3;ind++)
                        res_p[ind]=res_p[ind]-part_p[csi][ind];
                        
                    //get residual energy
                    res_e=0.;
                    for(int ind=0;ind<3;ind++)
                    	res_e+=res_p[ind]*res_p[ind];//dot product
                    res_e/=2.*(cal_par->csiarray.md - 2.*cal_par->csiarray.mp);
                    //printf("res_e: %f\n",res_e);
                    //getc(stdin);
                    //h->Fill(res_e);
                    
                    if((res_e>=low)&&(res_e<=high))
											{
												if(cev->csiarray.h.FT>0)
													for(pos=1;pos<NCSI;pos++) //look at each CsI position
														if((cev->csiarray.h.THP&(one<<pos))!=0) //is there a hit in the detector?
															flag_csi|=(one<<pos); //flag the hit for preservation
												if(cev->tg.h.FT>0)
													for(pos=1;pos<NPOSTIGR;pos++) //look at each Tigress position
														if((cev->tg.h.THP&(1<<(pos-1)))!=0)
															if(cev->tg.det[pos].hge.FT>0)
																for(col=0;col<NCOL;col++)
																	if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
																		if(cev->tg.det[pos].ge[col].h.FT>0)
																			if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
																			  {
																		      id=pos-1;
																		      id_ge=id*NCOL+col;
																		      flag_ge|=(one<<id_ge); //flag the hit for preservation
																		    }
											} 

                    
                  }
                
              }
      }
  
  

  //drop TIGRESS data out of the energy limits
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
  
  //drop csi out of the energy limits
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
  //TCanvas *canvas;
  //TApplication *theApp;

  if(argc!=4)
    {
      printf("\n ./separate_FusEvap_ResE master_file_name low high\n");
      printf("\n Assumes fusion evaporation data, and separates based on the inferred energy of the residual nucleus (the one emitting the gamma ray) immediately following the evaporation of 2 particles.\n");
      exit(-1);
    }
  
  h = new TH1D("Residual Energy","Residual Energy",S16K,-S8K,S8K);
  h->Reset();

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

  //theApp=new TApplication("App", &argc, argv);
  //canvas = new TCanvas("Residual Energy", "Residual Energy",10,10, 500, 300);
  //h->Draw();
  //theApp->Run(kTRUE);
 
 
}

