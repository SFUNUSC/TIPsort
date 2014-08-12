#include "sort.h"

int analyze_data(raw_event *data)
{

  unsigned long long int one=1,none=-1;
  int pos1,pos2,type;
  double t1,t2,chi;
  unsigned long long int flag=0,kill;
 
  for(pos1=1;pos1<NCSI;pos1++)
    if((data->csiarray.h.THP&(one<<pos1))!=0)
      {
	type=data->csiarray.wfit[pos1].type;
	chi=data->csiarray.wfit[pos1].chisq;
	chi/=data->csiarray.wfit[pos1].ndf;
	if(type>=idmin)
	  if(type<=idmax)
	    if(chi>=chimin)
	      if(chi<=chimax)
		{
		  t1=16.*data->csiarray.wfit[pos1].t[0]; 
		  if((data->h.setupHP&RF_BIT)!=0)
		    {
		      t1-=data->rf.sin.t0;
		   
		      t1+=S4K;
		      t1/=16;
		      if(t1<0)t1=S32K-2;
		      if(t1>S32K) t1=S32K-3;
		    }
		  else
		    t1=S32K-4;

		  for(pos2=pos1+1;pos2<NCSI;pos2++)
		    if((data->csiarray.h.THP&(one<<pos2))!=0)
		      {
			type=data->csiarray.wfit[pos2].type;
			chi=data->csiarray.wfit[pos2].chisq;
			chi/=data->csiarray.wfit[pos2].ndf;
			if(type>=idmin)
			  if(type<=idmax)
			    if(chi>=chimin)
			      if(chi<=chimax)
				{
				  t2=16.*data->csiarray.wfit[pos2].t[0]; 
				  if((data->h.setupHP&RF_BIT)!=0)
				    {
				      t2-=data->rf.sin.t0;		   
				      t2+=S4K;
				      t2/=16;
				      if(t2<0)t2=S32K-2;
				      if(t2>S32K) t2=S32K-3;
				    }
				  else
				    t2=S32K-4;


				  if(g->IsInside(t1,t2)||g->IsInside(t2,t1))
				    {
				      flag|=(one<<pos1);
				      flag|=(one<<pos2);
				    }						
				}
		      }
		  
		}
      }

  //drop particles with the flag of zero
  for(pos1=1;pos1<NCSI;pos1++)
    if((data->csiarray.h.THP&(one<<pos1))!=0)
      if((flag&(one<<pos1))==0)
	{
	  memset(&data->csiarray.csi[pos1],0,sizeof(channel));
	  memset(&data->csiarray.wfit[pos1],0,sizeof(ShapePar));
	  memset(&data->csiarray.t0[pos1],0,sizeof(double));
	  data->csiarray.h.Efold--;
	  data->csiarray.h.Tfold--;	  
	  data->csiarray.h.TSfold--;
	  kill=none-(one<<pos1);
	  data->csiarray.h.TSHP&=kill;
	  data->csiarray.h.EHP&=kill;
	  data->csiarray.h.THP&=kill;
	}

  if(data->csiarray.h.TSfold==0)
    {
      kill=none-CsIArray_BIT; 
      data->h.setupHP&=kill;
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
      printf("\n ./separate_CSI_TTFitRaw master_file_name idmin idmax chimin chimax\n");
      exit(-1);
    }

  printf("Program separates events based on CsI fit timing in fold 2\n");
  
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

 
  idmin=atof(argv[2]);
  idmax=atof(argv[3]);
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
   
}
