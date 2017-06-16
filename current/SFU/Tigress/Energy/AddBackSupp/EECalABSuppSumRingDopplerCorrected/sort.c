#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  int pos1,pos2,col1,col2;
  //double t1,t2,t;
  int  e1,e2;

  int suppFlag1=0;
  int suppFlag2=0;

  //do i need this?
  int take1=0;
  int take2=0;

  e1=0.;
  e2=0.;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);
  
  //check addback fold
  //printf("FH = %d FA= %d\n",cev->tg.h.FH,cev->tg.h.FA);
  //getc(stdin);
  
  if(cev->tg.h.FA==3)
    //look through each Tigress position
    for(pos1=1;pos1<NPOSTIGR;pos1++)
      {
  	//reset suppression flag for every position
  	suppFlag1=0;
  	//check if the position is in the hit pattern
  	if((cev->tg.h.HHP&(1<<(pos1-1)))!=0)
	  //check the position fold
	  if(cev->tg.det[pos1].hge.FH>0)
	    //check if the position is in the addback hit pattern
	    if((cev->tg.h.AHP&(1<<(pos1-1)))!=0)
	      {
		//reset take for add-back suppression
		take1=0;
		//Run through four cores for each position
		for(col1=0;col1<NCOL;col1++)
		  {
		    //check if the position and color is in the hit pattern
		    if((cev->tg.det[pos1].hge.HHP&(1<<col1))!=0)
		      //check the fold
		      if(cev->tg.det[pos1].ge[col1].h.FH>0)
			{
  			  //suppress if the position is in the map and has not yet been suppressed
  			  if(cev->tg.det[pos1].ge[col1].suppress>=supLow && cev->tg.det[pos1].ge[col1].suppress<=supHigh && take1==0)
  			    {
  			      /* once suppression flag is set
  				 do not reset it, could remove the take bit
  				 and keep resetting suppFlag, but this
  				 is nicer */
  			      suppFlag1=1;
  			      take1=1;
			    }
			}
		  }
		
		
		//get energy for the first position
		e1=(int)rint(cev->tg.det[pos1].addback.E/cal_par->tg.contr_e);
		colAddBack = cev->tg.det[pos1].addbackC;
		if(cev->tg.det[pos1].ge[colAddBack].ring<NRING)
	     e1=e1*dshift[cev->tg.det[pos1].ge[colAddBack].ring];
		
		for(pos2=pos1+1;pos2<NPOSTIGR;pos2++)
		  {
		    //reset suppression flag for every position
		    suppFlag2=0;
		    //check if the position is in the hit pattern
		    if((cev->tg.h.HHP&(1<<(pos2-1)))!=0)
		      //check the position fold
		      if(cev->tg.det[pos2].hge.FH>0)
			//check if the position is in the addback hit pattern
			if((cev->tg.h.AHP&(1<<(pos2-1)))!=0)
			  {
			    //reset take for add-back suppression
			    take2=0;
			    //Run through four cores for each position
			    for(col2=0;col2<NCOL;col2++)
			      {
				//check if the position and color is in the hit pattern
				if((cev->tg.det[pos2].hge.HHP&(1<<col2))!=0)
				  //check the fold
				  if(cev->tg.det[pos2].ge[col2].h.FH>0)
				    {
				      //suppress if the position is in the map and has not yet been suppressed
				      if(cev->tg.det[pos2].ge[col2].suppress>=supLow && cev->tg.det[pos2].ge[col2].suppress<=supHigh && take2==0)
					{
					  /* once suppression flag is set
					     do not reset it, could remove the take bit
					     and keep resetting suppFlag, but this
					     is nicer */
					  suppFlag2=1;
					  take2=1;
					}
				    }
			      }
			    
			    //get energy for the second position
			    e2=(int)rint(cev->tg.det[pos2].addback.E/cal_par->tg.contr_e);
			    colAddBack = cev->tg.det[pos2].addbackC;
			    if(cev->tg.det[pos2].ge[colAddBack].ring<NRING)
	     			e2=e2*dshift[cev->tg.det[pos2].ge[colAddBack].ring];
			    
			    //if the events did not have a suppression flag (good events following suppression algorithm)
			    if( (suppFlag1+suppFlag2)==0 )
			      //if the evaluated energy is good at both positions
			      if(e1>0)
				if(e1<S4K)
				  if(e2>0)
				    if(e2<S4K)
				      {
								mat[e1][e2]++;
								mat[e2][e1]++;
					
								//symmetrized
								h->Fill(e1,e2);
								h->Fill(e2,e1);
				      }
			  }
		  }
	      }
      }
  
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  FILE *cluster;
  char n[132];
  char title[132];

  if(argc!=5)
    {
      printf("TIGRESS_EECalABSuppSumRingDopplerCorrected master_file_name supLow supHigh shift_file\n");
      exit(-1);
    }
  
  printf("Program sorts calibrated time histogram for TIGRESS cores \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  memset(hist,0,sizeof(hist));
  memset(mat,0,sizeof(mat));
  read_master(argv[1],name);
  supLow=atof(argv[2]);
  supHigh=atof(argv[3]);

  h = new TH2D("Tigress EECalABSupp Sum","Tigress EECalABSuppSum",S1K,0,S4K-1,S1K,0,S4K-1);
  h->Reset();

  if(name->flag.cluster_file==1)
    {
      printf("\nSorting data from cluster file:\n %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("\nI can't open input file %s\n",name->fname.cluster_file);
	  exit(-2);
	}
    }
  else
    {
      printf("\nCluster file not defined\n");
      exit(-1);
    }

	
	
	//get doppler shifts  
  if((shiftFile=fopen(argv[4],"r"))==NULL)
    {
      printf("ERROR!!! Cannot open the ring doppler shift file %s\n",argv[4]);
      printf("Press ENTER to exit.\n");
      getc(stdin);
      exit(-1);
    }
  num_rings=0;
  while(!(feof(shiftFile)))//go until the end of file is reached
    {
      if(num_rings<(NRING+1))
        if(fgets(str,256,shiftFile)!=NULL)
          {
            dshift[num_rings]=atof(str);
            num_rings++;
          }
    }
  num_rings--;
  fclose(shiftFile);
	


  if(name->flag.TIGRESS_cal_par==1)
        {
          printf("\nTIGRESS calibration read from the file:\n %s\n",name->fname.TIGRESS_cal_par);
          initialize_TIGRESS_calibration(&cal_par->tg,name->fname.TIGRESS_cal_par);
	  
        }
      else
        {
          printf("\nTIGRESS calibration parameters not defined\n");
          exit(EXIT_FAILURE);
        }

  name->flag.inp_data=1; 
  while(fscanf(cluster,"%s",n)!=EOF)
    {
      strcpy(name->fname.inp_data,n);
      sort(name);
    }

  fclose(cluster);


 
  if((output=fopen("matrix.mat","w"))==NULL)
    {
      printf("\nI can't open file matrix.mat\n");
      exit(EXIT_FAILURE);
    }
	  
  fwrite(mat,S4K*S4K*sizeof(short int),1,output);
  fclose(output);

  for(int i=0;i<S4K;i++)
    for(int j=0;j<S4K;j++)
      hist[i]+=mat[i][j];
    

  if((output=fopen("projection.spn","w"))==NULL)
    {
      printf("\nI can't open file projection.spn\n");
      exit(EXIT_FAILURE);
    }
	  
  fwrite(hist,S4K*sizeof(int),1,output);
  fclose(output);

  sprintf(title,"Tigress_EECalABSuppSum.root");
  TFile f(title, "recreate");
  h->GetXaxis()->SetTitle("E1 [keV]");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("E2 [keV]");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  gStyle->SetPalette(1);
  h->Write();
  
}

  

