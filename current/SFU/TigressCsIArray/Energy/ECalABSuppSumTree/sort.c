#include "sort.h"


//checks whether a value falls within the range of two bounds (lower bound inclusive)
//the order of the bounds doesn't matter
bool valueInRange(double value, double bound1, double bound2)
{

  if(bound1>bound2)
    {
      if((value>=bound2)&&(value<bound1))
        return true;
      else
        return false;
    }
  else if(bound2>bound1)
    {
      if((value>=bound1)&&(value<bound2))
        return true;
      else
        return false;
    }
  else if(bound1==bound2)
    {
      if(value==bound1)
        return true;
      else
        return false;
    }
    
  return false;

}
/*=========================================================================*/
int analyze_data(raw_event *data)
{
  cal_event* cev;
  double eAddBack=0; 
  int take=0;
  double ecsi;
  int csi;
  unsigned long long int one=1;
  
  TData.CsIfold=0;
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  memset(beam_p,0,sizeof(beam_p));
  memset(part_p,0,sizeof(part_p));
  memset(gamma_dir,0,sizeof(gamma_dir));

  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  
  //get momentum and beta values from calibration (values specified in parameter file)
  beam_p[2]=cal_par->csiarray.pp;
  beta=cal_par->csiarray.pbeta;

  
  //check the Ge fold
  if(cev->tg.h.FH>0)
	  //look through each Tigress position
	  for(pos=1;pos<NPOSTIGR;pos++)
      {
        //reset suppression flag for each position
        TData.suppFlag=0;
        //check if the position is in the hit pattern
        if((cev->tg.h.HHP&(1<<(pos-1)))!=0) 
          //check the energy fold
          if(cev->tg.det[pos].hge.FH>0)
            //check if the position is in the addback hit pattern
            if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
              {
                /* set take=0 for each position for suppression 
                   of all possible clover hits in add-back mode*/
                take=0;
                //Run through four cores for each position for suppression
                for(col=0;col<NCOL;col++)
                  {
	                  //Check if this color is indicated in the hit pattern
	                  if((cev->tg.det[pos].hge.HHP&(1<<col))!=0)
	                    //Check that this combination has a fold greater than zero
	                    if(cev->tg.det[pos].ge[col].h.FH>0)
	                      //Check if this combination is indicated in the hit pattern
	                      if((cev->tg.det[pos].ge[col].h.HHP&1)!=0)
	                        //suppress if the position is in the map and has not yet been suppressed
	                        if(cev->tg.det[pos].ge[col].suppress>=supLow && cev->tg.det[pos].ge[col].suppress<=supHigh && take==0)
		                        { 
	                            /* once suppression flag is set
	                               do not reset it, could remove the take bit
	                               and keep resetting suppFlag, but this
	                               is nicer */   
		                          TData.suppFlag=1;
		                          take=1;
		                        }
                  }

                eAddBack = cev->tg.det[pos].addback.E/cal_par->tg.contr_e;  
                colAddBack = cev->tg.det[pos].addbackC;
                
                if(eAddBack>=0 && eAddBack<S32K)
                  {
                    //construct vectors
                    for(int ind=0;ind<3;ind++)
                      gamma_dir[ind]=cal_par->tg.tpos_xyz[pos][colAddBack][ind];
                    vecMag=sqrt(gamma_dir[0]*gamma_dir[0] + gamma_dir[1]*gamma_dir[1] + gamma_dir[2]*gamma_dir[2]);
                    for(int ind=0;ind<3;ind++)
                      gamma_dir[ind]=gamma_dir[ind]/vecMag;//make unit vector
                    
                    if(cev->csiarray.h.FH>0)
                      for(csi=1;csi<NCSI;csi++)
                        if((cev->csiarray.h.HHP&(one<<csi))!=0)
                          {
                            for(int ind=0;ind<3;ind++)
                              part_p[csi][ind]=cal_par->csiarray.cpos_xyz[csi][ind];
                            vecMag=sqrt(part_p[csi][0]*part_p[csi][0] + part_p[csi][1]*part_p[csi][1] + part_p[csi][2]*part_p[csi][2]);
                            ecsi=cev->csiarray.csi[csi].E/1000.; /* CsI energy in MeV */
                            for(int ind=0;ind<3;ind++)
                              part_p[csi][ind]=part_p[csi][ind]*(sqrt(2*ecsi*cal_par->csiarray.mp))/vecMag;//make vector proper length for momentum
                          }
              
                    //subtract detected particle momenta from beam momentum to get residual momentum
                    for(int ind=0;ind<3;ind++)
                      res_p[ind]=beam_p[ind];
                    for(csi=1;csi<NCSI;csi++)
                      for(int ind=0;ind<3;ind++)
                        res_p[ind]=res_p[ind]-part_p[csi][ind];
                    //make momentum vector into direction vector  
                    for(int ind=0;ind<3;ind++)
                      res_dir[ind]=res_p[ind]/sqrt(res_p[0]*res_p[0] + res_p[1]*res_p[1] + res_p[2]*res_p[2]);
                    //calculate ds
                    TData.ds=sqrt(1-beta*beta) / (1-(beta*(res_dir[0]*gamma_dir[0] + res_dir[1]*gamma_dir[1] + res_dir[2]*gamma_dir[2])));
                    
                    /*if(ds>1.04)
                      {
                        printf("ds: %f\n",ds);
                        printf("Tigress pos: %i\n",pos);
                        printf("beta: %f, gamma_dir: [%f %f %f]\n",beta,gamma_dir[0],gamma_dir[1],gamma_dir[2]);
                        printf("res_p: [%f %f %f]\n",res_p[0],res_p[1],res_p[2]);
                        printf("res_pdir: [%f %f %f]\n",res_dir[0],res_dir[1],res_dir[2]);
                        printf("beam_p: [%f %f %f]\n",beam_p[0],beam_p[1],beam_p[2]);
                        for(csi=1;csi<NCSI;csi++)
                          if((part_p[csi][0]+part_p[csi][1]+part_p[csi][2])!=0)
                            printf("In CsI # %i, part_p = [%f %f %f]\n",csi,part_p[csi][0],part_p[csi][1],part_p[csi][2]);
                        getc(stdin);
                      }*/
                    
                    tree->Fill();
                  }
                
              }
      }
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*=========================================================================*/
int main(int argc, char *argv[])
{
  
  FILE *cluster;
  input_names_type* name;
  char DataFile[132];
  
  if(argc!=5)
    {
      printf("TigressCsI_ECalABSuppSumTree master_file_name supLow supHigh gate_file\n");
      printf("Program sorts a ROOT tree containing ECalABSuppSum data.\n");
      exit(-1);
    }
  
  tree= new TTree("tree","tree");
  //tree->Branch("CsIfold",&TData.CsIfold,"CsIfold/I");
  //tree->Branch("CsIId",TData.CsIId,"CsIId[CsIfold]/I");
  //tree->Branch("CsIE",TData.CsIE,"CsIE[CsIfold]/D");
  tree->Branch("ds",&TData.ds,"ds/D");
  tree->Branch("suppFlag",&TData.suppFlag,"suppFlag/D");
  
  printf("Program sorts a ROOT tree containing ECalABSuppSum data.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  memset(&TData,0,sizeof(TData));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);
  
  supLow = atof(argv[2]);
  supHigh = atof(argv[3]);
  
  if(name->flag.cluster_file==1)
    {
      printf("Sorting ECalABSuppDSSum histograms for TIGRESS clovers and cores based upon the cluster file: %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	      {
	        printf("ERROR!!! I can't open input file %s\n",name->fname.cluster_file);
	        exit(-2);
	      }
	  }
  else
    {
      printf("ERROR!!! Cluster file not defined\n");
      exit(-1);
    }
  
  //get gates  
  if((gateFile=fopen(argv[4],"r"))==NULL)
    {
      printf("ERROR!!! Cannot open the custom gate file %s\n",argv[4]);
      exit(-1);
    }
  num_gates=0;
  while(!(feof(gateFile)))//go until the end of file is reached
    {
      if(num_gates<(NGATES+1))
        if(fgets(str,256,gateFile)!=NULL)
          {
            gates[num_gates]=atof(str);
            num_gates++;
          }
    }
  num_gates--;
  fclose(gateFile);
  for(int ind=0;ind<num_gates;ind++)
    printf("Gate %i from %f to %f\n",ind,gates[ind],gates[ind+1]);
  //getc(stdin);
  
  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calpar read from: %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR!!! TIGRESS calibration parameters not defined\n");
      exit(EXIT_FAILURE);
    }

  if(name->flag.CSIARRAY_cal_par==1)
    {
      printf("CsIArray calpar read from: %s\n",name->fname.CSIARRAY_cal_par);
      initialize_CSIARRAY_calibration(&cal_par->csiarray,name->fname.CSIARRAY_cal_par);
    }
  else
    {
      printf("ERROR! CsIArray calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting data from file %s\n", name);
      sort(name);
    }
  
  TDirectory *dir=new TDirectory();
  dir=gDirectory;
  TFile f("Tree_ECalABSuppSum.root","recreate");
  f.cd();
  //tree->AutoSave();
  //tree->SetDirectory(dir);
  tree->Write();
  f.Close();
  dir->cd();
  
  /*theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("DS","DS",10,10, 500, 300);
  gPad->SetLogy(1);
  h->Draw();
  theApp->Run(kTRUE);*/
  
}
