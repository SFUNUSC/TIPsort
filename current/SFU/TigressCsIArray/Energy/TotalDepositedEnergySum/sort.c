#include "sort.h"

int analyze_data(raw_event *data) {
  cal_event *cev;
  uint64_t one = 1;
  int pos, csi;
  double etig, ecsi;
  double etottig = 0.;
  double etotcsi = 0.;
  double etot = 0.;

  cev = (cal_event *)malloc(sizeof(cal_event));
  memset(cev, 0, sizeof(cal_event));
  calibrate_CSIARRAY(data, &cal_par->csiarray, &cev->csiarray);
  calibrate_TIGRESS(data, &cal_par->tg, &cev->tg);

  if ((data->h.setupHP & TIGRESS_BIT) == 0)
    return SEPARATOR_DISCARD;

  // if((data->h.setupHP&RF_BIT)==0)
  //   return SEPARATOR_DISCARD;

  if ((data->h.setupHP & CsIArray_BIT) == 0)
    return SEPARATOR_DISCARD;

  if (cev->tg.h.FH > 0)
    for (pos = 1; pos < NPOSTIGR; pos++)
      if (((cev->tg.h.HHP & (1 << (pos - 1))) != 0) &&
          (cev->tg.det[pos].hge.FE > 0))
        if ((cev->tg.h.AHP & (1 << (pos - 1))) != 0) {
          etig = cev->tg.det[pos].addback.E; /* TIGRESS energy in keV */
          etottig += etig;
          etot += etig;
        }
  if (cev->csiarray.h.FH > 0)
    for (csi = 1; csi < NCSI; csi++)
      if ((cev->csiarray.h.HHP[csi / 64] & (one << csi % 64)) != 0) {
        // ecsi=cev->csiarray.csi[csi].E/1000.; /* CsI energy in MeV */
        ecsi = cev->csiarray.csi[csi].E; /* CsI energy in keV */
        etotcsi += ecsi;
        etot += ecsi;
      }
  
  etot = etot/cntr;//energy contraction
  etottig = etottig/cntr;//energy contraction
  etotcsi = etotcsi/cntr;//energy contraction

  if (etot < 0 || etot > S32K - 10)
    etot = S32K - 10;

  hist[0][(int)(etot)]++;
  hist[1][(int)(etottig)]++;
  hist[2][(int)(etotcsi)]++;

  free(cev);
  return SEPARATOR_DISCARD;
}
/*=============================================================================*/
int main(int argc, char *argv[]) {
  FILE *cluster, *output;
  input_names_type *name;
  char DataFile[132];

  if (argc != 2) {
    printf("TigressCsIArray_TotalDepositedEnergySum "
           "master_file_name\n");
    exit(-1);
  }

  printf("Program sorts total energy deposit histograms for TIGRESS/CSIARRAY events.\n");
  name = (input_names_type *)malloc(sizeof(input_names_type));
  memset(name, 0, sizeof(input_names_type));

  cal_par = (calibration_parameters *)malloc(sizeof(calibration_parameters));
  memset(cal_par, 0, sizeof(calibration_parameters));

  memset(hist, 0, sizeof(hist));

  read_master(argv[1], name);
  cntr=2.0;

  if (name->flag.cluster_file == 1) {
    printf("Sorting based upon the cluster file: %s\n",
           name->fname.cluster_file);
    if ((cluster = fopen(name->fname.cluster_file, "r")) == NULL) {
      printf("ERROR! I can't open input file %s\n", name->fname.cluster_file);
      exit(-2);
    }
  } else {
    printf("ERROR! Cluster file not defined\n");
    exit(-1);
  }

  if (name->flag.TIGRESS_cal_par == 1) {
    printf("TIGRESS calibration read from %s.\n", name->fname.TIGRESS_cal_par);
    initialize_TIGRESS_calibration(&cal_par->tg, name->fname.TIGRESS_cal_par);
  }
  if (name->flag.CSIARRAY_cal_par == 1) {
    printf("CsIArray calpar read from: %s\n", name->fname.CSIARRAY_cal_par);
    initialize_CSIARRAY_calibration(&cal_par->csiarray,
                                    name->fname.CSIARRAY_cal_par);
  } else {
    printf("ERROR!!! TIGRESS calibration parameters not defined!\n");
    exit(EXIT_FAILURE);
  }

  while (fscanf(cluster, "%s", DataFile) != EOF) {
    memset(name, 0, sizeof(input_names_type));
    strcpy(name->fname.inp_data, DataFile);

    printf("Sorting data from %s\n", name->fname.inp_data);
    sort(name);
  }

  if ((output = fopen("TotalDepositedEnergySum.mca", "w")) == NULL) {
    printf("ERROR!!! I cannot open the mca file!\n");
    exit(EXIT_FAILURE);
  }

  fwrite(hist, 3 * S32K * sizeof(int), 1, output);
  fclose(output);
  printf("Output spectrum wriiten to: TotalDepositedEnergySum.mca\n");
  printf("Energy contraction: %f keV/ch\n",cntr);
  printf("Spectrum 0: total energy\n");
  printf("Spectrum 1: energy in Tigress\n");
  printf("Spectrum 2: energy in CsI array\n");

  return 0;
}
