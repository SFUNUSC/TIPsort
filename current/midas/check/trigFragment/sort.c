#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{ 
	if((ptr->trigger_num&0x0fffffff)==trig)
    {
  
  printf("=========================================================\n");
  printf("Trigger  number : %8d\n",ptr->trigger_num&0x0fffffff);
  printf("           Port : %8d\n",ptr->port);
  printf("          Tig10 : %8d\n",ptr->tig10);
  printf("      Collector : %8d\n",ptr->collector);
  printf("        Channel : %8d\n",ptr->channel);
  printf("         Charge : %8d\n",ptr->charge);
  printf("Waveform length : %8d\n",ptr->waveform_length);
  
  /*if(ptr->waveform_length!=0)
    {  
      if(h!=NULL) delete h;
      h=new TH1D("Waveform","Waveform",ptr->waveform_length,0,ptr->waveform_length);
      if(c!=NULL) delete c;
      c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);
      for(Int_t i=0;i<ptr->waveform_length;i++)
	{
	  h->Fill(i,waveform[i]);
	}
      h->Draw();
      theApp->Run(kTRUE);
    }*/
    
    
    }
  
  
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  
  char *ext;

  if(argc!=3)
    {
      printf("check_MidasTrigFragment midas_input_data trigger_number\n");
      printf("Program prints information on fragments belonging to a certain trigger number in a midas file or list of midas files.\n");
      printf("Input is list (ext .list) or single file (ext .mid)\n");
      exit(-1);
    }

	printf("Program prints information on fragments belonging to a certain trigger number in a midas file or list of midas files.\n");
	
	trig=atoi(argv[2]);
	
	if(strrchr(argv[1],'.')!=NULL)
		ext=strrchr(argv[1],'.'); /* returns a pointer to the last . to grab extention*/
	else
		{
      printf ("ERROR!!! Imporoper midas file source extension.\n");
      printf("Input is list (ext .list) or single file (ext .mid)\n");
      exit(-2);
    }
    
	/* sort from a list of midas files */
  if(strcmp(ext,".list")==0)
    {
      printf ("Sorting from a list: found extension %s\n", ext);
      sort_but_not_assemble_list(argv[1]);
    }
  /* sort from a single midas file */
  else if(strcmp(ext,".mid")==0)
    {
      printf ("Sorting from a midas file: found extension %s\n", ext);
      sort_but_not_assemble(argv[1]);
    }
  else
    {
      printf ("ERROR!!! Imporoper midas file source extension.\n");
      printf("Input is list (ext .list) or single file (ext .mid)\n");
      exit(-2);
    }
	
  sort_but_not_assemble(argv[1]);

}
