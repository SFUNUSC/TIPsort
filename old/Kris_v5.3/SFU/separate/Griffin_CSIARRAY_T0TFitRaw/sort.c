#include "sort.h"

int analyze_data(raw_event *data)
{
 
  unsigned long long int one=1,none=-1;
  int pos,type,col,csi;
  double tgr,tcsi,chi;
  unsigned long long int flag=0,kill;

  if((data->h.setupHP&GRIFFIN_BIT)==0)
    {
      encode(data,output,enb);
      return SEPARATOR_DISCARD;
    }

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
 
  
  for(pos=1;pos<NPOSGRIF;pos++)
    for(col=0;col<NCOL;col++)
      {
	tgr=data->gr.det[pos].ge[col].t0[0];	
	if(tgr>0)
	  {
	    tgr-=data->rf.sin.t0;
	    tgr/=16;
	    tgr+=512;

	    
	    for(csi=1;csi<NCSI;csi++)
	      if((data->csiarray.h.THP&(one<<csi))!=0)
			  {
			    type=data->csiarray.wfit[csi].type;
			    chi=data->csiarray.wfit[csi].chisq;
			    chi/=data->csiarray.wfit[csi].ndf;
			    if(type>=idmin)
			      if(type<=idmax)
				if(chi>=chimin)
				  if(chi<=chimax)
				    {
				      tcsi=16.*data->csiarray.wfit[csi].t[0]; 				      
				      tcsi-=data->rf.sin.t0;				      
				      tcsi+=S4K;
				      tcsi/=16;
				      if(tcsi<0)tcsi=S32K-2;
				      if(tcsi>S32K) tcsi=S32K-3;
				      

				      if(g->IsInside(tgr,tcsi))
					flag|=(one<<csi);						     
				    }
			  }
	  }
      }
  
  //drop csi with the flag of zero
  for(csi=1;csi<NCSI;csi++)
    if((data->csiarray.h.TSHP&(one<<csi))!=0)
      if((flag&(one<<csi))==0)
	{
	  memset(&data->csiarray.csi[csi],0,sizeof(channel));
	  memset(&data->csiarray.wfit[csi],0,sizeof(ShapePar));
	  memset(&data->csiarray.t0[csi],0,sizeof(double));
	  data->csiarray.h.Efold--;
	  data->csiarray.h.Tfold--;	  
	  data->csiarray.h.TSfold--;
	  kill=none-(one<<csi);
	  data->csiarray.h.TSHP&=kill;
	  data->csiarray.h.EHP&=kill;
	  data->csiarray.h.THP&=kill;
	}
  
  if(data->csiarray.h.TSfold<=0)
    {
      kill=none-CsIArray_BIT; 
      data->h.setupHP&=kill;
      memset(&data->csiarray,0,sizeof(CsIArray));
    }

    encode(data,output,enb);
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/

int main(int argc, char *argv[])
{

  input_names_type* name;
  TFile *f;

  if(argc!=6)
    {
      printf("\n ./separate_Griffin_CSIARRAY_T0TFitRaw master_file_name idmin idmax chimin chimax\n");
      exit(-1);
    }

  printf("Program separates events based on Griffin/CsI timing\n");
  
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


 
  if(name->flag.root_file==1)
        {
          printf("\nUsing root file:\n %s\n",name->fname.root_file);
	  f=new TFile(name->fname.root_file,"READ");
	  
        }
      else
        {
          printf("\nRoot file not defined\n");
          exit(-1);
        }

  if(name->flag.root_gate==1)
    {
      printf("\nUsing root gate:\n %s\n",name->fname.root_gate_name);
      g = (TCutG *) gROOT->FindObject(name->fname.root_gate_name);
	  
        }
      else
        {
          printf("\nRoot gate not defined\n");
          exit(-1);
        }

  f->Close();


  idmin=atoi(argv[2]);
  idmax=atoi(argv[3]);
  chimin=atof(argv[4]);
  chimax=atof(argv[5]);
 
 
  name->flag.separate=0;
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
  // save the last buffer
  fwrite(enb,sizeof(int),BUFFSIZE,output);
 
}
