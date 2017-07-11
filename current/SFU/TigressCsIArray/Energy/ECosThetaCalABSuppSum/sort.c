#include "sort.h"

void addToFile(const char * filename, const char * input)
{
	FILE *file;
	if((file=fopen(filename,"a"))==NULL)
		{
			printf("ERROR: cannot open the file %s\n",filename);
    	exit(EXIT_FAILURE);
   	}
  fprintf(file,"%s",input);
  fclose(file);
}

/*=========================================================================*/
int analyze_data(raw_event *data)
{
  cal_event* cev;
  double eAddBack=0;
  int suppFlag=0;
  int take=0;
  double ecsi;
  int csi;
  unsigned long long int one=1;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  memset(beam_p,0,sizeof(beam_p));
  memset(part_p,0,sizeof(part_p));
  memset(gamma_dir,0,sizeof(gamma_dir));

  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  calibrate_CSIARRAY(data,&cal_par->csiarray,&cev->csiarray);
  
  //get momentum and beta values from calibration (values specified in parameter file)
  //beam_p[2]=cal_par->csiarray.pp;
  //beta=cal_par->csiarray.pbeta;
  //beam_p[2]=sqrt(2.0*cal_par->csiarray.Ebeam*cal_par->csiarray.mproj); //momentum of incoming beam
  beam_p[2]=sqrt( ((cal_par->csiarray.Ebeam + cal_par->csiarray.mproj)*(cal_par->csiarray.Ebeam + cal_par->csiarray.mproj)) - (cal_par->csiarray.mproj*cal_par->csiarray.mproj) ); //momentum of incoming beam, relativistic, from KE = mc^2 + m0c^2, mc^2 = sqrt(p^2c^2 + m0^2c^4)
  //printf("Beam p: %f\n",beam_p[2]);
  //getc(stdin);
  //beta=sqrt(2.0*cal_par->csiarray.Ebeam/(cal_par->csiarray.mproj)); // v/c of incoming beam

  
  //check the Ge fold
  if(cev->tg.h.FH>0)
	  //look through each Tigress position
	  for(pos=1;pos<NPOSTIGR;pos++)
      {
        //reset suppression flag for each position
        suppFlag=0;
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
	                    //Check that this combination has a fold great than zero
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
		                          suppFlag=1;
		                          take=1;
		                        }
                  }

                eAddBack = cev->tg.det[pos].addback.E/cal_par->tg.contr_e;  
                colAddBack = cev->tg.det[pos].addbackC;
                
                if(eAddBack>=ELow && eAddBack<=EHigh)
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
                              {
		                          	part_p[csi][ind]=part_p[csi][ind]*(sqrt( ((ecsi+cal_par->csiarray.mp)*(ecsi+cal_par->csiarray.mp)) - (cal_par->csiarray.mp*cal_par->csiarray.mp) )); //relativistic p
		                          	part_p[csi][ind]/=vecMag;//make vector proper length for momentum
                              }
                          }
              
                    //subtract detected particle momenta from beam momentum to get residual momentum
                    for(int ind=0;ind<3;ind++)
                      res_p[ind]=beam_p[ind];
                    for(csi=1;csi<NCSI;csi++)
                      for(int ind=0;ind<3;ind++)
                        res_p[ind]=res_p[ind]-part_p[csi][ind];
                    //make momentum vector into unit direction vector  
                    for(int ind=0;ind<3;ind++)
                      res_dir[ind]=res_p[ind]/sqrt(res_p[0]*res_p[0] + res_p[1]*res_p[1] + res_p[2]*res_p[2]);
                    //calculate cos theta
                    ctheta=res_dir[0]*gamma_dir[0] + res_dir[1]*gamma_dir[1] + res_dir[2]*gamma_dir[2];
                    
                    if(suppFlag==0)
                    	{
			              		h->Fill(ctheta,eAddBack);
			              		sprintf(str,"%lf %lf\n",ctheta,eAddBack);
			              		addToFile(outname,str);
                    	}
                  }
                
              }
      }
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*=========================================================================*/
int main(int argc, char *argv[])
{

  TCanvas *canvas;
  TApplication *theApp;
  
  FILE *cluster;
  input_names_type* name;
  char DataFile[132];
  char title[132];
  
  if(argc!=6)
    {
      printf("TigressCsI_ECosThetaCalABSuppSum master_file_name suppLow suppHigh ELow EHigh\n");
      printf("Program sorts EThetaCalABSuppSum histograms for calibrated TIGRESS and CsI array.\n");
      printf("ELow and EHigh are energy bounds to search in, modified by the contraction factor.\n");
      exit(-1);
    }
  
  //set up the output file
  sprintf(outname,"ECosTheta.txt");
  printf("Output filename is: %s\n",outname);
  FILE *file;
	if((file=fopen(outname,"w"))==NULL)
		{
			printf("ERROR: cannot open the file %s\n",outname);
    	exit(EXIT_FAILURE);
   	}
  fprintf(file,"FIT lin\n");
  fprintf(file,"PLOT 1d\n");
  fclose(file);
  
  supLow = atof(argv[2]);
  supHigh = atof(argv[3]);
  ELow = atof(argv[4]);
  EHigh = atof(argv[5]);
  
  h = new TH2D("ECosThetaHistogram","ECosThetaHistogram",180,-1,1,EHigh-ELow,ELow,EHigh);
  h->Reset();
  
  printf("Program sorts ECosThetaCalABSuppSum histograms for TIGRESS.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  read_master(argv[1],name);
  
  
  
  if(name->flag.cluster_file==1)
    {
      printf("Sorting ECosThetaCalABSuppSum histograms for TIGRESS clovers and cores based upon the cluster file: %s\n",name->fname.cluster_file);
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
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("EDS","EDS",10,10, 800, 500);
  h->GetXaxis()->SetTitle("Cos(#theta)");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("Calibrated TIGRESS Energy [keV]*contraction");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  h->Draw();
  theApp->Run(kTRUE);
  
  sprintf(title,"TigressCsIArray_EDSCal.root");
  TFile f(title, "recreate");
  h->Write();
  
}
