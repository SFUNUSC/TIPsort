#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  uint64_t one=1;
  int pos1;
  
  
 	//if((data->h.setupHP&RF_BIT)==0)
  //  return SEPARATOR_DISCARD;

 	if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  
  
  int csiHitIndex=0;
	double angle=0.;
  if(cev->csiarray.h.FE>0)
    for(pos1=1;pos1<NCSI;pos1++)
      if((cev->csiarray.h.EHP[pos1/64]&(one<<pos1%64))!=0)
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

  if(argc!=2)
    {
      printf("CsIArray_AngleBetweenHits master_file_name\n");
      exit(-1);
    }
  
  positionHist = new TH1D("CosAngleHistogramFromPositions","CosAngleHistogramFromPositions",200,-1,1);
  positionHist->Reset();
  h = new TH1D("CosAngleHistogramFromData","CosAngleHistogramFromData",200,-1,1);
  h->Reset();
  
  printf("Program sorts histogram of angles between hits in the CsIArray.\n");
  
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
  
  sort(name);
  
  /*if((output=fopen("CsIArray_ECalRing.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file!");
      exit(EXIT_FAILURE);
    }

  fwrite(hist,NCSIRING*S32K*sizeof(int),1,output);
  fclose(output);*/
  
  //fill histogram of positions from combinatorics
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
			}
  
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("AngleHistogramFromData","AngleHistogramFromData",10,10, 500, 300);
  gPad->SetLogy(0);
  h->Draw();
  theApp->Run(kTRUE);
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("AngleHistogramFromPositions","AngleHistogramFromPositions",10,10, 500, 300);
  gPad->SetLogy(0);
  positionHist->Draw();
  theApp->Run(kTRUE);
  
  
  
  }
