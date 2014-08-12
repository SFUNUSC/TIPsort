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
  char PINARRAY_cal_par[132];
  char CSIARRAY_cal_par[132];
  char TIGRESS_cal_par[132];
  char GRIFFIN_cal_par[132];
  char S3sec_cal_par[132];
  char S3ring_cal_par[132];
  char cluster_file[132];
  char pcluster_file[132];
  char root_file[132];
  char root_gate_name[132];
};

struct flag
{
  int inp_data;
  int out_data;
  int separate;
  int PINARRAY_cal_par;
  int CSIARRAY_cal_par;
  int TIGRESS_cal_par;
  int GRIFFIN_cal_par;
  int S3sec_cal_par;
  int S3ring_cal_par;
  int cluster_file;
  int pcluster_file;
  int root_file;
  int root_gate;
};

typedef struct
{
  struct fname fname;
  struct flag flag;
}input_names_type;

void read_master(char *name, input_names_type *inp_names);


#endif
