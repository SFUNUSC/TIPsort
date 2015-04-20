#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  unsigned long long int one=1;
  int type;
  double eAddBack;
  double chi,s,f,r,t;
  int na,np,sp;
  
  
  //check if all the elements of the setup are present; discard if not
  if((data->h.setupHP&RF_BIT)==0) return SEPARATOR_DISCARD;
  if((data->h.setupHP&CsIArray_BIT)==0) return SEPARATOR_DISCARD;
  if((data->h.setupHP&GRIFFIN_BIT)==0) return SEPARATOR_DISCARD;
  
  ev++;
  na=0;
  np=0;
  
  /* work out number and type of particles identified in the event */
  for(pos=1;pos<NCSI;pos++)
    if((aGateFlag[pos]==1)||(pGateFlag[pos]==1))
      if((data->csiarray.h.TSHP&(one<<pos))!=0)
	{
	  type=data->csiarray.wfit[pos].type;
	  chi=data->csiarray.wfit[pos].chisq;
	  chi/=data->csiarray.wfit[pos].ndf;
	  if((type>=idmin) && (type<=idmax))
	    if((chi>=chimin) && (chi<=chimax))
	      {
		t=16.*data->csiarray.wfit[pos].t[0]; 
		t-=data->rf.sin.t0;		      
		t+=S4K;
		t/=16;
		s=data->csiarray.wfit[pos].am[3];
		f=data->csiarray.wfit[pos].am[2];
		r=100+s/f*100;
		if(pGateFlag[pos]==1)
		  {
		    if(pGate[pos]->IsInside(t,r)) np++;
		  }
		if(aGateFlag[pos]==1)
		  {
		    if(aGate[pos]->IsInside(t,r)) na++;
		  }}}

  if((np<pMax) && (na<aMax)) ch[np][na]++;
  sp=id[np][na];

  /* now calibrate GRIFFIN (earlier is just a waste of CPU) */
  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_GRIFFIN(data,&cal_par->gr,&cev->gr);
  
  if(cev->gr.h.FH>0)
    for(pos=1;pos<NPOSGRIF;pos++)
      if(((cev->gr.h.HHP&(one<<(pos-1)))!=0) && (cev->gr.det[pos].hge.FE>0))
	if((cev->gr.h.AHP&(1<<(pos-1)))!=0)
	  {
	    eAddBack = cev->gr.det[pos].addback.E/cal_par->gr.contr_e;
	    col = cev->gr.det[pos].addbackC;
	    if(eAddBack<0 || eAddBack>S32K-10) eAddBack=S32K-10;
	    hist[sp][(int)rint(eAddBack)]++;
	  }
  free(cev);
  return SEPARATOR_DISCARD;
}
/*=======================================================================*/
int main(int argc, char *argv[])
{
  input_names_type *name;
  FILE *out,*gateNameFile,*cluster;
  TFile *f;
  char aGateName[132],pGateName[132],det[132],DataFile[132];
  int np,na,sum;
  
  if(argc!=6)
    {
      printf("Griffin_ECalABSum_CsIPIDtr master_file_name idmin idmax chimin chimax\n");
      exit(-1);
    }
  
  printf("Program generates Griffin ECalABSum histograms separated on CsIPIDtr.\n");
  
  memset(ch,0,sizeof(ch));
  ev=0;
  memset(id,0,sizeof(id));
  id[0][0]=1;//random
  id[1][0]=2;//1p
  id[2][0]=3;//2p
  id[0][1]=4;//1a
  id[1][1]=5;//1a1p
  id[0][2]=6;//2a
  //everything else goes to spectrum 0
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));
  
  memset(hist,0,sizeof(hist));
  
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
      gateNameFile=fopen(name->fname.gate_name_file,"r");
      
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
	    }}
      fclose(gateNameFile);
    }
  else
    {
      printf("ERROR!!! Gate name file not defined!\n");
      exit(-1);
    }
  f->Close();
  
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
  
  while(fscanf(cluster,"%s",DataFile) != EOF)
    {
      memset(name,0,sizeof(input_names_type));
      strcpy(name->fname.inp_data,DataFile);
  
      printf("Sorting data from %s\n", name);
      sort(name); 
    }

  printf("Number of PID events is     %10d\n",ev);
  printf(" Np/Na");
  for(np=0;np<pMax;np++)
    printf(" %8d ",np);
  printf("\n");
  for(na=0;na<aMax;na++)
    {
      printf("   %2d ",na);
      
      for(int np=0;np<pMax;np++)
	printf(" %8d ",ch[np][na]);
      printf("\n");
    }
  sum=0;
  for(np=0;np<pMax;np++)
    for(na=0;na<aMax;na++)
      sum+=ch[np][na];

  printf("Fraction of PID events is   %10.1f\n",100.*sum/ev);
  printf(" Np/Na");
  for(int np=0;np<pMax;np++)
    printf(" %8d ",np);
  printf("\n");
  for(int na=0;na<aMax;na++)
    {
      printf("   %2d ",na);
      
      for(int np=0;np<pMax;np++)
	printf(" %8.1f ",100.*ch[np][na]/ev);
      printf("\n");
    }
 
  if((out=fopen("ECalSum.mca","w"))==NULL)
    {
      printf("ERROR!!! I cannot open the mca file\n");
      exit(EXIT_FAILURE);
    }
	  
  for(np=0;np<NSP;np++)
    fwrite(hist[np],S32K*sizeof(int),1,out);
 
  fclose(out);


}
