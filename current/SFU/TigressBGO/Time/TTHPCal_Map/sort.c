#include "sort.h"

int analyze_data(raw_event *data)
{
  //unsigned long long int one=1;
  int posTIG,colTIG,posBGO,colBGO,supBGO;
  int tig,bgo;
  double tigE;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);

  //check TIGRESS energy and time hit fold
  if(cev->tg.h.FH>0)
    //check BGO time hit fold
    if(cev->tg.h.BFT>0)
      //TIGRESS part - loop through positions, cores; check associated energy+time hit patterns
      for(posTIG=1;posTIG<NPOSTIGR;posTIG++)
	if((cev->tg.h.HHP&(1<<(posTIG-1)))!=0)
	  if(cev->tg.det[posTIG].hge.FH>0)
	    for(colTIG=0;colTIG<NCOL;colTIG++)
	      if((cev->tg.det[posTIG].hge.HHP&(1<<colTIG))!=0)
		if(cev->tg.det[posTIG].ge[colTIG].h.FH>0)
		  if((cev->tg.det[posTIG].ge[colTIG].h.HHP&1)!=0)
		    {
		      tigE=cev->tg.det[posTIG].ge[colTIG].seg[0].E/cal_par->tg.contr_e;
		      if(tigE>=tigE_low && tigE<=tigE_high) //energy gate for suppression
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
					  
					  // tigress on x-axis, bgo on y-axis
					  h->Fill(tig,bgo);
					  tthp[tig][bgo]++;
					}
		      
		    }
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  FILE *cluster;
  char title[132],DataFile[132];
  int i,j;

  //hit pattern statistics
  double N,sum,sum2,sq;
  double mean,stdev;

  //stuff for making the suppression map  
  FILE *supMap;
  int posTIG,colTIG,posBGO,colBGO,supBGO;
  int N_sigma;

  if(argc!=5)
    {
      printf("TigressBGO_TTHPCal_Map master_file_name N_sigma tigE_low tigE_high\n");
      exit(-1);
    }

  //suppress events with number of hits H > mean+N_sigma*stdev, N_sigma is an integer
  N_sigma = atoi(argv[2]);

  //tigE_low and tigE_high are low and high energy limits for TIGRESS gamma-rays for making the suppression map
  tigE_low = atof(argv[3]);
  tigE_high = atof(argv[4]);

  //hit pattern histogram
  h = new TH2D("TigressBGO TTHPCal Map","TigressBGO TTHPCal Map",NPOSTIGR*NCOL,0,NPOSTIGR*NCOL-1,NPOSTIGR*NCOL*NSUP,0,NPOSTIGR*NCOL*NSUP-1);
  h->Reset();

  //suppression map histogram
  h2 = new TH2D("TigressBGO Cal Suppession Map","TigressBGO Cal Suppression Map",NPOSTIGR*NCOL,0,NPOSTIGR*NCOL-1,NPOSTIGR*NCOL*NSUP,0,NPOSTIGR*NCOL*NSUP-1);
  h2->Reset();

  printf("Program sorts 2D hit pattern for Tigress and BGO using good time hit patterns and makes a suppression map\n");

  //allocate memory - make sure to include calibrations!
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

  read_master(argv[1],name);

  if(name->flag.cluster_file==1)
    {
      printf("Sorting Tigress BGO hit pattern histograms from cluster file: %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("ERROR! I can't open input file %s\n",name->fname.cluster_file);
	  exit(-2);
	}
    }
  else
    {
      printf("ERROR! Cluster file not defined\n");
      exit(-1);
    }

  if(name->flag.TIGRESS_cal_par==1)
    {
      printf("TIGRESS calpar read from: %s\n",name->fname.TIGRESS_cal_par);
      initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
    }
  else
    {
      printf("ERROR!!! TIGRESS calibration parameters not defined\n");
      exit(-1);
    }
  
  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
      
      printf("Sorting data from %s\n", name);
      sort(name); 
    }

  //write histogram to file so you can look at it
  sprintf(title,"TigressBGO_TTHPCal_Map.root");
  TFile f(title, "recreate");
  h->GetXaxis()->SetTitle("TIGRESS detector number");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("BGO detector number");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  gStyle->SetPalette(1);
  h->Write();

  //open the file for writing the suppression map
  supMap = fopen("TIGRESS_suppression_map.par","w");
  fprintf(supMap,"This is a suppression map file for TIGRESS cores\n");
  fprintf(supMap,"posTIG  colTIG  posBGO  colBGO  supBGO\n");

  //two pass calculation for std dev - this should work properly (it does)
  //plus one pass for making the map based on the result
  //there are good one pass algorithms, but the time is in sorting, not this stuff
for(i=0;i<NPOSTIGR*NCOL-1;i++)
    {
      N=0.;
      sum=0.;
      sum2=0.;
      stdev=0.;
      mean=0.;
      sq=0.;

      for(j=0;j<NPOSTIGR*NCOL*NSUP-1;j++)
	{
	  if(tthp[i][j]!=0)
	    {
	      N+=1.;
	      sum+=(double)tthp[i][j];
	    }
	  //printf("tthp tig: %d bgo: %d count: %d\n",i,j,tthp[i][j]);
	  //getc(stdin);
	}

      if(N==0)
	{
	  mean=0.;
	  stdev=0.;
	}

      //2nd pass to get stdev is here
      else
	{
	  mean=sum/N;	  
	  for(j=0;j<NPOSTIGR*NCOL*NSUP-1;j++)
	    {
	      if(tthp[i][j]!=0)
		{
		  sq+=((double)tthp[i][j]-mean)*((double)tthp[i][j]-mean);
		  sum2+=(double)tthp[i][j]-mean;
		}	      
	    }
	  stdev=sqrt((sq-(sum2*sum2)/N)/(N-1));
	}
      
      for(j=0;j<NPOSTIGR*NCOL*NSUP-1;j++)
	if(tthp[i][j] > 0) //does this do anything?
	  if(tthp[i][j] >= mean+N_sigma*stdev)
	    {
	      //reconstruct TIGRESS and BGO detectors from the hit map
	      //% is modular arithmatic, i.e. r = a%b is the remainder
	      //when a is divided by b.
	      colTIG = i%NCOL;
	      posTIG = (i-colTIG)/NCOL + 1;
	      
	      supBGO = j%NSUP;
	      colBGO = ((j-supBGO)/NSUP)%NCOL;
	      posBGO = (((j-supBGO)/NSUP) - colBGO)/NCOL + 1; 
	      
	      fprintf(supMap,"%d       %d       %d       %d       %d\n",posTIG,colTIG,posBGO,colBGO,supBGO);
	      
	      //fill histogram of the suppression map
	      h2->Fill(i,j);
	    }
    }
 
//write the map histogram to the file
  h2->GetXaxis()->SetTitle("TIGRESS detector number");
  h2->GetXaxis()->CenterTitle(true);
  h2->GetYaxis()->SetTitle("BGO detector number");
  h2->GetYaxis()->CenterTitle(true);
  h2->GetYaxis()->SetTitleOffset(1.5);
  h2->SetOption("COLZ");
  gStyle->SetPalette(1);
  h2->Write();

  //close the map	
  fclose(supMap);
}

