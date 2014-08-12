#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  int type;
  double chi,s,f,r,t;
  
  if((data->h.setupHP&RF_BIT)==0) return SEPARATOR_DISCARD;
  if((data->h.setupHP&CsIArray_BIT)==0) return SEPARATOR_DISCARD;

  for(pos=1;pos<NCSI;pos++)
    if((data->csiarray.h.THP&(one<<pos))!=0)
      {
	type=data->csiarray.wfit[pos].type;
	chi=data->csiarray.wfit[pos].chisq;
	chi/=data->csiarray.wfit[pos].ndf;
	
	if((type>=idmin) && (type<=idmax))
	  if((chi>=chimin) && (chi<=chimax))
	    {
	      t=16.*data->csiarray.wfit[pos].t[0]; 
	      if((data->h.setupHP&RF_BIT)!=0)
		{
		  t-=data->rf.sin.t0;
		  t+=S8K;
		  
		  if(t<0)t=S32K-2;
		  if(t>S32K) t=S32K-3;
		}
	      else
		t=S32K-4;
	      
	      t/=16;
	      s=data->csiarray.wfit[pos].am[3];
	      f=data->csiarray.wfit[pos].am[2];
	      r=s/f*100;
	      r+=100;
	      
	      if(r>S32K-4) r=S32K-4;
	      h[pos]->Fill(t,r);
	      h_t[pos]->Fill(t);

	      hist1[(int)rint(r)]++;
	      hist2[(int)rint(t)]++;
	    }}
  
  return SEPARATOR_DISCARD;
}
/*==================================================================*/
int main(int argc, char *argv[])
{
  FILE *output, *cluster;
  input_names_type* name;
  char DataFile[132];
  char HistName1[132],HistName2[132];
    
  if(argc!=6)
    {
      printf("CsIArray_PID_TR_Sum cluster_file idmin idmax chimin chimax");
      exit(-1);
    }

  for(pos=1;pos<NCSI;pos++)
    {
      sprintf(HistName1,"PID_TR_%02d",pos);
      h[pos] = new TH2D(HistName1,HistName1,S8K,0,S32K,S1K/2,0,S1K/2);
      h[pos]->Reset();
      sprintf(HistName2,"PID_T_%02d",pos);
      h_t[pos] = new TH1D(HistName2,HistName2,S8K,0,S32K);
      h_t[pos]->Reset();
    }
  
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  
  printf("Program sorts PID histogram for the CsIArray \n");

  memset(hist1,0,sizeof(hist1));
  memset(hist2,0,sizeof(hist2));
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  if((cluster=fopen(argv[1],"r"))==NULL)
    {
      printf("ERROR!! I cannot open the cluster file!\n"); 
      exit(-2);
    }

  while(fscanf(cluster,"%s",DataFile)!=EOF)
    {
      printf("Reading data from %s\n",DataFile);
      strcpy(name->fname.inp_data,DataFile);
      sort(name);
    }

  if((output=fopen("CsIArray_PID_RatioSum.mca","w"))==NULL)
    {
      printf("ERROR! I can't open the mca file!\n"); 
      exit(EXIT_FAILURE);
    }
 
  fwrite(hist1,S32K*sizeof(int),1,output);
  fclose(output);

  if((output=fopen("CsIArray_PID_TimeSum.mca","w"))==NULL)
    {
      printf("ERROR! I can't open the mca file!\n"); 
      exit(EXIT_FAILURE);
    }
  
  fwrite(hist2,S32K*sizeof(int),1,output);
  fclose(output);  

  TFile *f= new TFile(name->fname.root_output_file,"RECREATE");
  f->cd();
  for(pos=1;pos<NCSI;pos++)
    {
      sprintf(HistName1, "PID_TR_%02d", pos);
      sprintf(HistName2, "PID_T_%02d", pos);
      h[pos]->Draw("COLZ");
      h[pos]->Write(HistName1);
      h_t[pos]->Draw("");
      h_t[pos]->Write(HistName2);
    }
  f->Close();
}
