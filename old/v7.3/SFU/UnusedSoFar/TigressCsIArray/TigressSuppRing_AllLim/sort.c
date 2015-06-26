#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos,col,csi,ring;
  double ttg,tcsi,ecsi,etg;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);

  if(cev->tg.h.FH>0)
    if(cev->csiarray.h.FH>0)
      for(pos=1;pos<NPOSTIGR;pos++)
	if((cev->tg.h.HHP&(1<<(pos-1)))!=0)
	  if(cev->tg.det[pos].hge.FH>0)
	    if(cev->tg.det[pos].suppress==0)
	    for(col=0;col<NCOL;col++)
	      if((cev->tg.det[pos].hge.HHP&(1<<col))!=0)
		if(cev->tg.det[pos].ge[col].h.FH>0)
		  if((cev->tg.det[pos].ge[col].h.HHP&1)!=0)
		    {
		      ttg=cev->tg.det[pos].ge[col].seg[0].T;
		      etg=cev->tg.det[pos].ge[col].seg[0].E;
		      ring=cev->tg.det[pos].ge[col].ring;
		      if(ttg>cal_par->tg.ctlow[pos][col])
			if(ttg<cal_par->tg.cthigh[pos][col])
			  if(etg>cal_par->tg.celow[pos][col])
			    if(etg<cal_par->tg.cehigh[pos][col])
			      for(csi=1;csi<NCSI;csi++)
				if((cev->csiarray.h.HHP&(one<<csi))!=0)
				  {
				    tcsi=cev->csiarray.csi[csi].T;
				    ecsi=cev->csiarray.csi[csi].E;
				    if(tcsi>cal_par->csiarray.tlow[csi])
				      if(tcsi<cal_par->csiarray.thigh[csi])
					if(ecsi>cal_par->csiarray.elow[csi])
					  if(ecsi<cal_par->csiarray.ehigh[csi])
					    {
					      etg/=cal_par->tg.contr_e;
					      if(etg>0)
						if(etg<S4K)
						  if(ring>0)
						    if(ring<NRING)
						      hist[ring][(int)rint(etg)]++;

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
  FILE* output;
 
  if(argc!=2)
    {
      printf("\n ./Tigress_CSIARRAY_TigressRing_AllLim master_file_name\n");
      exit(-1);
    }
  

  printf("Program sorts calibrated TIGRESS ring spectra gated on CSIARRAY and TIGRESS energies and times \n");
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



  sort(name);

  if((output=fopen("gated_rings.spn","w"))==NULL)
    {
      printf("\nI can't open file ring_cal_core_energy.spn\n");
      exit(EXIT_FAILURE);
    }
	  
  fwrite(hist,NRING*S4K*sizeof(int),1,output);
	 
  fclose(output);


}
