#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos,col,hp;
  double e,t;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  if(cev->tg.h.FE>0)
    for(pos=1;pos<NPOSTIGR;pos++)
      if((cev->tg.h.EHP&(1<<(pos-1)))!=0)
  	if(cev->tg.det[pos].hge.FE>0)
	  {
	    hp=0;
	    for(col=0;col<NCOL;col++)
  	    if((cev->tg.det[pos].hge.EHP&(1<<col))!=0)
  	      if(cev->tg.det[pos].ge[col].h.FE>0)
  		if((cev->tg.det[pos].ge[col].h.EHP&1)!=0)
  		  {
  		    e=cev->tg.det[pos].ge[col].seg[0].E;
		    if(e>cal_par->tg.celow[pos][col])
		      if(e<cal_par->tg.cehigh[pos][col])
			{
			  t=cev->tg.det[pos].ge[col].seg[0].T;
			  if(t>cal_par->tg.ctlow[pos][col])
			    if(t<cal_par->tg.cthigh[pos][col])
			      {
				hp|=(1<<col);
			      }
	
			}
  		  }
	    hist[pos][hp]++;
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
      printf("\n ./TIGRESS_ColorFold master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts calibrated energy histogram for TIGRESS cores \n");
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

  if((output=fopen("color_fold.spn","w"))==NULL)
    {
      printf("\nI can't open file color_fold.spn\n");
      exit(EXIT_FAILURE);
    }
	  
  fwrite(hist,NPOSTIGR*S4K*sizeof(int),1,output);
	 
  fclose(output);



}

