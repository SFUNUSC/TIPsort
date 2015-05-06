#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;

  //for building the initial energy and suppression array
  int pos;

  //for suppression
  int col;
  int suppFlag;
  int take;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_TIGRESS(data,&cal_par->tg,&cev->tg);

  //make sure energy and suppression arrays are initialized to 0 for each fragment
  memset(ene,0,sizeof(ene));
  memset(sup,0,sizeof(sup));
  
  /* INIT ENERGY AND SUPPRESSION ARRAYS*/
  //check addback fold  
  if(cev->tg.h.FA>1)
    //look through TIGRESS positions
    for(pos=1;pos<NPOSTIGR;pos++)
      {
	//if the position is in the addback hit pattern
	if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
	  {
	    //then put the addback energy at the position in the energy array
	    ene[pos]=cev->tg.det[pos].addback.E/cal_par->tg.contr_e;
	    //and a 'true' in the array for suppression
	    sup[pos]=1;
	  }
      }

  printf("ENERGY ARRAY\n");
  for(pos=1;pos<NPOSTIGR;pos++)
    printf("pos %.2d E = %.2f\n",pos,ene[pos]);
  printf("SUPPRESSION ARRAY INIT\n");
  for(pos=1;pos<NPOSTIGR;pos++)
    printf("pos %.2d E = %d\n",pos,sup[pos]);
  getc(stdin);

  /* CHECK SUPPRESSION AND SET SUPPRESSION ARRAY */
  //check the Ge fold
  if(cev->tg.h.FH>0)
    //look through each Tigress position
    for(pos=1;pos<NPOSTIGR;pos++)
      {
  	suppFlag=0;
	//check if the position is in the addback hit pattern
      	if((cev->tg.h.AHP&(1<<(pos-1)))!=0)
	  {
	    take=0;
	    //Run through four cores for each position
	    for(col=0;col<NCOL;col++)
	      {
		//check if the position and color is in the hit pattern
		if((cev->tg.det[pos].hge.HHP&(1<<col))!=0)
		  if(cev->tg.det[pos].ge[col].h.FH>0)
		    //suppress if the position is in the map and has not yet been suppressed
		    if(take==0)
		      if(cev->tg.det[pos].ge[col].suppress>=940)
			if(cev->tg.det[pos].ge[col].suppress<=1060)
			  {
			    suppFlag=1;
			    take=1;
			  }
		
	      }
	  }
	sup[pos]&=suppFlag;
      }
  
  printf("SUPPRESSION ARRAY POST SUPPRESSION CHECK\n");
  for(pos=1;pos<NPOSTIGR;pos++)
    printf("pos %.2d E = %d\n",pos,sup[pos]);
  getc(stdin);		
  
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

  if(argc!=2)
    {
      printf("\n ./TIGRESS_EE_TTDiffGatedCluster_v2 master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts calibrated time histogram for TIGRESS cores \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  memset(hist,0,sizeof(hist));
  memset(mat,0,sizeof(mat));
  memset(ene,0,sizeof(ene));
  memset(sup,0,sizeof(sup));
  read_master(argv[1],name);
 
  h = new TH2D("Tigress EE TDiff","Tigress EE TDiff",S1K,0,S4K-1,S1K,0,S4K-1);
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


 
  if((output=fopen("matrix_v2.mat","w"))==NULL)
    {
      printf("\nI can't open file matrix_v2.mat\n");
      exit(EXIT_FAILURE);
    }
	  
  fwrite(mat,S4K*S4K*sizeof(short int),1,output);
  fclose(output);

  for(int i=0;i<S4K;i++)
    for(int j=0;j<S4K;j++)
      hist[i]+=mat[i][j];
    

  if((output=fopen("projection_v2.spn","w"))==NULL)
    {
      printf("\nI can't open file projection.spn\n");
      exit(EXIT_FAILURE);
    }
	  
  fwrite(hist,S4K*sizeof(int),1,output);
  fclose(output);

  sprintf(title,"Tigress_EE_TCalDiff_v2.root");
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

  

