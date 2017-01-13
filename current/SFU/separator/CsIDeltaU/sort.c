#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int csi;
  int pos1,pos2,ring;
  double u;

  long long int one=1,none=-1,kill;
  long long int flag_csi;
  
  if((data->h.setupHP&RF_BIT)==0)
    return SEPARATOR_DISCARD;
  
  if((data->h.setupHP&CsIArray_BIT)==0)
    return SEPARATOR_DISCARD;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);

  flag_csi=0;

  if(cev->csiarray.h.FE==2)
    if(cev->csiarray.h.FT==2)
      for(pos1=1;pos1<NCSI;pos1++)
        if((cev->csiarray.h.EHP&(one<<pos1))!=0)
          {
            ring=cev->csiarray.ring[pos1];//check the ring position of the first hit
            for(pos2=pos1+1;pos2<NCSI;pos2++)
	      if((cev->csiarray.h.THP&(one<<pos2))!=0)
                if((ring==cev->csiarray.ring[pos2])||(same_ring!=1))//check that the 2 hits are in the same ring
                  {
	            u=cev->csiarray.U;
                    if(u>=low)
                      if(u<=high)
                        {
                          flag_csi|=(one<<pos1); //flag the hits for preservation
                          flag_csi|=(one<<pos2);
                        }
                  }
          }

  free(cev);
  
  
  //drop unflagged csi hits
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
  
  if((argc!=5)&&(argc!=4))
    {
      printf("\n ./separate_CsIArray_DeltaU master_file_name low high same_ring\n");
      printf("\n Separates out events with 2 CsI hits where the deltaU value falls between the low and high paremters.\n");
      printf("same_ring: if 1, the CsI hits must be in the same ring.  Optional parameter, defaults to 0.\n");
      exit(-1);
    }
  if(argc==4)
    same_ring=0;
  else
    same_ring=atoi(argv[4]);
  
  low=atof(argv[2]);
  high=atof(argv[3]);
  //printf("Program sorts calibrated 2D histogram for CsIArray timing.\n");
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
  
  /*theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("TTCalDiff", "TTCalDiff",10,10, 500, 300);
  gPad->SetLogz(1);
  g->SetLineColor(2);
  h->Draw();
  g->Draw("same");
  theApp->Run(kTRUE);*/
}
