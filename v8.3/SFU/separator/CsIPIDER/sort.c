#include "sort.h"

int analyze_data(raw_event *data)
{


  unsigned long long int one=1;
  int np,na;
  double s,f,r,e;
  

  if((data->h.setupHP&RF_BIT)==0) return SEPARATOR_DISCARD;
  if((data->h.setupHP&CsIArray_BIT)==0) return SEPARATOR_DISCARD;

  np=0;
  na=0;

  for(pos=1;pos<NCSI;pos++)
    if((data->csiarray.h.THP&(one<<pos))!=0)
      {
	e=data->csiarray.wfit[pos].am[1];
	s=data->csiarray.wfit[pos].am[3];
	f=data->csiarray.wfit[pos].am[2];
	r=s/f*100;
	r+=100;
	if(pGateFlag[pos]==1)
	  if(pGate[pos]->IsInside(e,r)) 
	    np++;
	if(aGateFlag[pos]==1)
	  if(aGate[pos]->IsInside(e,r)) 
	    na++;	 
      }
  
  if(np==gate_np)
    if(na==gate_na)
      encode(data,output,enb);

  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  FILE *gateNameFile;
  TFile* f;
  char aGateName[132],pGateName[132],det[132];

  if(argc!=4)
    {
      printf("\n ./separate_CsIArray_PID_ER master_file_name np na\n");
      exit(-1);
    }
 
  printf("Program separates events according to the specified CsI gates \n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));

  read_master(argv[1],name);
  gate_np=atoi(argv[2]);
  gate_na=atoi(argv[3]);

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
  if((output=fopen(name->fname.out_data,"w"))==NULL)
    {
      printf("\nI can't open output file %s for writing\n",name->fname.out_data);
      exit(-2);
    }
  memset(enb,0,sizeof(enb));
  enb[0]=BUFFER_TAG;
  enb[1]++;
  enb[1]++;

  sort(name);

 
}

