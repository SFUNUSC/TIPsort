#include "sort.h"

/*================================================================*/
int analyze_fragment(Tig10_event* ptr,short* waveform)
{
  int d;
  WaveFormPar wpar;
  double s,f,r;

  if(ptr->channel==chn)
    if((d=ptr->waveform_length)!=0)
      {
	if(h!=NULL) delete h;
	h=new TH1D("Waveform","Waveform",ptr->waveform_length,0,ptr->waveform_length);
	if(c!=NULL) delete c;
        c = new TCanvas("Waveform", "Waveform",10,10, 700, 500);

	wpar.baseline_range=RANGE;
	get_t10t90(d,waveform,&wpar);
	get_shape(4,d,waveform,par,&wpar);

        chi=par->chisq;//data->csiarray.wfit[pos].chisq;
        type=par->type;//data->csiarray.wfit[pos].type;
        chi/=par->ndf;//data->csiarray.wfit[pos].ndf;

        if((type>=idmin) && (type<=idmax))
          if((chi>=chimin) && (chi<=chimax))
            {
	      s=par->am[3];//data->csiarray.wfit[pos].am[3];
              f=par->am[2];//data->csiarray.wfit[pos].am[2];

	      r=(100*(s/f))+100;
	      r+=100;

              if((r>lowRatio) && (r<=highRatio))
                {
                  for(Int_t i=0;i<ptr->waveform_length;i++) h->Fill(i,waveform[i]);
                    		  
                  h->Draw();
                  theApp->Run(kTRUE);
		}}}
  return 0;
}
 /*================================================================*/
int main(int argc, char *argv[])
{
  //int ac;
  //char *av[10];
  char runFile[132];
  
  if(argc!=12)
    {
      printf("view_waveformType midasFile channel idmin idmax chimin chimax lowRatio highRatio trc tf ts\n");
      exit(-1);
    }
  
  par=(ShapePar*)malloc(sizeof(ShapePar));
  memset(par,0,sizeof(ShapePar));

  strcpy(runFile,argv[1]);
  chn       = atoi(argv[2]);
  idmin     = atoi(argv[3]);
  idmax     = atoi(argv[4]);
  chimin    = atoi(argv[5]);
  chimax    = atoi(argv[6]);
  lowRatio  = atoi(argv[7]);
  highRatio = atoi(argv[8]);
  
  par->t[1]=atof(argv[9]);
  par->t[2]=atof(argv[10]);
  par->t[3]=atof(argv[11]);


  printf("Sorting file %s for waveforms with PID ratios between %d and %d.\n",runFile,lowRatio,highRatio);
  theApp=new TApplication("App", &argc, argv);
  sort_but_not_assemble(runFile);
}
