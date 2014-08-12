#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos,col,pin,ring;
  double ttg,tpin,epin,etg,ene;


  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_PINARRAY(data,&cal_par->pinarray,&cev->pinarray);


 
  if(cev->tg.h.FH>0)
    if(cev->pinarray.h.FH>0)
      for(pos=1;pos<NPOSTIGR;pos++)
	if((cev->tg.h.HHP&(1<<(pos-1)))!=0)
	  if(cev->tg.det[pos].hge.FH>0)
	    if(cev->tg.det[pos].suppress==0)
	      {
		col=cev->tg.det[pos].addbackC;
		ttg=cev->tg.det[pos].addback.T;
		etg=cev->tg.det[pos].addback.E;
		ring=cev->tg.det[pos].ge[col].ring;
		ene=etg/cal_par->tg.contr_e;
		if(ene>0)
		  if(ene<S32K)
		    if(ring>0)
		      if(ring<NRING)
			hist[ring][(int)rint(ene)]++;

		if(ttg>cal_par->tg.ctlow[pos][col])
		  if(ttg<cal_par->tg.cthigh[pos][col])
		    if(etg>cal_par->tg.celow[pos][col])
		      if(etg<cal_par->tg.cehigh[pos][col])
			for(pin=1;pin<NPIN;pin++)
			  if((cev->pinarray.h.HHP&(one<<pin))!=0)
			    {
			      tpin=cev->pinarray.pin[pin].T;
			      epin=cev->pinarray.pin[pin].E;
			      if(tpin>cal_par->pinarray.tlow[pin])
				if(tpin<cal_par->pinarray.thigh[pin])
				  if(epin>cal_par->pinarray.elow[pin])
				    if(epin<cal_par->pinarray.ehigh[pin])
				      {
					ene=etg/cal_par->tg.contr_e;
					if(ene>0)
					  if(ene<S32K)
					    if(ring>0)
					      if(ring<NRING)
						hist[ring][(int)rint(ene)]--;
				      }
			    }
	      }
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  FILE* output,*cluster;
  char n[132];

  if(argc!=2)
    {
      printf("\n ./Tigress_PINARRAY_AddBackSuppRing_RandomCluster master_file_name\n");
      exit(-1);
    }
  

  printf("Program sorts calibrated TIGRESS ring suppressed addback spectra outside of the gate on PINARRAY and TIGRESS energies and times \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);

   if(name->flag.cluster_file==1)
        {
          printf("\nSorting data from cluster file:\n %s\n",name->fname.cluster_file);
	  if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	    {
	      printf("\nI can't open input file %s\n",name->fname.cluster_file);
	      exit(-2);
	    }
        }
      else
        {
          printf("\nCluster file not defined\n");
          exit(-1);
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

  if(name->flag.PINARRAY_cal_par==1)
        {
          printf("\nPINARRAY calibration read from the file:\n %s\n",name->fname.PINARRAY_cal_par);
          initialize_PINARRAY_calibration(&cal_par->pinarray,name->fname.PINARRAY_cal_par);
	  
        }
      else
        {
          printf("\nPINARRAY calibration parameters not defined\n");
          exit(-1);
        }



  name->flag.inp_data=1; 
  while(fscanf(cluster,"%s",n)!=EOF)
    {
      strcpy(name->fname.inp_data,n);
      sort(name);
    }
  fclose(cluster);


  if((output=fopen("random_rings.mca","w"))==NULL)
    {
      printf("\nI can't open file random_rings.mca\n");
      exit(EXIT_FAILURE);
    }
	  
  fwrite(hist,NRING*S32K*sizeof(int),1,output);
	 
  fclose(output);


}

