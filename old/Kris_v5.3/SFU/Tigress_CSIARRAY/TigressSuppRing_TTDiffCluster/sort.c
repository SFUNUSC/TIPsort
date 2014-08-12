#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int pos,col,csi,ring;
  double ttg,tcsi,t,etg;
  
 
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
   for(csi=1;csi<NCSI;csi++)
      if((data->csiarray.h.THP&(one<<csi))!=0)
	{
	  tcsi=data->csiarray.csi[csi].cfd&0x00ffffff;
	  tcsi-=(data->csiarray.csi[csi].timestamp*16)&0x00ffffff;
	  if(data->tg.h.Gefold>0)
	    for(pos=1;pos<NPOSTIGR;pos++)
	      if((data->tg.h.GeHP&(1<<(pos-1)))!=0)
		if(data->tg.det[pos].h.Gefold>0)
		  for(col=0;col<NCOL;col++)
		    if((data->tg.det[pos].h.GeHP&(1<<col))!=0)
		      if(data->tg.det[pos].ge[col].h.Tfold>0)
			if((data->tg.det[pos].ge[col].h.THP&1)!=0)
			  {
			    ttg=data->tg.det[pos].ge[col].seg[0].cfd&0x00ffffff;
			    ttg-=(data->tg.det[pos].ge[col].seg[0].timestamp*16)
&0x00ffffff;
			    t=ttg-tcsi;
			    t*=10;
			    t/=16;
			    t+=S1K/2;
			    if(t<0)t=S4K-2;
			    if(t>S4K-10) t=S4K-10;

			    if(t>cal_par->tg.ctlow[pos][col])
			      if(t<cal_par->tg.cthigh[pos][col])
				{
				  ring=cev->tg.det[pos].ge[col].ring;
				  etg=cev->tg.det[pos].ge[col].seg[0].E;
				  etg/=cal_par->tg.contr_e;
				  if(etg>0)
				    if(etg<S4K)
				      if(ring>0)
					if(ring<NRING)
					  {
					    hist[0][(int)rint(t)]++;
					    hist[ring][(int)rint(etg)]++;
					  }

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
      printf("\n ./Tigress_CSIARRAY_TigressRing_TTDiffCluster master_file_name\n");
      exit(-1);
    }
  

  printf("Program sorts calibrated TIGRESS ring spectra gated on CSIARRAY and TIGRESS energies and times \n");
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
	 
	  printf("TIGRESS calibration initialized\n");
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



  name->flag.inp_data=1; 
  while(fscanf(cluster,"%s",n)!=EOF)
    {
      strcpy(name->fname.inp_data,n);
      sort(name);
    }
  fclose(cluster);


  if((output=fopen("gated_rings.spn","w"))==NULL)
    {
      printf("\nI can't open file ring_cal_core_energy.spn\n");
      exit(EXIT_FAILURE);
    }
	  
  fwrite(hist,NRING*S4K*sizeof(int),1,output);
	 
  fclose(output);


}

