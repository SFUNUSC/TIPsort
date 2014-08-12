#include "sort.h"

int analyze_data(raw_event *data)
{
  //unsigned long long int one=1;
  int posTIG,colTIG,posBGO,colBGO,supBGO;
  double tig,bgo,e;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);

//Check Ge fold
  //if(data->tg.h.Gefold>0)
  if(cev->tg.h.FH>0)
    //Run through all positions
    for(posTIG=1;posTIG<NPOSTIGR;posTIG++)
      //Check if the position is indicated in the hit pattern
      //if((data->tg.h.GeHP&(1<<(posTIG-1)))!=0)
      if((cev->tg.h.EHP&(1<<(posTIG-1)))!=0)
	if((cev->tg.h.THP&(1<<(posTIG-1)))!=0)
	  //Check if the position has a fold great than zero
	  //if(data->tg.det[posTIG].h.Gefold>0)
	  if(cev->tg.det[posTIG].hge.FE>0)
	    if(cev->tg.det[posTIG].hge.FT>0)
	      //Run through four cores for each position
	      for(colTIG=0;colTIG<NCOL;colTIG++)
		//Check if this position is indicated in the hit pattern
		//if((data->tg.det[posTIG].h.GeHP&(1<<colTIG))!=0)
		if((cev->tg.det[posTIG].hge.EHP&(1<<colTIG))!=0)
		  if((cev->tg.det[posTIG].hge.THP&(1<<colTIG))!=0)
		    //Check that this combination has a time fold great than zero
		    //if(data->tg.det[posTIG].ge[colTIG].h.Tfold>0)
		    if(cev->tg.det[posTIG].ge[colTIG].h.FE>0)
		      if(cev->tg.det[posTIG].ge[colTIG].h.FT>0)
			//Check if this combination is indicated in the time hit pattern
			//if((data->tg.det[posTIG].ge[colTIG].h.THP&1)!=0)
			if((cev->tg.det[posTIG].ge[colTIG].h.EHP&1)!=0)
			  if((cev->tg.det[posTIG].ge[colTIG].h.THP&1)!=0)
			    {

			      e=cev->tg.det[posTIG].ge[colTIG].seg[0].E/cal_par->tg.contr_e;
			      if(e>=95. && e<=105.)

			    //Check BGO fold
			    if(data->tg.h.BGOfold>0)
			      //Run through all positions
			      for(posBGO=1;posBGO<NPOSTIGR;posBGO++)
				{
				  //not sure what take is doing
				  //take=0;
				  //check if this position is in the hit pattern with fold > 0
				  if(((data->tg.h.BGOHP&(1<<(posBGO-1)))!=0) && (data->tg.det[posBGO].h.BGOfold>0))
				    //run through all the colors
				    for(colBGO=0;colBGO<NCOL;colBGO++)
				      //check if this color is in the hit pattern with a fold > 0
				      if(((data->tg.det[posBGO].h.BGOHP&(1<<colBGO))!=0) && (data->tg.det[posBGO].bgo[colBGO].h.Tfold>0))
					//run through all suppressors
					for(supBGO=0;supBGO<NSUP;supBGO++)
					  //check if this suppressor is in the hit pattern with fold > 0
					  if(((data->tg.det[posBGO].bgo[colBGO].h.THP&(1<<supBGO))!=0))// && (take==0))
					    {
					      //tigress from 0-63 ... pos1 from 0-3, pos2 from 4-7, etc.
					      tig =(posTIG-1)*NCOL+colTIG;
					      
					      //bgo from 0-319 ... pos1 col0 from 0-4, pos1 col1 from 5-9, etc.
					      bgo = ((posBGO-1)*NCOL+colBGO)*NSUP+supBGO; 
					      
					      // tigress on x-axis, bgo on y-axis
					      h->Fill(tig,bgo);
					      tthp[(int)rint(tig)][(int)rint(bgo)]++;
					    }
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
  double N,sum;
  double mean,var,stdev;

  //stuff for making the suppression map  
  FILE *supMap;
  int tigPos,tigCol,bgoPos,bgoCol,bgoSup;

  if(argc!=2)
    {
      printf("TigressBGO_TTHP master_file_name\n");
      exit(-1);
    }

  //hit pattern histogram
  h = new TH2D("TigressBGO TTHP","TigressBGO TTHP",NPOSTIGR*NCOL,0,NPOSTIGR*NCOL-1,NPOSTIGR*NCOL*NSUP,0,NPOSTIGR*NCOL*NSUP-1);
  h->Reset();

  //suppression map histogram
  h2 = new TH2D("TigressBGO Suppession Map","TigressBGO Suppression Map",NPOSTIGR*NCOL,0,NPOSTIGR*NCOL-1,NPOSTIGR*NCOL*NSUP,0,NPOSTIGR*NCOL*NSUP-1);
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
  sprintf(title,"TigressBGO_TTHP_Map.root");
  TFile f(title, "recreate");
  h->GetXaxis()->SetTitle("TIGRESS hit pattern");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("BGO hit pattern");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  gStyle->SetPalette(1);
  h->Write();

  //open the file for writing the suppression map
  supMap = fopen("TIGRESS_suppression_map.par","w");
  fprintf(supMap,"This is a suppression map file for TIGRESS cores\n");
  fprintf(supMap,"tigPos  tigCol  bgoPos  bgoCol  bgoSup\n");

  //two pass calculation for std dev - this should work properly (it does)
  //plus one pass for making the map based on the result
  //there are good one pass algorithms, but the time is in sorting, not this stuff
  for(i=0;i<NPOSTIGR*NCOL-1;i++)
    {
      sum = 0.;
      N = 0.;
      var = 0.;
      
      for(j=0;j<NPOSTIGR*NCOL*NSUP-1;j++)
	if(tthp[i][j] != 0)
	  {
	    N += 1.;
	    sum += tthp[i][j];
	  }
          
      mean = sum/N;
      
      for(j=0;j<NPOSTIGR*NCOL*NSUP-1;j++)
	if(tthp[i][j] != 0)
	  var += (tthp[i][j]-mean)*(tthp[i][j]-mean);
      
      var = var/(N-1);
      stdev = sqrt(var);
      
      for(j=0;j<NPOSTIGR*NCOL*NSUP-1;j++)
	if(tthp[i][j] >= mean+3*stdev)
	  {
	    //reconstruct TIGRESS and BGO detectors from the hit map
	    //% is modular arithmatic, i.e. r = a%b is the remainder
	    //when a is divided by b.
	    tigCol = i%NCOL;
	    tigPos = (i-tigCol)/NCOL + 1;
	    
	    bgoSup = j%NSUP;
	    bgoCol = ((j-bgoSup)/NSUP)%NCOL;
	    bgoPos = (((j-bgoSup)/NSUP) - bgoCol)/NCOL + 1; 
	    
	    fprintf(supMap,"%d       %d       %d       %d       %d\n",tigPos,tigCol,bgoPos,bgoCol,bgoSup);

	    //fill histogram of the suppression map
	    h2->Fill(i,j);
	  }
    }
  
  //write the map histogram to the file
  h2->GetXaxis()->SetTitle("TIGRESS detector");
  h2->GetXaxis()->CenterTitle(true);
  h2->GetYaxis()->SetTitle("BGO detector");
  h2->GetYaxis()->CenterTitle(true);
  h2->GetYaxis()->SetTitleOffset(1.5);
  h2->SetOption("COLZ");
  gStyle->SetPalette(1);
  h2->Write();

  //close the map	
  fclose(supMap);
}

