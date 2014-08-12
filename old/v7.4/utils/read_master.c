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
	  if(strcmp(str1,"output_data_list")==0)
	    {
	      inp_names->flag.out_data_list=1;
	      strcpy(inp_names->fname.out_data_list,str2);
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
	  if(strcmp(str1,"root_output_file")==0)
	    {
	      inp_names->flag.root_output_file=1;
	      strcpy(inp_names->fname.root_output_file,str2);
	    }
	  if(strcmp(str1,"root_gate_file")==0)
	    {
	      inp_names->flag.root_gate_file=1;
	      strcpy(inp_names->fname.root_gate_file,str2);
	    }
	  if(strcmp(str1,"gate_name_file")==0)
	    {
	      inp_names->flag.gate_name_file=1;
	      strcpy(inp_names->fname.gate_name_file,str2);
	    }
	  if(strcmp(str1,"PINARRAY_calibration_parameters")==0)
	    {
	      inp_names->flag.PINARRAY_cal_par=1;
	      strcpy(inp_names->fname.PINARRAY_cal_par,str2);
	    }
	  if(strcmp(str1,"PINArray_RFunwrapping_offset")==0)
	    {
	      inp_names->flag.PINArray_RFunwrapping_par1=1;
	      inp_names->param.PINArray_offset=atof(str2);
	    }
	  if(strcmp(str1,"PINArray_RFunwrapping_shift")==0)
	    {
	      inp_names->flag.PINArray_RFunwrapping_par2=1;
	      inp_names->param.PINArray_shift=atof(str2);
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
	  if(strcmp(str1,"TIGRESS_RFunwrapping_offset")==0)
	    {
	      inp_names->flag.TIGRESS_RFunwrapping_par1=1;
	      inp_names->param.TIGRESS_offset=atof(str2);
	    }
	  if(strcmp(str1,"TIGRESS_RFunwrapping_shift")==0)
	    {
	      inp_names->flag.TIGRESS_RFunwrapping_par2=1;
	      inp_names->param.TIGRESS_shift=atof(str2);
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
	  if(strcmp(str1,"S3_sector_RFunwrapping_offset")==0)
	    {
	      inp_names->flag.S3sec_RFunwrapping_par1=1;
	      inp_names->param.sec_offset=atof(str2);
	    }
	  if(strcmp(str1,"S3_sector_RFunwrapping_shift")==0)
	    {
	      inp_names->flag.S3sec_RFunwrapping_par2=1;
	      inp_names->param.sec_shift=atof(str2);
	    }
	  if(strcmp(str1,"S3_ring_RFunwrapping_offset")==0)
	    {
	      inp_names->flag.S3ring_RFunwrapping_par1=1;
	      inp_names->param.ring_offset=atof(str2);
	    }
	  if(strcmp(str1,"S3_ring_RFunwrapping_shift")==0)
	    {
	      inp_names->flag.S3ring_RFunwrapping_par2=1;
	      inp_names->param.ring_shift=atof(str2);
	    }}
    else
      {
	printf("Wrong structure of file %s\n",name);
	printf("Aborting sort\n");
	exit(1);
      }}
  else
    {
      printf("Wrong structure of file %s\n",name);
      printf("Aborting sort\n");
      exit(1);
    }}
