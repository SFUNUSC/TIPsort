#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int type;
  double e;
  double chi,s,f,r,t;
  int aGateFlag=0;
  int pGateFlag=0;

  //check if all the elements of the setup are present; discard if not
  if((data->h.setupHP&RF_BIT)==0) return SEPARATOR_DISCARD;
  if((data->h.setupHP&CsIArray_BIT)==0) return SEPARATOR_DISCARD;
  if((data->h.setupHP&GRIFFIN_BIT)==0) return SEPARATOR_DISCARD;

  //work out number and type of particles identified in the event
  for(pos=1;pos<NCSI;pos++)
    if(positionGateFlag[pos]==1)
      if((data->csiarray.h.THP&(one<<pos))!=0)
	{
	  type=data->csiarray.wfit[pos].type;
	  chi=data->csiarray.wfit[pos].chisq;
	  chi/=data->csiarray.wfit[pos].ndf;
	  if(type>=idmin && type<=idmax)
	    if(chi>=chimin && chi<=chimax)
	      {
		t=16.*data->csiarray.wfit[pos].t[0]; 
		t-=data->rf.sin.t0;
		
		t+=S8K;
		if(t<0)t=S32K-2000;
		if(t>S32K) t=S32K-3000;
		
		t/=16;
		s=data->csiarray.wfit[pos].am[3];
		f=data->csiarray.wfit[pos].am[2];
		r=s/f*100;
		r+=100;
		
		if(pGate[pos]->IsInside(t,r)) pGateFlag++;
		if(aGate[pos]->IsInside(t,r)) aGateFlag++;
	      }}

  //check if there is proton or alpha; discard if not
  if((pGateFlag==0)&&(aGateFlag)==0) return SEPARATOR_DISCARD;
    
  //now calibrate GRIFFIN (earlier is just a waste of CPU)
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_GRIFFIN(data,&cal_par->gr,&cev->gr);

  if(cev->gr.h.FE>0)
    for(pos=1;pos<NPOSGRIF;pos++)
      if((cev->gr.h.EHP&(one<<(pos-1)))!=0)
	if(cev->gr.det[pos].hge.FE>0)
	  for(col=0;col<NCOL;col++)
	    if((cev->gr.det[pos].hge.EHP&(one<<col))!=0)
	      if(cev->gr.det[pos].ge[col].h.FE>0)
		if((cev->gr.det[pos].ge[col].h.EHP&one)!=0)
		  {
		    e=cev->gr.det[pos].ge[col].seg[0].E/cal_par->gr.contr_e;
		    if(e<0 || e>S32K) e=S32K-1000;
		    
		    //use explitic conditions
		    if((aGateFlag==1)&&(pGateFlag==1)) ap_hist[pos][col][(int)rint(e)]++;
		    if((aGateFlag==1)&&(pGateFlag==0)) a_hist[pos][col][(int)rint(e)]++;
		    if((aGateFlag==0)&&(pGateFlag==1)) p_hist[pos][col][(int)rint(e)]++;
		  }
  free(cev);
  return SEPARATOR_DISCARD;
}
/*=======================================================================*/
int main(int argc, char *argv[])
{
  input_names_type *name;
  FILE *output1,*output2,*output3,*gateNameFile,*cluster;
  TFile *f;
  int stop,det;
  char a[132],p[132],ap[132];
  char aGateName[132],pGateName[132],DataFile[132],detector[132];

  if(argc!=6)
    {
      printf("Griffin_ECal_CsIPIDTR master_file_name idmin idmax chimin chimax\n");
      exit(-1);
    }
  
  printf("Program generates Griffin ECal histograms separated on CsI_PID_TR.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));

  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

  memset(a_hist,0,sizeof(a_hist));
  memset(p_hist,0,sizeof(p_hist));
  memset(ap_hist,0,sizeof(ap_hist));
  read_master(argv[1],name);

  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  
  if(name->flag.cluster_file==1)
    {
      printf("Sorting ECal_PIDtrSum energy histograms for GRIFFIN cores based upon the cluster file: %s\n",name->fname.cluster_file);
      if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
	{
	  printf("ERROR! I can't open input file %s\n",name->fname.cluster_file);
	  exit(-2);
	}}
  else
    {
      printf("ERROR! Cluster file not defined\n");
      exit(-1);
    }

  if(name->flag.GRIFFIN_cal_par==1)
    {
      printf("Griffin calibration read from %s.\n",name->fname.GRIFFIN_cal_par);
      initialize_GRIFFIN_calibration(&cal_par->gr,name->fname.GRIFFIN_cal_par);
    }
  else
    {
      printf("ERROR!!! Griffin calibration parameters not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.root_gate_file==1)
    {
      printf("Using root file: %s\n",name->fname.root_gate_file);
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
      gateNameFile=fopen(name->fname.gate_name_file,"r");

      while(fscanf(gateNameFile,"%s %s %s",detector,aGateName,pGateName)!=EOF)
	for(pos=1;pos<NCSI;pos++)
	  {
	    det=atoi(detector);
	    if(det==pos)
	      {
		aGate[pos] = (TCutG *) gROOT->FindObject(aGateName);
		pGate[pos] = (TCutG *) gROOT->FindObject(pGateName);
		printf("The gates for detector %d are %s and %s\n",pos,aGateName,pGateName);
		positionGateFlag[pos]++;
	      }}
      fclose(gateNameFile);
    }
  else
    {
      printf("ERROR!!! Gate name file not defined!\n");
      exit(-1);
    }

  f->Close();
  
  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
  
      printf("Sorting data from %s\n", name);
      //name->flag.separate=1;
      sort(name); 
    }
  
  for(pos=1;pos<NPOSGRIF;pos++)
    {
      stop=0;
      for(col=0;col<NCOL;col++)
	stop+=cal_par->gr.ceflag[pos][col];
      if(stop>0)
	{
	  sprintf(a,"pos%1d%1d_ECal_aGate.mca",pos/10,pos-(pos/10)*10);
	  sprintf(p,"pos%1d%1d_ECal_pGate.mca",pos/10,pos-(pos/10)*10);
	  sprintf(ap,"pos%1d%1d_ECal_apGate.mca",pos/10,pos-(pos/10)*10);
	  if((output1=fopen(a,"w"))==NULL)
	    {
	      printf("ERROR!!! I cannot open the mca file\n");
	      exit(EXIT_FAILURE);
	    }
	  if((output2=fopen(p,"w"))==NULL)
	    {
	      printf("ERROR!!! I cannot open the mca file\n");
	      exit(EXIT_FAILURE);
	    }
	  if((output3=fopen(ap,"w"))==NULL)
	    {
	      printf("ERROR!!! I cannot open the mca file\n");
	      exit(EXIT_FAILURE);
	    }
	  
	  for(col=0;col<NCOL;col++)
	    {
	      fwrite(a_hist[pos][col],S32K*sizeof(int),1,output1);
	      fwrite(p_hist[pos][col],S32K*sizeof(int),1,output2);
	      fwrite(ap_hist[pos][col],S32K*sizeof(int),1,output3);
	    }
	  fclose(output1);
	  fclose(output2);
	  fclose(output3);
	}}}
