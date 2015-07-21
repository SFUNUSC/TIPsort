#include "sort.h"

int analyze_data(raw_event *data)
{ 
  cal_event* cev;
  //unsigned long long int one=1;
  int posTIG,posBGO,colTIG,colBGO,supBGO;
  int tig,bgo;
  //double ren,ran;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));

  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);

  /* calibrated hit pattern requires good calibrated time and energy from TIGRESS and good calibrated time from BGO */

  //printf("FH %d BFT %d\n",cev->tg.h.FH,cev->tg.h.BFT);

  //check TIGRESS hit fold
  if(cev->tg.h.FH>0)
    //check BGO hit fold
    if(cev->tg.h.BFT>0)
      //TIGRESS part - loop through positions, cores; check associated hit patterns
      for(posTIG=1;posTIG<NPOSTIGR;posTIG++)
	if((cev->tg.h.HHP&(1<<(posTIG-1)))!=0)
	  if(cev->tg.det[posTIG].hge.FH>0)
	    for(colTIG=0;colTIG<NCOL;colTIG++)
	      if((cev->tg.det[posTIG].hge.HHP&(1<<colTIG))!=0)
		if(cev->tg.det[posTIG].ge[colTIG].h.FH>0)
		  if((cev->tg.det[posTIG].ge[colTIG].h.HHP&1)!=0)
		    //BGO part - loop through positions, cores, suppressors; check associated hit patterns
		    for(posBGO=1;posBGO<NPOSTIGR;posBGO++)
		      if((cev->tg.h.BTHP&(1<<(posBGO-1)))!=0)
			if(cev->tg.det[posBGO].hbgo.BFT>0)
			  for(colBGO=0;colBGO<NCOL;colBGO++)
			    if((cev->tg.det[posBGO].hbgo.BTHP&(1<<colBGO))!=0)
			      if(cev->tg.det[posBGO].bgo[colBGO].h.BFT>0)
				for(supBGO=0;supBGO<NSUP;supBGO++)
				  if((cev->tg.det[posBGO].bgo[colBGO].h.BTHP&(1<<supBGO))!=0)
				    {
				      //tigress from 0-63 ... pos1 from 0-3, pos2 from 4-7, etc.
				      tig =(posTIG-1)*NCOL+colTIG;
				      
				      //bgo from 0-319 ... pos1 col0 from 0-4, pos1 col1 from 5-9, etc.
				      bgo = ((posBGO-1)*NCOL+colBGO)*NSUP+supBGO;

				      //printf("posTIG %d colTIG %d tig %d\n",posTIG,colTIG,tig);
				      //printf("posBGO %d colBGO %d supBGO %d bgo %d\n",posBGO,colBGO,supBGO,bgo);
				      //getc(stdin);
				   			      
				      // tigress on x-axis, bgo on y-axis
				      h->Fill(tig,bgo);
				      tthp[tig][bgo]++;
				    }

  //printf("/***** END OF EVENT *****/\n");
  //getc(stdin);
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  char title[132];
  //int i,j;
  //double N,sum,sum2,sq;
  //double mean,stdev;

  if(argc!=2)
    {
      printf("TigressBGO_TTHPCal master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("TigressBGO TTHPCal","TigressBGO TTHPCal",NPOSTIGR*NCOL,0,NPOSTIGR*NCOL-1,NPOSTIGR*NCOL*NSUP,0,NPOSTIGR*NCOL*NSUP-1);
  h->Reset();


  printf("Program sorts 2D hit pattern for Tigress and BGO using good calibrated time hit patterns \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

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

  sort(name);

  //write histogram to file so you can look at it
  sprintf(title,"TigressBGO_TTHPCal.root");
  TFile f(title, "recreate");
  h->GetXaxis()->SetTitle("TIGRESS detector number");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("BGO detector number");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  gStyle->SetPalette(1);
  h->Write();

  //read histogram and make the map
  //stdev calculated using corrected two-pass algorithm
  /* for(i=0;i<NPOSTIGR*NCOL-1;i++) */
  /*   { */
  /*     N=0.; */
  /*     sum=0.; */
  /*     sum2=0.; */
  /*     stdev=0.; */
  /*     mean=0.; */
  /*     sq=0.; */

  /*     for(j=0;j<NPOSTIGR*NCOL*NSUP-1;j++) */
  /* 	{ */
  /* 	  if(tthp[i][j] != 0) */
  /* 	    { */
  /* 	      N += 1.; */
  /* 	      sum += (double)tthp[i][j]; */
  /* 	    } */
  /* 	  //printf("tthp tig: %d bgo: %d count: %d\n",i,j,tthp[i][j]); */
  /* 	  //getc(stdin); */
  /* 	} */

  /*     if(N==0) */
  /* 	{ */
  /* 	  mean=0.; */
  /* 	  stdev=0.; */
  /* 	} */

  /*     //2nd pass to get stdev is here */
  /*     else */
  /* 	{ */
  /* 	  mean = sum/N;	   */
  /* 	  for(j=0;j<NPOSTIGR*NCOL*NSUP-1;j++) */
  /* 	    { */
  /* 	      if(tthp[i][j] != 0) */
  /* 		{ */
  /* 		  sq += ((double)tthp[i][j]-mean)*((double)tthp[i][j]-mean); */
  /* 		  sum2 += (double)tthp[i][j]-mean; */
  /* 		}	       */
  /* 	    } */
  /* 	  stdev = sqrt((sq-(sum2*sum2)/N)/(N-1)); */
  /* 	} */
  /*     printf("tthp tig: %d mean: %.2f, stdev: %.2f\n",i,mean,stdev); */
  /*     //getc(stdin); */
  /*   } */
  
  return 0;
}

