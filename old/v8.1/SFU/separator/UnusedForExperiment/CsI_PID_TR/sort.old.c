#include "sort.h"

int analyze_data(raw_event *data)
{
  unsigned long long int one=1;
  double chi,s,f,r,t;
  int type;
  
  if((data->h.setupHP&CsIArray_BIT)==0) 
    {
      encode(data,output,enb);
      return SEPARATOR_DISCARD;
    }
  
  if((data->h.setupHP&RF_BIT)==0) 
    {
      encode(data,output,enb);
      return SEPARATOR_DISCARD;
    }
  
  for(csi=1;csi<NCSI;csi++)
    if(CsIGateFlag[csi]==1)
      if((data->csiarray.h.TSHP&(one<<csi))!=0)
	{
	  type=data->csiarray.wfit[csi].type;
	  chi=data->csiarray.wfit[csi].chisq;
	  chi/=data->csiarray.wfit[csi].ndf;
	  if(type>=idmin && type<=idmax)
	    {
	      if(chi>=chimin && chi<=chimax)
		{
		  t=data->csiarray.wfit[csi].t[0]*16; 
		  t-=data->rf.sin.t0;
		  t+=S8K;
		  
		  if(t<0)t=S32K-1000;
		  if(t>S32K) t=S32K-2000;
		}}
	  else t=S32K-3000;
	  
	  t/=16;
	  s=data->csiarray.wfit[csi].am[3];
	  f=data->csiarray.wfit[csi].am[2];
	  r=s/f*100;

	  if(Gate[csi]->IsInside(t,r+100)) return SEPARATOR_KEEP;
	  else return SEPARATOR_DISCARD;
	}
  encode(data,output,enb);
  return SEPARATOR_DISCARD;
}
/*=======================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TFile *f;
  FILE *gateNameFile;
  char GateName[132], detector[132];
  int det=0;

  if(argc!=6)
    {
      printf("Sep_CsIPIDtr master_file_name idmin idmax chimin chimax\n");
      exit(-1);
    }
  
  printf("Program separates events based on CsI_PIDtr.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  read_master(argv[1],name);
  
  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
  
  if(name->flag.inp_data!=1)
    {
      printf("ERROR!!! Input data file not defined!\n");
      exit(EXIT_FAILURE);
    }
  
  if(name->flag.out_data!=1)
    {
      printf("ERROR!!! Output data file not defined!\n");
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

      while(fscanf(gateNameFile,"%s %s",detector,GateName)!=EOF)
	for(csi=1;csi<NCSI;csi++)
	  {
	    det=atoi(detector);
	    if(det==csi)
	      {
		Gate[csi] = (TCutG *) gROOT->FindObject(GateName);
		printf("The gate for detector %d is %s.\n",csi,GateName);
		CsIGateFlag[csi]=1;
	      }
	    else printf("ERROR!!! Invalid detector number in gate name file!\n");
	  }
      fclose(gateNameFile);
    }
  else
    {
      printf("ERROR!!! Root gate not defined!\n");
      exit(-1);
    }
  f->Close();
  
  memset(enb,0,sizeof(enb));
  enb[0]=BUFFER_TAG;
  enb[1]++;
  enb[1]++;
  
  name->flag.separate=1;
  sort(name); 
  fwrite(enb,sizeof(int),BUFFSIZE,output);
}
