#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  
  long long int one=1,none=-1,kill;
  int id,id_ge;
  int pos1;
  
  if((data->h.setupHP&TIGRESS_BIT)==0)
    return SEPARATOR_DISCARD;

  //if((data->h.setupHP&RF_BIT)==0)
  //  return SEPARATOR_DISCARD;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  //drop any data associated with the specified crystal
  if(cev->tg.h.FT>0)
    if((cev->tg.h.THP&(1<<(pos-1)))!=0)
      if(cev->tg.det[pos].hge.FT>0)
        if((cev->tg.det[pos].hge.THP&(1<<col))!=0)
          if(cev->tg.det[pos].ge[col].h.FT>0)
            if((cev->tg.det[pos].ge[col].h.THP&1)!=0)
              {
                //drop this crystal
                id=pos-1;
                id_ge=id*NCOL+col;
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

  free(cev);
  
  for(pos1=1;pos1<NPOSTIGR;pos1++)
    {
      id=pos1-1;
      if((data->tg.h.GeHP&(1<<id))!=0)
        if(data->tg.det[pos1].h.Gefold<=0)
          {
            //drop this position
            memset(&data->tg.det[pos1],0,sizeof(CssTIGR));
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
    
    
  if((data->h.setupHP&TIGRESS_BIT)==0)
    return SEPARATOR_DISCARD;
    
  //if((data->h.setupHP&RF_BIT)==0)
  //  return SEPARATOR_DISCARD; 
  
  /*//check that there are 2 Tigress events 
	if(data->tg.h.Gefold<2)
		return SEPARATOR_DISCARD;*/
    
  encode(data,output,enb);
  
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;

  if(argc!=4)
    {
      printf("separate_Tigress_DropCore master_file_name pos col\n");
      printf("\n Separator which drops any hits in the specified Tigress core.\n");
      exit(-1);
    }
  
  pos=atoi(argv[2]);
  col=atoi(argv[3]);
  
  printf("Program drops any hits in the specified Tigress core (core #%i, col#%i).\n",pos,col);
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
}

