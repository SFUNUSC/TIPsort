#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  //unsigned long long int one=1;
  int posTIG,colTIG,posBGO,colBGO,supBGO;
  double tig,bgo;
  //double ren,ran;
  
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));

//Check Ge fold
  if(data->tg.h.Gefold>0)
    //Run through all positions
    for(posTIG=1;posTIG<NPOSTIGR;posTIG++)
      //Check if the position is indicated in the hit pattern
      if((data->tg.h.GeHP&(1<<(posTIG-1)))!=0)
	//Check if the position has a fold great than zero
	if(data->tg.det[posTIG].h.Gefold>0)
	  //Run through four cores for each position
	  for(colTIG=0;colTIG<NCOL;colTIG++)
	    //Check if this position is indicated in the hit pattern
	    if((data->tg.det[posTIG].h.GeHP&(1<<colTIG))!=0)
	      //Check that this combination has a time fold great than zero
	      if(data->tg.det[posTIG].ge[colTIG].h.Tfold>0)
		//Check if this combination is indicated in the time hit pattern
		if((data->tg.det[posTIG].ge[colTIG].h.THP&1)!=0)
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
  free(cev);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  char title[132];
  int i,j;
  double N,sum,sq;
  double mean,var;

  if(argc!=2)
    {
      printf("TigressBGO_TTHP master_file_name\n");
      exit(-1);
    }
  
  h = new TH2D("TigressBGO TTHP","TigressBGO TTHP",NPOSTIGR*NCOL,0,NPOSTIGR*NCOL-1,NPOSTIGR*NCOL*NSUP,0,NPOSTIGR*NCOL*NSUP-1);
  h->Reset();


  printf("Program sorts 2D hit pattern for Tigress and BGO using good time hit patterns \n");
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));

  read_master(argv[1],name);

  if(name->flag.inp_data!=1)
    {
      printf("\nInput data file not defined\n");
      exit(EXIT_FAILURE);
    }

  sort(name);

  //write histogram to file so you can look at it
  sprintf(title,"TigressBGO_TTHP.root");
  TFile f(title, "recreate");
  h->GetXaxis()->SetTitle("TIGRESS hit pattern");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("BGO hit pattern");
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetOption("COLZ");
  gStyle->SetPalette(1);
  h->Write();

  //read histogram and make the map
  for(i=0;i<NPOSTIGR*NCOL-1;i++)
    {
      N = 0.;
      sum = 0.;
      var = 0.;
      sq=0.;

      for(j=0;j<NPOSTIGR*NCOL*NSUP-1;j++)
	{
	  if(tthp[i][j] != 0)
	    N += 1.;
	  
	  sum += tthp[i][j];
	  sq += tthp[i][j]*tthp[i][j];
	  
	  //printf("tthp tig: %d bgo: %d count: %d\n",i,j,tthp[i][j]);
	  //getc(stdin);
	}
      mean = sum/N;
      var = sqrt((sq-(sum*sum)/N)/(N-1));
      printf("tthp tig: %d bgo: %d sum: %f N: %f mean: %f, var: %f\n",i,j,sum,N,mean,var);
      //getc(stdin);
    }
  

}

