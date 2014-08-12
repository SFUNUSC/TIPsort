#include "read_master.h"
/*******************************************************************/
void read_master(char *name, input_names_type *inp_names)
{
  FILE *inp;
  char str1[132],str2[132];


  if((inp=fopen(name,"r"))==NULL)
      {
         printf("\nFile %s can not be opened\n",name);
         exit(EXIT_FAILURE);
      }

  if(fgets(str1,132,inp)!=NULL)
    {
    if(fgets(str1,132,inp)!=NULL)
      while(fscanf(inp,"%s %s",str1,str2)!=EOF)
	{
	  
	  if(strcmp(str1,"input_data")==0)
	    {
	      inp_names->flag.inp_data=1;
	      strcpy(inp_names->fname.inp_data,str2);
	    }
	  if(strcmp(str1,"output_data")==0)
	    {
	      inp_names->flag.out_data=1;
	      strcpy(inp_names->fname.out_data,str2);
	    }
	  if(strcmp(str1,"cluster_file")==0)
	    {
	      inp_names->flag.cluster_file=1;
	      strcpy(inp_names->fname.cluster_file,str2);
	    }
	  if(strcmp(str1,"projection_cluster_file")==0)
	    {
	      inp_names->flag.pcluster_file=1;
	      strcpy(inp_names->fname.pcluster_file,str2);
	    }
	  if(strcmp(str1,"root_file")==0)
	    {
	      inp_names->flag.root_file=1;
	      strcpy(inp_names->fname.root_file,str2);
	    }
	  if(strcmp(str1,"root_gate_name")==0)
	    {
	      inp_names->flag.root_gate=1;
	      strcpy(inp_names->fname.root_gate_name,str2);
	    }
	  if(strcmp(str1,"PINARRAY_calibration_parameters")==0)
	    {
	      inp_names->flag.PINARRAY_cal_par=1;
	      strcpy(inp_names->fname.PINARRAY_cal_par,str2);
	    }
	  if(strcmp(str1,"CSIARRAY_calibration_parameters")==0)
	    {
	      inp_names->flag.CSIARRAY_cal_par=1;
	      strcpy(inp_names->fname.CSIARRAY_cal_par,str2);
	    }
	  if(strcmp(str1,"TIGRESS_calibration_parameters")==0)
	    {
	      inp_names->flag.TIGRESS_cal_par=1;
	      strcpy(inp_names->fname.TIGRESS_cal_par,str2);
	    }
	  if(strcmp(str1,"GRIFFIN_calibration_parameters")==0)
	    {
	      inp_names->flag.GRIFFIN_cal_par=1;
	      strcpy(inp_names->fname.GRIFFIN_cal_par,str2);
	    }
	  if(strcmp(str1,"S3_sector_calibration_parameters")==0)
	    {
	      inp_names->flag.S3sec_cal_par=1;
	      strcpy(inp_names->fname.S3sec_cal_par,str2);
	    }
	  if(strcmp(str1,"S3_ring_calibration_parameters")==0)
	    {
	      inp_names->flag.S3ring_cal_par=1;
	      strcpy(inp_names->fname.S3ring_cal_par,str2);
	    }
	} 
	  else
	    {
	      printf("Wrong structure of file %s\n",name);
	      printf("Aborting sort\n");
	      exit(1);
	    }
	  
    }
  else
    {
      printf("Wrong structure of file %s\n",name);
      printf("Aborting sort\n");
      exit(1);
    }
}      
