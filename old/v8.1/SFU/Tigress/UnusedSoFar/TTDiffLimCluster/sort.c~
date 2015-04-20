#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos1,pos2;
  double t1,t2,tx,ty;

  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);

  if(cev->tg.h.FH>1)
    for(pos1=1;pos1<NPOSTIGR;pos1++)
      if((cev->tg.h.HHP&(1<<(pos1-1)))!=0)
	if(cev->tg.det[pos1].hge.FH>0)
	  {
	    t1=cev->tg.det[pos1].addback.T;
	    for(pos2=pos1+1;pos2<NPOSTIGR;pos2++)
	      if((cev->tg.h.HHP&(1<<(pos2-1)))!=0)
		if(cev->tg.det[pos2].hge.FH>0)
		  {
		    t2=cev->tg.det[pos1].addback.T;
		    tx=t1-t2+S2K;
		    ty=t2-t1+S2K;
		    if(tx>=0)
		      if(tx<S4K)
			hist[(int)rint(tx)]++;
		    if(ty>=0)
		      if(ty<S4K)
			hist[(int)rint(ty)]++;
		  }
	  }


  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;


  if(argc!=2)
    {
      printf("\n ./TIGRESS_TTDiffLim master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts calibrated time histogram for TIGRESS cores \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  memset(hist,0,sizeof(hist));
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
          exit(EXIT_FAILURE);
        }



  sort(name);


  if((output=fopen("TTCalDiff.spn","w"))==NULL)
    {
      printf("\nI can't open file TTCalDiff.spn\n");
      exit(EXIT_FAILURE);
    }
	  
  fwrite(hist,S4K*sizeof(int),1,output);
  fclose(output);
  
}

  

