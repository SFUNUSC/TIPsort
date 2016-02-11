#include "sort.h"

int analyze_data(raw_event *data)
{
  switch(detectorSubsystem)
    {
    case 1:
      if(data->csiarray.h.TSfold==fold)
        {
          encode(data,output,enb);
          return SEPARATOR_DISCARD;
        }
      break;
    case 2:
      if(data->gr.h.Gefold==fold)
        {
          encode(data,output,enb);
          return SEPARATOR_DISCARD;
        }
      break;    
    case 3:
      if(data->tg.h.Gefold==fold)
        {
          encode(data,output,enb);
          return SEPARATOR_DISCARD;
        }
      break;
    case 4:
      if(data->pinarray.h.TSfold==fold)
        {
          encode(data,output,enb);
          return SEPARATOR_DISCARD;
        }
      break;
    default:
      printf("Not a valid detector subsystem selection!\n");
      printf("Re-execute---- 1 for CsI, 2 for Griffin, 3 for Tigress, 4 for PINArray.\n");
      break;
    }
  
 
  return SEPARATOR_DISCARD;
}
/*====================================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;

  if(argc!=4)
    {
      printf("\n ./separate_Fold master_file_name detector_subsystem_number fold_value\n");
      printf("\n Sorts events with the specified fold.\n");
      printf("Detector subsystem number required as third argument.\n");
      printf("---->1 for CsI, 2 for Griffin, 3 for Tigress, 4 for PINArray.<----\n\n");  
      exit(-1);
    }
  
  detectorSubsystem=atoi(argv[2]);
  fold=atoi(argv[3]);

  printf("Separating fold %i events from dectector subsystem %i.\n",fold,detectorSubsystem);
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(calibration_parameters*)malloc(sizeof(calibration_parameters));
  memset(cal_par,0,sizeof(calibration_parameters));

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

