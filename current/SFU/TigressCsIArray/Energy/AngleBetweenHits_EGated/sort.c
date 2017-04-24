#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos1;
  
  
  double eAddBack=0;
  
 	//if((data->h.setupHP&RF_BIT)==0)
  //  return SEPARATOR_DISCARD;

 	if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  
  bool inGate = false;
  if(cev->tg.h.FH>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if(((cev->tg.h.HHP&(1<<(pos-1)))!=0) && (cev->tg.det[pos].hge.FE>0))
				if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
					{
					
						//Let's output some samples of add back at work...
						/* for(col=0;col<NCOL;col++) */
						/*   if(((cev->tg.det[pos].hge.HHP&(1<<col))!=0) && (cev->tg.det[pos].ge[col].h.FH>0)) */
						/* 	{ */
						/* 	  ePreAddBack = cev->tg.det[pos].ge[col].seg[0].E/cal_par->tg.contr_e; */
						/* 	  if(cev->tg.det[pos].hge.FE > 2) printf("For Position %d and Core %d the energy is %f.\n",pos,col,ePreAddBack); */
						/* 	} */

						eAddBack = cev->tg.det[pos].addback.E/cal_par->tg.contr_e;  
						col = cev->tg.det[pos].addbackC;
						if(eAddBack>=eGateMin && eAddBack<=eGateMax)
							{
								inGate=true;
								break;
							}
					
						//if(cev->tg.det[pos].hge.FE > 2) printf("The add back energy is ---> %f.\n",eAddBack);
					}
  
  if(inGate)
  	{
  		int csiHitIndex=0;
			double angle=0.;
			if(cev->csiarray.h.FE>0)
				for(pos1=1;pos1<NCSI;pos1++)
				  if((cev->csiarray.h.EHP&(one<<pos1))!=0)
						{
							//printf("Hit found...\n");
							if(csiHitIndex<2)
								{
									for(int ind=0;ind<3;ind++)
										res_dir[csiHitIndex][ind]=cal_par->csiarray.cpos_xyz[pos1][ind];
									csiHitIndex++;
								}
						}
			//get the angle between the hits
			if(csiHitIndex>=2) //are we able to get an angle (have 2 hits?)
				{
					//printf("Calculating angle...\n");
					for(int i=0;i<2;i++)
						res_dir_mag[i]=sqrt(res_dir[i][0]*res_dir[i][0] + res_dir[i][1]*res_dir[i][1] + res_dir[i][2]*res_dir[i][2]);
					for(int i=0;i<3;i++)
						angle += res_dir[0][i] * res_dir[1][i]; //dot product
					angle = angle/(res_dir_mag[0]*res_dir_mag[1]); //normalization
					//angle = acos(angle);
					//angle *= 180./3.14159265359; //convert to degrees
					//printf("Angle between hits: %f\n",angle);
					h->Fill(angle);
				}
		}
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
	TCanvas *canvas;
  TApplication *theApp;
  
  //FILE *output;
  input_names_type* name;

  if(argc!=4)
    {
      printf("TigressCsIArray_AngleBetweenHits_EGated master_file_name Emin Emax\n");
      printf("Program sorts histogram of angles between hits in the CsIArray, for events which come with a gamma ray in the specified energy gate (gate values specified in keV).\n");
      exit(-1);
    }
  
  //positionHist = new TH1D("CosAngleHistogramFromPositions","CosAngleHistogramFromPositions",200,-1,1);
  //positionHist->Reset();
  h = new TH1D("CosAngleHistogramFromData","CosAngleHistogramFromData",200,-1,1);
  h->Reset();
  
  printf("Program sorts histogram of angles between hits in the CsIArray, for events which come with a gamma ray in the specified energy gate (gate values specified in keV).\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);
  eGateMin=atof(argv[2]);
  eGateMax=atof(argv[3]);

  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined!\n");
      exit(EXIT_FAILURE);
    }

  if(name->flag.CSIARRAY_cal_par==1)
    {
      printf("CsIArray calpar read from: %s\n",name->fname.CSIARRAY_cal_par);
      initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
    }
  else
    {
      printf("ERROR! CsIArray calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calpar read from: %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR!!! TIGRESS calibration parameters not defined\n");
      exit(EXIT_FAILURE);
    }
  
  //apply proper contraction to gate
  eGateMin=eGateMin/cal_par->tg.contr_e;
  eGateMax=eGateMax/cal_par->tg.contr_e;
  
  sort(name);
  
  /*//fill histogram of positions from combinatorics
  printf("%i\n",NCSI);
  double angle=0.;
  for(int pos1=1;pos1<NCSI;pos1++)
		for(int pos2=pos1;pos2<NCSI;pos2++)
			{
				for(int ind=0;ind<3;ind++)
					{
						res_dir[0][ind]=cal_par->csiarray.cpos_xyz[pos1][ind];
						res_dir[1][ind]=cal_par->csiarray.cpos_xyz[pos2][ind];
					}
				//get the angle
				for(int i=0;i<2;i++)
					res_dir_mag[i]=sqrt(res_dir[i][0]*res_dir[i][0] + res_dir[i][1]*res_dir[i][1] + res_dir[i][2]*res_dir[i][2]);
				for(int i=0;i<3;i++)
					angle += res_dir[0][i] * res_dir[1][i]; //dot product
				angle = angle/(res_dir_mag[0]*res_dir_mag[1]); //normalization
				//angle = acos(angle);
				//angle *= 180./3.14159265359; //convert to degrees
				//printf("Angle between hits: %f\n",angle);
				positionHist->Fill(angle);
			}*/
  
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("AngleHistogramFromData","AngleHistogramFromData",10,10, 500, 300);
  gPad->SetLogy(0);
  h->Draw();
  theApp->Run(kTRUE);
  
  /*theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("AngleHistogramFromPositions","AngleHistogramFromPositions",10,10, 500, 300);
  gPad->SetLogy(0);
  positionHist->Draw();
  theApp->Run(kTRUE);*/
  
  
  
  }
