
#include "RootManager.h"
/*===================================*/
RootManager *RootManager::fRootManager = NULL;
/*===================================*/
RootManager *RootManager::instance() {
	if(!fRootManager)
		fRootManager = new RootManager();
	return fRootManager;
}
/*===================================*/
RootManager::RootManager()	{
	printf("RootManager created.\n");
	InitRoot();
}
/*===================================*/
RootManager::~RootManager()	{	}


/*===================================*/
void RootManager::InitRoot()	{
	fRootFile = 0;

	gStyle->SetOptStat(10);
	gStyle->SetPalette(1);
	gStyle->SetCanvasColor(19);
	gStyle->SetFrameFillColor(10);
	gROOT->ForceStyle();

	ClearGates();


}



/*===================================*/
TList *RootManager::OpenFile(char *filename)	{

  RefreshGateList();
  MainWindow::GetMainWindow()->ClearHistogramTree();
  
  if(fRootFile!=NULL)
    {
      fRootFile->Close();
      delete fRootFile;
    }
  fRootFile = new TFile(filename);
  
  if(!fRootFile)	{
    printf("problem opening file\n");
    return NULL;
  }
  else 
    printf("file opened\n");
  
  fRootFile->cd();
  TList *fKeys = fRootFile->GetListOfKeys();
  
  return fKeys;

}

/*===================================*/
void RootManager::MakeCut(void)
{
  if(cut!=NULL)
    delete cut;
  cut=new TCutG();

  gROOT->SetEditorMode("CutG");
  cut = (TCutG*)gPad->WaitPrimitive("CUTG","CutG");
  
  if(!cut)
    printf("no gate found.\n");
   else
    {
      char buffer[256];
      sprintf(buffer,"cut_%i",numberofgatesmade++);	
      cut->SetName(buffer);
      cut->SetTitle(buffer);
    }
}
/*===================================*/
void RootManager::RefreshGateList(void)
{
  MainWindow::GetMainWindow()->ClearGateTree();
  std::vector<TCutG>::iterator itPos = fListOfCuts.begin();
  for(; itPos < fListOfCuts.end(); itPos++)
    MainWindow::GetMainWindow()->AddToListTree("gates",(TNamed*)(&(*itPos)));
}
/*===================================*/
void RootManager::GateToList(void)
{
  Int_t present=0;
  char buff[256];
  TCutG *same=NULL;
  //get the most recent version of the cut from root
  memcpy(&current,cut,sizeof(TCutG));

  std::vector<TCutG>::iterator itPos = fListOfCuts.begin();

  for(; itPos < fListOfCuts.end(); itPos++)
      if(!strcmp((*itPos).GetName(),current.GetName()))
	{
	  present=1;
	  same=&(*itPos);
	}
    
  if(present==0)
    {
      printf("Adding gate\n");
      fListOfCuts.push_back(current);
      MainWindow::GetMainWindow()->AddToListTree("gates",(TNamed*)(&current));
    }
  else
    {
      sprintf(buff,"Gate %s in the list\n Overwrite?",current.GetName());
      Int_t retval = 0;
      Int_t buttons = 0;
      buttons |= kMBYes;
      buttons |= kMBNo;
      new TGMsgBox(gClient->GetRoot(), MainWindow::GetMainWindow(), "Confirm",buff,kMBIconQuestion, buttons, &retval);
      if(retval==1)
	{
	  printf("Gate %s overwritten\n",current.GetName());
	  memcpy(same,&current,sizeof(TCutG));
	}
    }

}
/*===================================*/

void RootManager::HandleObject(const char *name)
{

	TObject *selected;
	//check if the object is in memory first.
	selected = gROOT->FindObjectAny(name);
	if(!selected) // if not in memory, check the in file.
		selected = fRootFile->Get(name);

	if(selected)	{
		if(selected->InheritsFrom("TH2"))
		  {
		  selected->Draw("COLZ");	 
		  fCanvas->Update();
		  }
		  
		else if(selected->InheritsFrom("TH1"))
		  {
		    selected->Draw();		  
		    fCanvas->Update();
		  }

		else if(selected->InheritsFrom("TCutG"))
		  {
			
		    //		    printf("Gate %s contains points:\n",selected->GetName());
		    selected->Draw();		  
		    fCanvas->Update();
		  }

	}
}



/*==========================================*/
void RootManager::SaveGates(const char *name)	{

  TFile *fRootFileOut;
  fRootFileOut = new TFile(name,"RECREATE");
  fRootFileOut->cd();
  std::vector<TCutG>::iterator itPos = fListOfCuts.begin();
  for(; itPos < fListOfCuts.end(); itPos++)
    (*itPos).Write();
  fRootFileOut->Close();
}

/*==========================================*/
void RootManager::ImportGates(const char *name)	{
  
  TFile *fRootFileOut;
  fRootFileOut = new TFile(name,"READ");
  fRootFileOut->cd();
 
  TList *fKeys = fRootFileOut->GetListOfKeys();
	
  TIter next(fKeys);
  while(TObject *obj = next())	{
    TObject *h_obj=fRootFileOut->Get(obj->GetName());
    if(h_obj->InheritsFrom("TCutG"))	{
      cut=(TCutG*)h_obj;
      GateToList();
    }
  }  
  fRootFileOut->Close();
}
/*==========================================*/
void RootManager::ClearGates(void)	
{
	numberofgatesmade=0;
	fListOfCuts.clear();
	RefreshGateList();
}
