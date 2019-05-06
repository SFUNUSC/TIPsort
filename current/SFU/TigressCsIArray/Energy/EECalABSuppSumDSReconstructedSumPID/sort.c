#include "sort.h"

int analyze_data(raw_event *data) {
  cal_event *cev;
  double eAddBack = 0;
  double ecsi, pval;
  int csi;
  uint64_t one = 1;
  int pos1, pos2, col1, col2;
  // double t1,t2,t;
  int e1, e2;
  double s,f,r,e;
  int type;

  int suppFlag1 = 0;
  int suppFlag2 = 0;

  // do i need this?
  int take1 = 0;
  int take2 = 0;

  e1 = 0.;
  e2 = 0.;

  cev = (cal_event *)malloc(sizeof(cal_event));
  memset(cev, 0, sizeof(cal_event));
  memset(beam_p, 0, sizeof(beam_p));
  memset(part_p, 0, sizeof(part_p));
  memset(gamma_dir, 0, sizeof(gamma_dir));

  calibrate_TIGRESS(data, &cal_par->tg, &cev->tg);
  calibrate_CSIARRAY(data, &cal_par->csiarray, &cev->csiarray);

  // get beam momentum value from calibration (energy specified in parameter
  // file)
  beam_p[2] = sqrt(((cal_par->csiarray.Ebeam + cal_par->csiarray.mproj) *
                    (cal_par->csiarray.Ebeam + cal_par->csiarray.mproj)) -
                   (cal_par->csiarray.mproj *
                    cal_par->csiarray.mproj)); // momentum of incoming beam,
                                               // relativistic, from KE = mc^2 +
                                               // m0c^2, mc^2 = sqrt(p^2c^2 +
                                               // m0^2c^4)
  // printf("Beam p: %f\n",beam_p[2]);
  // getc(stdin);

  // check addback fold
  // printf("FH = %d FA= %d\n",cev->tg.h.FH,cev->tg.h.FA);
  // getc(stdin);

  //get the particle types from PID-ER
  if (cev->csiarray.h.FH > 0)
    for (csi = 1; csi < NCSI; csi++)
      if ((cev->csiarray.h.HHP[csi / 64] & (one << csi % 64)) != 0) {

        //printf("csi = %i\n",csi);
        
        type=data->csiarray.wfit[csi].type;
        if(type==1)
          {
            if(useCharge==1)
              e=data->csiarray.csi[csi].charge;
            else
              e=data->csiarray.wfit[csi].am[1];
            s=data->csiarray.wfit[csi].am[3];
            f=data->csiarray.wfit[csi].am[2];
          
            if(f==0)
              r=0;
            else
              r=s/f*100;
          
            r+=100;

            if((pGateFlag[csi]==1)&&(pGate[csi]->IsInside(e,r)))
              {
                part_type[csi] = 1; //proton
              }
            else if((aGateFlag[csi]==1)&&(aGate[csi]->IsInside(e,r)))
              {
                part_type[csi] = 2; //alpha
              }
            else
              {
                printf("Unidentified particle! e = %f, r = %f\n",e,r);
                printf("gateFlags = %i, %i\n",pGateFlag[csi],aGateFlag[csi]);
                part_type[csi] = 0;
                getc(stdin);
              }
            
          }
      }

  if (cev->tg.h.FA > 0) // addback fold>0
    // look through each Tigress position
    for (pos1 = 1; pos1 < NPOSTIGR; pos1++) {
      // reset suppression flag for every position
      suppFlag1 = 0;
      // check if the position is in the hit pattern
      if ((cev->tg.h.HHP & (1 << (pos1 - 1))) != 0)
        // check the position fold
        if (cev->tg.det[pos1].hge.FH > 0)
          // check if the position is in the addback hit pattern
          if ((cev->tg.h.AHP & (1 << (pos1 - 1))) != 0) {
            // reset take for add-back suppression
            take1 = 0;
            // Run through four cores for each position
            for (col1 = 0; col1 < NCOL; col1++) {
              // check if the position and color is in the hit pattern
              if ((cev->tg.det[pos1].hge.HHP & (1 << col1)) != 0)
                // check the fold
                if (cev->tg.det[pos1].ge[col1].h.FH > 0) {
                  // suppress if the position is in the map and has not yet been
                  // suppressed
                  if (cev->tg.det[pos1].ge[col1].suppress >= supLow &&
                      cev->tg.det[pos1].ge[col1].suppress <= supHigh &&
                      take1 == 0) {
                    /* once suppression flag is set
                    do not reset it, could remove the take bit
                    and keep resetting suppFlag, but this
                    is nicer */
                    suppFlag1 = 1;
                    take1 = 1;
                  }
                }
            }

            // get energy for the first position
            eAddBack = cev->tg.det[pos1].addback.E; //don't apply contraction factor here
            colAddBack = cev->tg.det[pos1].addbackC;

            if (cev->csiarray.h.FH > 0)
              for (csi = 1; csi < NCSI; csi++)
                if ((cev->csiarray.h.HHP[csi / 64] & (one << csi % 64)) != 0) {

                  if(part_type[csi] == 1){
                    cal_par->csiarray.mp = amuMeV*1.00727647; //proton mass
                  }else if(part_type[csi] == 2){
                    cal_par->csiarray.mp = amuMeV*4.0012; //alpha mass
                  }else{
                    printf("Unknown particle, assigning large mass.\n");
                    cal_par->csiarray.mp = amuMeV*1000.0; //unknown particle
                  }

                  for (int ind = 0; ind < 3; ind++){
                    part_dir[csi][ind] = cal_par->csiarray.cpos_xyz[csi][ind];
                  }
                  vecMag = sqrt(part_dir[csi][0] * part_dir[csi][0] +
                                part_dir[csi][1] * part_dir[csi][1] +
                                part_dir[csi][2] * part_dir[csi][2]);
                  for (int ind = 0; ind < 3; ind++){
                    part_dir[csi][ind] = part_dir[csi][ind] / vecMag; // make vector unit length
                  }

                  ecsi = cev->csiarray.csi[csi].E / 1000.; /* CsI energy in MeV */
                  pval = sqrt(
                      ((ecsi + cal_par->csiarray.mp) *
                       (ecsi + cal_par->csiarray.mp)) -
                      (cal_par->csiarray.mp *
                       cal_par->csiarray.mp)); // relativistic p (magnitude)

                  for (int ind = 0; ind < 3; ind++)
                    part_p[csi][ind] = pval * part_dir[csi][ind];

                  // printf("ecsi: %f MeV, pval: %f MeV/c, part_p: [%f %f %f]
                  // MeV/c\n",ecsi,pval,part_p[csi][0],part_p[csi][1],part_p[csi][2]);
                  // getc(stdin);
                }

            // subtract detected particle momenta from beam momentum to get
            // residual momentum
            for (int ind = 0; ind < 3; ind++)
              res_p[ind] = beam_p[ind];
            for (csi = 1; csi < NCSI; csi++)
              for (int ind = 0; ind < 3; ind++)
                res_p[ind] = res_p[ind] - part_p[csi][ind];
            for (int ind = 0; ind < 3; ind++)
              res_p[ind] = res_p[ind] * fudgeFactor;

            // calculate speed of residual
            pval = sqrt(res_p[0] * res_p[0] + res_p[1] * res_p[1] +
                        res_p[2] * res_p[2]);
            beta = sqrt(
                1.0 -
                ((cal_par->csiarray.mr * cal_par->csiarray.mr) /
                 ((pval * pval) +
                  (cal_par->csiarray.mr *
                   cal_par->csiarray.mr)))); // relativistic calculation of beta
            // printf("beta: %f\n",beta);

            // construct unit vectors for gamma and residual (source) nucleus
            for (int ind = 0; ind < 3; ind++)
              gamma_dir[ind] = cal_par->tg.tpos_xyz[pos1][colAddBack][ind];
            vecMag =
                sqrt(gamma_dir[0] * gamma_dir[0] + gamma_dir[1] * gamma_dir[1] +
                     gamma_dir[2] * gamma_dir[2]);
            for (int ind = 0; ind < 3; ind++)
              gamma_dir[ind] =
                  gamma_dir[ind] / vecMag; // make unit vector for gamma ray
            vecMag = sqrt(res_p[0] * res_p[0] + res_p[1] * res_p[1] +
                          res_p[2] * res_p[2]);
            for (int ind = 0; ind < 3; ind++)
              res_dir[ind] =
                  res_p[ind] / vecMag; // make unit vector for residual nucleus

            // calculate ds
            ds = sqrt(1 - (beta * beta)) /
                 (1 - (beta *
                       (res_dir[0] * gamma_dir[0] + res_dir[1] * gamma_dir[1] +
                        res_dir[2] * gamma_dir[2])));
            // ds=1+(beta* (res_dir[0]*gamma_dir[0] + res_dir[1]*gamma_dir[1] +
            // res_dir[2]*gamma_dir[2]) );

            /*printf("ds: %f\n",ds);
            printf("Tigress pos: %i\n",pos1);
            printf("beta: %f, gamma_dir: [%f %f
            %f]\n",beta,gamma_dir[0],gamma_dir[1],gamma_dir[2]);
            printf("res_p: [%f %f %f] MeV/c\n",res_p[0],res_p[1],res_p[2]);
            printf("res_pdir: [%f %f %f]\n",res_dir[0],res_dir[1],res_dir[2]);
            printf("beam_p: [%f %f %f] MeV/c\n",beam_p[0],beam_p[1],beam_p[2]);
            for(csi=1;csi<NCSI;csi++)
              if((part_p[csi][0]+part_p[csi][1]+part_p[csi][2])!=0)
                printf("In CsI # %i, part_p = [%f %f %f]
            MeV/c\n",csi,part_p[csi][0],part_p[csi][1],part_p[csi][2]);
            getc(stdin);*/

            if (ds != ds) {
              printf("ERROR: Doppler shift is undefined!\nHave you properly "
                     "specified all detector postions in the parameter "
                     "files?\n");
              printf("\nINFO DUMP\n---------\n");
              printf("ds: %f\n", ds);
              printf("Tigress pos: %i\n", pos1);
              printf("beta: %f, gamma_dir: [%f %f %f]\n", beta, gamma_dir[0],
                     gamma_dir[1], gamma_dir[2]);
              printf("res_p: [%f %f %f] MeV/c\n", res_p[0], res_p[1], res_p[2]);
              printf("res_pdir: [%f %f %f]\n", res_dir[0], res_dir[1],
                     res_dir[2]);
              printf("beam_p: [%f %f %f] MeV/c\n", beam_p[0], beam_p[1],
                     beam_p[2]);
              for (csi = 1; csi < NCSI; csi++)
                if ((part_p[csi][0] + part_p[csi][1] + part_p[csi][2]) != 0)
                  printf("In CsI # %i, part_p = [%f %f %f] MeV/c\n", csi,
                         part_p[csi][0], part_p[csi][1], part_p[csi][2]);
              printf("\n");
              exit(-1);
            }

            // printf("spectrum: %i, ch: %i\n",0+suppFlag,(int)(eAddBack/ds));
            eAddBack = eAddBack / ds;
						eAddBack = eAddBack / cntr;

            e1 = (int)eAddBack;

            memset(
                part_p, 0,
                sizeof(
                    part_p)); // reset particle momenta for the next calculation
            memset(gamma_dir, 0, sizeof(gamma_dir));

            for (pos2 = pos1 + 1; pos2 < NPOSTIGR; pos2++) {
              // reset suppression flag for every position
              suppFlag2 = 0;
              // check if the position is in the hit pattern
              if ((cev->tg.h.HHP & (1 << (pos2 - 1))) != 0)
                // check the position fold
                if (cev->tg.det[pos2].hge.FH > 0)
                  // check if the position is in the addback hit pattern
                  if ((cev->tg.h.AHP & (1 << (pos2 - 1))) != 0) {
                    // reset take for add-back suppression
                    take2 = 0;
                    // Run through four cores for each position
                    for (col2 = 0; col2 < NCOL; col2++) {
                      // check if the position and color is in the hit pattern
                      if ((cev->tg.det[pos2].hge.HHP & (1 << col2)) != 0)
                        // check the fold
                        if (cev->tg.det[pos2].ge[col2].h.FH > 0) {
                          // suppress if the position is in the map and has not
                          // yet been suppressed
                          if (cev->tg.det[pos2].ge[col2].suppress >= supLow &&
                              cev->tg.det[pos2].ge[col2].suppress <= supHigh &&
                              take2 == 0) {
                            /* once suppression flag is set
                                     do not reset it, could remove the take bit
                                     and keep resetting suppFlag, but this
                                     is nicer */
                            suppFlag2 = 1;
                            take2 = 1;
                          }
                        }
                    }

                    // get energy for the second position

                    eAddBack =
                        cev->tg.det[pos2].addback.E; //don't apply contraction factor here
                    colAddBack = cev->tg.det[pos2].addbackC;

                    if (cev->csiarray.h.FH > 0)
                      for (csi = 1; csi < NCSI; csi++)
                        if ((cev->csiarray.h.HHP[csi / 64] &
                             (one << csi % 64)) != 0) {
                          
                          if(part_type[csi] == 1){
                            cal_par->csiarray.mp = amuMeV*1.00727647; //proton mass
                          }else if(part_type[csi] == 2){
                            cal_par->csiarray.mp = amuMeV*4.0012; //alpha mass
                          }else{
                            printf("Unknown particle, assigning large mass.\n");
                            cal_par->csiarray.mp = amuMeV*1000.0; //unknown particle
                          }

                          for (int ind = 0; ind < 3; ind++)
                            part_dir[csi][ind] =
                                cal_par->csiarray.cpos_xyz[csi][ind];
                          vecMag = sqrt(part_dir[csi][0] * part_dir[csi][0] +
                                        part_dir[csi][1] * part_dir[csi][1] +
                                        part_dir[csi][2] * part_dir[csi][2]);
                          for (int ind = 0; ind < 3; ind++)
                            part_dir[csi][ind] =
                                part_dir[csi][ind] /
                                vecMag; // make vector unit length

                          ecsi = cev->csiarray.csi[csi].E /
                                 1000.; /* CsI energy in MeV */
                          pval = sqrt(((ecsi + cal_par->csiarray.mp) *
                                       (ecsi + cal_par->csiarray.mp)) -
                                      (cal_par->csiarray.mp *
                                       cal_par->csiarray
                                           .mp)); // relativistic p (magnitude)

                          for (int ind = 0; ind < 3; ind++)
                            part_p[csi][ind] = pval * part_dir[csi][ind];

                          // printf("ecsi: %f MeV, pval: %f MeV/c, part_p: [%f
                          // %f %f]
                          // MeV/c\n",ecsi,pval,part_p[csi][0],part_p[csi][1],part_p[csi][2]);
                          // getc(stdin);
                        }

                    // subtract detected particle momenta from beam momentum to
                    // get residual momentum
                    for (int ind = 0; ind < 3; ind++)
                      res_p[ind] = beam_p[ind];
                    for (csi = 1; csi < NCSI; csi++)
                      for (int ind = 0; ind < 3; ind++)
                        res_p[ind] = res_p[ind] - part_p[csi][ind];
                    for (int ind = 0; ind < 3; ind++)
                      res_p[ind] = res_p[ind] * fudgeFactor;

                    // calculate speed of residual
                    pval = sqrt(res_p[0] * res_p[0] + res_p[1] * res_p[1] +
                                res_p[2] * res_p[2]);
                    beta =
                        sqrt(1.0 -
                             ((cal_par->csiarray.mr * cal_par->csiarray.mr) /
                              ((pval * pval) +
                               (cal_par->csiarray.mr *
                                cal_par->csiarray
                                    .mr)))); // relativistic calculation of beta
                    // printf("beta: %f\n",beta);

                    // construct unit vectors for gamma and residual (source)
                    // nucleus
                    for (int ind = 0; ind < 3; ind++)
                      gamma_dir[ind] =
                          cal_par->tg.tpos_xyz[pos2][colAddBack][ind];
                    vecMag = sqrt(gamma_dir[0] * gamma_dir[0] +
                                  gamma_dir[1] * gamma_dir[1] +
                                  gamma_dir[2] * gamma_dir[2]);
                    for (int ind = 0; ind < 3; ind++)
                      gamma_dir[ind] = gamma_dir[ind] /
                                       vecMag; // make unit vector for gamma ray
                    vecMag = sqrt(res_p[0] * res_p[0] + res_p[1] * res_p[1] +
                                  res_p[2] * res_p[2]);
                    for (int ind = 0; ind < 3; ind++)
                      res_dir[ind] =
                          res_p[ind] /
                          vecMag; // make unit vector for residual nucleus

                    // calculate ds
                    ds = sqrt(1 - (beta * beta)) /
                         (1 - (beta * (res_dir[0] * gamma_dir[0] +
                                       res_dir[1] * gamma_dir[1] +
                                       res_dir[2] * gamma_dir[2])));
                    // ds=1+(beta* (res_dir[0]*gamma_dir[0] +
                    // res_dir[1]*gamma_dir[1] + res_dir[2]*gamma_dir[2]) );

                    /*printf("ds: %f\n",ds);
                    printf("Tigress pos: %i\n",pos2);
                    printf("beta: %f, gamma_dir: [%f %f
                    %f]\n",beta,gamma_dir[0],gamma_dir[1],gamma_dir[2]);
                    printf("res_p: [%f %f %f]
                    MeV/c\n",res_p[0],res_p[1],res_p[2]);
                    printf("res_pdir: [%f %f
                    %f]\n",res_dir[0],res_dir[1],res_dir[2]);
                    printf("beam_p: [%f %f %f]
                    MeV/c\n",beam_p[0],beam_p[1],beam_p[2]);
                    for(csi=1;csi<NCSI;csi++)
                            if((part_p[csi][0]+part_p[csi][1]+part_p[csi][2])!=0)
                              printf("In CsI # %i, part_p = [%f %f %f]
                    MeV/c\n",csi,part_p[csi][0],part_p[csi][1],part_p[csi][2]);
                    getc(stdin);*/

                    if (ds != ds) {
                      printf("ERROR: Doppler shift is undefined!\nHave you "
                             "properly specified all detector postions in the "
                             "parameter files?\n");
                      printf("\nINFO DUMP\n---------\n");
                      printf("2nd gamma\n");
                      printf("ds: %f\n", ds);
                      printf("Tigress pos: %i\n", pos2);
                      printf("beta: %f, gamma_dir: [%f %f %f]\n", beta,
                             gamma_dir[0], gamma_dir[1], gamma_dir[2]);
                      printf("res_p: [%f %f %f] MeV/c\n", res_p[0], res_p[1],
                             res_p[2]);
                      printf("res_pdir: [%f %f %f]\n", res_dir[0], res_dir[1],
                             res_dir[2]);
                      printf("beam_p: [%f %f %f] MeV/c\n", beam_p[0], beam_p[1],
                             beam_p[2]);
                      for (csi = 1; csi < NCSI; csi++)
                        if ((part_p[csi][0] + part_p[csi][1] +
                             part_p[csi][2]) != 0)
                          printf("In CsI # %i, part_p = [%f %f %f] MeV/c\n",
                                 csi, part_p[csi][0], part_p[csi][1],
                                 part_p[csi][2]);
                      printf("\n");
                      exit(-1);
                    }

                    // printf("spectrum: %i, ch:
                    // %i\n",0+suppFlag,(int)(eAddBack/ds));
                    eAddBack = eAddBack / ds;
										eAddBack = eAddBack / cntr;

                    e2 = (int)eAddBack;

                    // if the events did not have a suppression flag (good
                    // events following suppression algorithm)
                    if ((suppFlag1 + suppFlag2) == 0)
                      // if the evaluated energy is good at both positions
                      if (e1 > 0)
                        if (e1 < S4K)
                          if (e2 > 0)
                            if (e2 < S4K) {
                              mat[e1][e2]++;
                              mat[e2][e1]++;

                              // symmetrized
                              h->Fill(e1, e2);
                              h->Fill(e2, e1);
                            }
                  }
            }
          }
    }

  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[]) {
  FILE *output;
  input_names_type *name;
  FILE *cluster;
  FILE *gateNameFile;
  TFile* gateFile;
  char aGateName[132],pGateName[132],det[132];
  char n[132];
  char title[132];

  if ((argc != 6) && (argc != 7)) {
    printf("TigressCsIArray_EECalABSuppDSReconstructedSumPID master_file_name "
           "supLow supHigh energy_contraction useCharge fudge_factor\n\n");
		printf("energy_contraction is in keV/ch.\n");
    printf("fudge_factor is a multiplicative factor which will be applied to "
           "the computed value of the residual nucleus momentum, in order to "
           "account for slowing of the beam/compound in the reaction target.  "
           "If left empty, a value of 1.0 will be used.\n");
    printf("\nuseCharge should be set to 1 if charge is to be used instead of fitted amplitude, otherwise set to 0.\n");
    printf("This code uses the CsIArray PID to determine what particle each hit is.\n");
    exit(-1);
  }

  printf("Program sorts calibrated time histogram for TIGRESS cores \n");
  name = (input_names_type *)malloc(sizeof(input_names_type));
  memset(name, 0, sizeof(input_names_type));
  cal_par = (calibration_parameters *)malloc(sizeof(calibration_parameters));
  memset(cal_par, 0, sizeof(calibration_parameters));
  memset(hist, 0, sizeof(hist));
  memset(mat, 0, sizeof(mat));
  read_master(argv[1], name);
  supLow = atof(argv[2]);
  supHigh = atof(argv[3]);
  cntr = atof(argv[4]);
  useCharge = atoi(argv[5]);
  printf("Using an energy contraction factor of %f\n", cntr);
  if (argc == 7)
    fudgeFactor = atof(argv[6]);
  else
    fudgeFactor = 1.0;

  h = new TH2D("Tigress EECalABSupp Sum", "Tigress EECalABSuppSum", S1K, 0,
               S4K - 1, S1K, 0, S4K - 1);
  h->Reset();

  if (name->flag.cluster_file == 1) {
    printf("\nSorting data from cluster file:\n %s\n",
           name->fname.cluster_file);
    if ((cluster = fopen(name->fname.cluster_file, "r")) == NULL) {
      printf("\nI can't open input file %s\n", name->fname.cluster_file);
      exit(-2);
    }
  } else {
    printf("\nCluster file not defined\n");
    exit(-1);
  }

  if (name->flag.TIGRESS_cal_par == 1) {
    printf("\nTIGRESS calibration read from the file:\n %s\n",
           name->fname.TIGRESS_cal_par);
    initialize_TIGRESS_calibration(&cal_par->tg, name->fname.TIGRESS_cal_par);

  } else {
    printf("\nTIGRESS calibration parameters not defined\n");
    exit(EXIT_FAILURE);
  }

  if (name->flag.CSIARRAY_cal_par == 1) {
    printf("CsIArray calpar read from: %s\n", name->fname.CSIARRAY_cal_par);
    initialize_CSIARRAY_calibration(&cal_par->csiarray,
                                    name->fname.CSIARRAY_cal_par);
  } else {
    printf("ERROR! CsIArray calibration parameters not defined!\n");
    exit(EXIT_FAILURE);
  }

  if(name->flag.root_gate_file==1)
    {
      printf("Using root gate file: %s\n",name->fname.root_gate_file);
      gateFile=new TFile(name->fname.root_gate_file,"READ");
    }
  else
    {
      printf("ERROR!!! Root gate file not defined!\n");
      exit(-1);
    }
 
  if(name->flag.gate_name_file==1)
    {
      printf("Using gate name file: %s\n",name->fname.gate_name_file);

      if((gateNameFile=fopen(name->fname.gate_name_file,"r"))==NULL)
  	{
  	  printf("The gate file %s can not be opened\n",name->fname.gate_name_file);
  	  exit(1);
  	}

      while(fscanf(gateNameFile,"%s %s %s",det,aGateName,pGateName)!=EOF)
    	{
    	  pos=atoi(det);
    	  if(strcmp(aGateName,"null"))
    	    {
    	      aGate[pos] = (TCutG *) gROOT->FindObject(aGateName);
    	      aGateFlag[pos]=1;
    	    }
    	  if(strcmp(pGateName,"null"))
    	    {
    	      pGate[pos] = (TCutG *) gROOT->FindObject(pGateName);
    	      pGateFlag[pos]=1;
    	    }
    	}
      fclose(gateNameFile);
    }

  for(pos=1;pos<NCSI;pos++)
    if((pGateFlag[pos]+aGateFlag[pos])!=0)
      {
      	printf("CsI %2d ",pos);
      	if(aGateFlag[pos]==1)
      	  printf("  alpha gate: %10s ",aGate[pos]->GetName());
      	else
      	  printf("%25s"," ");
      	if(pGateFlag[pos]==1)
      	  printf(" proton gate: %10s ",pGate[pos]->GetName());
      	else
      	  printf("%25s"," ");
      	printf("\n");
      }

  // print info
  printf("\n");
  printf("  Beam energy: %f MeV\n", cal_par->csiarray.Ebeam);
  printf("  Projectile mass: %f MeV/c^2\n", cal_par->csiarray.mproj);
  printf("  Particle mass: %f MeV/c^2\n", cal_par->csiarray.mp);
  printf("  Residual mass: %f MeV/c^2\n", cal_par->csiarray.mr);
  if (fudgeFactor > 1.0)
    printf("  WARNING: using fudge factor greater than 1.  This implies that "
           "the beam/compound sppeds up in the target, which should not be "
           "possible.\n");
  else if (fudgeFactor < 1.0)
    printf("  Using fudge factor of %f\n", fudgeFactor);
  printf("\n");

  name->flag.inp_data = 1;
  while (fscanf(cluster, "%s", n) != EOF) {
    strcpy(name->fname.inp_data, n);
    sort(name);
  }

  fclose(cluster);

  if ((output = fopen("matrix.mat", "w")) == NULL) {
    printf("\nI can't open file matrix.mat\n");
    exit(EXIT_FAILURE);
  }

  fwrite(mat, S4K * S4K * sizeof(short int), 1, output);
  fclose(output);

  for (int i = 0; i < S4K; i++)
    for (int j = 0; j < S4K; j++)
      hist[i] += mat[i][j];

  if ((output = fopen("projection.spn", "w")) == NULL) {
    printf("\nI can't open file projection.spn\n");
    exit(EXIT_FAILURE);
  }

  fwrite(hist, S4K * sizeof(int), 1, output);
  fclose(output);

  sprintf(title, "TigressCsIArray_EECalABSuppDSReconstructedSumPID.root");
  TFile f(title, "recreate");
  h->GetXaxis()->SetTitle("E1 [keV]");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("E2 [keV]");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  gStyle->SetPalette(1);
  h->Write();

	printf("Matrix written to file: matrix.mat\n");
	printf("Projection written to file: projection.spn\n");
	printf("Output ROOT file: TigressCsIArray_EECalABSuppDSReconstructedSumPID.root\n");
}
