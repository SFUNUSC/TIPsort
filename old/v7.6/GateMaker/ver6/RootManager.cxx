
#include "RootManager.h"

RootManager *RootManager::fRootManager = NULL;

RootManager *RootManager::instance() {
	if(!fRootManager)
		fRootManager = new RootManager();
	return fRootManager;
}

RootManager::RootManager()	{
	printf("RootManager created.\n");
	InitRoot();
}

RootManager::~RootManager()	{	}



void RootManager::InitRoot()	{
	fRootFile = 0;

	gStyle->SetOptStat(10);
	gStyle->SetPalette(1);
	gStyle->SetCanvasColor(19);
	gStyle->SetFrameFillColor(10);
	gROOT->ForceStyle();

	numberofgatesmade=0;
}



/*===================================*/
TList *RootManager::OpenFile(char *filename)	{

//	if(fRootFile)
//		ClearListTree();
	fRootFile = new TFile(filename);
	
	if(!fRootFile)	{
		printf("problem opening file\n");
		return NULL;
	}
	else 
		printf("file opened\n");

	fRootFile->cd();
	TList *fKeys = fRootFile->GetListOfKeys();
//	TIter next(fKeys);
//	while(TObject *obj = next())	{
//		TObject *h_obj 	= fRootFile->Get(obj->GetName());
//		if(h_obj->InheritsFrom("TH1"))	{
//		  AddToListTree(0,(TNamed*)h_obj);
//			printf("%s\n",h_obj->GetName());
//		}
//	}
	return fKeys;

}



/*===================================*/
int RootManager::MakeCut(void)
{
    gROOT->SetEditorMode("CutG");
		//fCurrentCut	=	(TCutG*)gROOT->GetListOfSpecials()->FindObject("CUTG");
    return 1;
}
/*===================================*/

void RootManager::HandleObject(const char *name)
{

	TObject *selected = fRootFile->Get(name);
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
				printf("I don't know what to do if someone clicks on this yet.\n");
		    //selected->Draw();		  
		    //fCanvas->Update();
		  }

	}
}

/*==========================================*/

TCutG *RootManager::SaveGate()	{
	fCurrentCut	=	(TCutG*)gROOT->GetListOfSpecials()->FindObject("CUTG");
	if(!fCurrentCut)	{
		printf("no gate found.\n");
		return NULL;
	}
	char buffer[256];
	sprintf(buffer,"cut_%i",numberofgatesmade++);	
	fCurrentCut->SetName(buffer);
	if(fRootFileOut)	{
		fRootFileOut->cd();
		fCurrentCut->Write();
		fRootFileOut->Write();
	}
	return fCurrentCut;	
}


TFile *RootManager::OpenSaveFile(const char *name)	{
	fRootFileOut = new TFile(name,"Update");
	return fRootFileOut;
}

