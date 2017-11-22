#include "sort.h"

int analyze_data(raw_event *data)
{
  uint64_t one=1;
  int np,na;
  double s,f,r,e;
  int type;
  double chisq;
  
  if((data->h.setupHP&RF_BIT)==0) 
    return SEPARATOR_DISCARD;

  if((data->h.setupHP&CsIArray_BIT)==0) 
    return SEPARATOR_DISCARD;

  np=0;
  na=0;

  for(pos=1;pos<NCSI;pos++)
    if((data->csiarray.h.THP[pos/64]&(one<<pos%64))!=0)
      {
	type=data->csiarray.wfit[pos].type;
	if(type==1)
	  {
	    e=data->csiarray.wfit[pos].am[1];
	    s=data->csiarray.wfit[pos].am[3];
	    f=data->csiarray.wfit[pos].am[2];
	    
	    /* for(int i=1;i<=4;i++) */
	    /* printf("amp[%d] %Lf\n",i,data->csiarray.wfit[pos].am[i]); */
	    
	    if(f==0)
	      r=0;
	    else
	      r=s/f*100;
	    
	    r+=100;

	    /* //for "rough" PID from 209Po */
	    /* if(e>=0. && e<=2000.) */
	    /*   if(r>100. && r<=300.) */
	    /*     { */
	    /*     na++; */
	    /*     /\* printf("kept event e %10.3f r %10.3f\n",e,r); *\/ */
	    /*     } */
	    /* /\* getc(stdin); *\/ */
	    
	    if(pGateFlag[pos]==1)
	      if(pGate[pos]->IsInside(e,r))
		np++;
	    if(aGateFlag[pos]==1)
	      if(aGate[pos]->IsInside(e,r))
		na++;
	    
	    /* if(np>0 || na>0) */
	    /*   { */
	    /*     printf("pos %d\n",pos); */
	    /*     printf("e %10.3f r %10.3f\n",e,r); */
	    /*     printf("na %d np %d\n",na,np); */
	    /*     getc(stdin); */
	    /*   } */
	  }
      }


  h->Fill(np,na);

  return SEPARATOR_DISCARD;
}
/*=======================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  FILE *gateNameFile;
  TFile* f;
  TCanvas *canvas;
  TApplication *theApp;
  char title[132],aGateName[132],pGateName[132],det[132];


  if(argc!=2)
    {
      printf("check_Fold2D_PIDER master_file\n");
      printf("Displays 2D fold histogram for protons and alphas whose gates at described in the gate_name_file specified in the master file.\n");
      exit(-1);
    }
  
  h = new TH2D("FoldHistogram","FoldHistogram",10,0,10,10,0,10);
  h->Reset();
  
  printf("Program sorts 2D fold histogram.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  read_master(argv[1],name);

  if(name->flag.inp_data!=1)
    {
      printf("\nInput data file not defined\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.out_data!=1)
    {
      printf("\nOutput data file not defined\n");
      exit(EXIT_FAILURE);
    }

  if(name->flag.root_gate_file==1)
    {
      printf("Using root gate file: %s\n",name->fname.root_gate_file);
      f=new TFile(name->fname.root_gate_file,"READ");
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

  memset(enb,0,sizeof(enb));
  enb[0]=BUFFER_TAG;
  enb[1]++;
  enb[1]++;

  sort(name); 
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas(title,title,10,10, 500, 300);
  h->GetXaxis()->SetTitle("proton Fold");
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitle("alpha Fold");
  h->GetYaxis()->CenterTitle(true);
  h->SetOption("COLZ");

  h->Draw();
  theApp->Run(kTRUE);
}
