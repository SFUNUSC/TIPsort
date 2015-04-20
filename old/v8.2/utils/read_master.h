#ifndef READ_MASTER_H
#define READ_MASTER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

struct fname
{
  char inp_data[132];
  char out_data[132];
  char out_data_list[132];
  char PINARRAY_cal_par[132];
  char CSIARRAY_cal_par[132];
  char TIGRESS_cal_par[132];
  char GRIFFIN_cal_par[132];
  char S3sec_cal_par[132];
  char S3ring_cal_par[132];
  char S3sec_RFunwrapping_par1[132];
  char S3sec_RFunwrapping_par2[132];
  char S3ring_RFunwrapping_par1[132];
  char S3ring_RFunwrapping_par2[132];
  char PINArray_RFunwrapping_par1[132];
  char PINArray_RFunwrapping_par2[132];
  char TIGRESS_RFunwrapping_par1[132];
  char TIGRESS_RFunwrapping_par2[132];
  char cluster_file[132];
  char pcluster_file[132];
  char root_output_file[132];
  char root_gate_file[132];
  char gate_name_file[132];
};

struct param
{
  int    ring_offset;
  int    ring_shift;
  int    sec_offset;
  int    sec_shift;
  int    PINArray_offset;
  int    PINArray_shift;
  int    TIGRESS_offset;
  int    TIGRESS_shift;
};

struct flag
{
  int inp_data;
  int out_data;
  int out_data_list;
  int separate;
  int PINARRAY_cal_par;
  int CSIARRAY_cal_par;
  int TIGRESS_cal_par;
  int GRIFFIN_cal_par;
  int S3sec_cal_par;
  int S3ring_cal_par;
  int S3sec_RFunwrapping_par1;
  int S3sec_RFunwrapping_par2;
  int S3ring_RFunwrapping_par1;
  int S3ring_RFunwrapping_par2;
  int PINArray_RFunwrapping_par1;
  int PINArray_RFunwrapping_par2;
  int TIGRESS_RFunwrapping_par1;
  int TIGRESS_RFunwrapping_par2;
  int cluster_file;
  int pcluster_file;
  int root_output_file;
  int root_gate_file;
  int gate_name_file;
};

typedef struct
{
  struct fname fname;
  struct flag flag;
  struct param param;
}input_names_type;

void read_master(char *name, input_names_type *inp_names);


#endif
