
#include "MainWindow.h"
#include "RootManager.h"

MainWindow::MainWindow(int width, int height)	
	:TGMainFrame(gClient->GetRoot(),width,height)	{
	//this = new TGMainFrame (this,width,height,kHorizontalFrame);
	SetWindowName("GateMaker");

	//this->AddFrame(this, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

	fMasterFrame = new TGHorizontalFrame(this,width,height);

	fCanvasFrame	=	new TGHorizontalFrame(fMasterFrame,width/2,height);;
	fControlFrame	=	new TGHorizontalFrame(fMasterFrame,width/2,height);;

	fListHolderFrame = new TGVerticalFrame(fControlFrame,width/4, height);
	fListFrame		=	new TGHorizontalFrame(fListHolderFrame,width/4,height);;
	fInfoFrame		=	new TGVerticalFrame(fControlFrame,width/4,height);;

	fButtonFrame	=	new TGVerticalFrame(fInfoFrame,width/4,height/2);;
	fLabelFrame		=	new TGVerticalFrame(fInfoFrame,width/4,height/2);;

	//fCanvasLayout	=	new TGLayoutHints ( kLHintsExpandX | kLHintsExpandY, 0,0, 0, 0);
	//fListLayout		=	new TGLayoutHints ( kLHintsExpandX | kLHintsExpandY, 0,0, 0, 0);
	fButtonLayout	=	new TGLayoutHints ( kLHintsExpandX | kLHintsExpandY, 0,0, 0, 0);
	//fLabelLayout	=	new TGLayoutHints ( kLHintsExpandX | kLHintsExpandY, 0,0, 0, 0);


	Load = new TGTextButton(fButtonFrame, "Open Load File",LOAD);
	Load->Associate(this);
	fButtonFrame->AddFrame(Load,fButtonLayout);

	fSaveFile = new TGTextButton(fButtonFrame,"Open Save File",SAVEFILE);
	fSaveFile->Associate(this);
	fButtonFrame->AddFrame(fSaveFile,fButtonLayout);

	Gate = new TGTextButton(fButtonFrame, "Draw gate",GATE);
	Gate->Associate(this);
	fButtonFrame->AddFrame(Gate,fButtonLayout);

	fSaveGateButton = new TGTextButton(fButtonFrame,"Save Gate",SAVEGATE);
	fSaveGateButton->Associate(this);
	fButtonFrame->AddFrame(fSaveGateButton,fButtonLayout);

	Quit = new TGTextButton(fButtonFrame,"Quit",QUIT);
	Quit->Associate(this);
	fButtonFrame->AddFrame(Quit,fButtonLayout);


	fLabel1 = new TGLabel(fLabelFrame,"Current In File:");
	fLabel1-> SetTextJustify(kTextLeft);
	fLabelFrame->AddFrame(fLabel1,fButtonLayout);	

	fLabel2 = new TGLabel(fLabelFrame,"");
	//fLabel2->Associate(this);
	fLabelFrame->AddFrame(fLabel2,fButtonLayout);	

	fLabel5 = new TGLabel(fLabelFrame,"Current Out File");
	fLabel5-> SetTextJustify(kTextLeft);
	fLabelFrame->AddFrame(fLabel5,fButtonLayout);	

	fLabel6 = new TGLabel(fLabelFrame,"");
	//fLabel5->Associate(this);
	fLabelFrame->AddFrame(fLabel6,fButtonLayout);	


	fLabel3 = new TGLabel(fLabelFrame,"Current Gate:");
	fLabel3-> SetTextJustify(kTextLeft);
	fLabelFrame->AddFrame(fLabel3,fButtonLayout);	

	fLabel4 = new TGLabel(fLabelFrame,"");
	//fLabel4->Associate(this);
	fLabelFrame->AddFrame(fLabel4,fButtonLayout);	



	ECanvas = new TRootEmbeddedCanvas("ECanvas", fCanvasFrame,width/2,height);
	fCanvasFrame->AddFrame(ECanvas,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));


	fLTCanvas = new TGCanvas(fListFrame,width/4, height);
	fLTViewPort = fLTCanvas->GetViewPort();
	fListTree = new TGListTree(fLTCanvas, kHorizontalFrame);
	fListTree->SetCheckMode(TGListTree::kRecursive);
	fLTList = new TList();
	fListFrame->AddFrame(fLTCanvas,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY ,2,2,2,2)); 
       
	fListTree->Associate(this);
	fListHolderFrame->AddFrame(fListFrame,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));



	fInfoFrame->AddFrame(fLabelFrame,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kLHintsBottom,0,0,0,0));
	fInfoFrame->AddFrame(fButtonFrame,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kLHintsTop,0,0,0,0));

	fControlFrame->AddFrame(fListHolderFrame,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kLHintsRight));
	fControlFrame->AddFrame(fInfoFrame,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kLHintsLeft,0,0,0,0));

	fMasterFrame->AddFrame(fControlFrame,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kLHintsLeft));
	fMasterFrame->AddFrame(fCanvasFrame,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kLHintsRight));

	this->AddFrame(fMasterFrame,new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));


	RootManager::instance()->SetCanvas(ECanvas->GetCanvas());

	MapSubwindows ();
	Resize(width+1,height);
	MapWindow ();
	Print ();
}

/*========================================================================*/
MainWindow::~MainWindow()	{	}
/*========================================================================*/




/*========================================================================*/
void MainWindow::ListTreeDoubleClickedAction(TGListTreeItem *item)	{
	printf("double clicked\t%s\n",item->GetText());
	RootManager::instance()->HandleObject(item->GetText());


/*
	TObject *selected = fRootFile->Get(item->GetText());
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
	}
*/
}

/*========================================================================*/
Bool_t MainWindow::ProcessMessage (Long_t msg, Long_t parm1, Long_t parm2)
{
  switch (GET_MSG (msg))
    {
    case kC_LISTTREE:
      switch (GET_SUBMSG(msg)) 
	{
	case kCT_ITEMCLICK:
	    break;
	case kCT_ITEMDBLCLICK:
	  {
	    TGListTreeItem *item;
	    if ((item = fListTree->GetSelected()) != 0)
	      ListTreeDoubleClickedAction(item);	      
	  }
	  break;
	default:
	  break;
	}
    case kC_COMMAND:
      switch (GET_SUBMSG (msg))
	{
	case kCM_BUTTON:
	  {
	    switch (parm1)
	      {
	      case LOAD:
					OpenAction();
		  		break;
	      case GATE:
					GateAction();
					break;
	      case QUIT:
					QuitButtonAction();
					break;
	      case SAVEGATE:
					SaveGateAction();
					break;
	      case SAVEFILE:
					SaveFileAction();
					break;
	      default:
		break;
	      }
	  }
	  break;
	default:
	  break;
	}
      break;
    default:
      break;
    }
   
    return kTRUE;
}

/*========================================================================*/

void MainWindow::QuitButtonAction()	{
	printf("quit\n");
	gApplication->Terminate();
}

/*========================================================================*/

void MainWindow::OpenAction()	{
	printf("open...\n");
	TGFileInfo file_info;
	const char *filetypes[] = {"ROOT Files","*.root", 0, 0};
	file_info.fFileTypes = filetypes;
	file_info.fIniDir = StrDup(".");
	new TGFileDialog(gClient->GetRoot(),this,kFDOpen,&file_info);
	printf("filename = %s\n",file_info.fFilename);
	fLabel2->SetText(file_info.fFilename);
	TList *fKeys = RootManager::instance()->OpenFile(file_info.fFilename);		
	
	TIter next(fKeys);
	while(TObject *obj = next())	{
		TObject *h_obj 	= RootManager::instance()->GetRootFile()->Get(obj->GetName());
		if(h_obj->InheritsFrom("TH1"))	{
		  AddToListTree("histograms",(TNamed*)h_obj);
			printf("%s\n",h_obj->GetName());
		}
	}
}

/*========================================================================*/

void MainWindow::GateAction()	{
	printf("gate action.\n");
	RootManager::instance()->MakeCut();
	
}

/*========================================================================*/

void MainWindow::SaveGateAction()	{
	printf("Save gate action.\n");
  TCutG *cut = RootManager::instance()->SaveGate();
	if(!cut)
		return;
	fLabel4->SetText(cut->GetName());
	AddToListTree("gates",(TNamed*)cut);
}

/*========================================================================*/

void MainWindow::SaveFileAction()	{
	printf("save file action.\n");

	printf("open save file...\n");
	TGFileInfo file_info;
	const char *filetypes[] = {"ROOT Files","*.root", 0, 0};
	file_info.fFileTypes = filetypes;
	file_info.fIniDir = StrDup(".");
	new TGFileDialog(gClient->GetRoot(),this,kFDOpen,&file_info);

	printf("filename = %s\n",file_info.fFilename);

	TFile *f = RootManager::instance()->OpenSaveFile(file_info.fFilename);
	fLabel6->SetText(f->GetName());
}

/*========================================================================*/


/*========================================================================*/
void MainWindow::AddToListTree(const char *parentnode,TNamed *item)	{
	if(!fListTree->FindItemByPathname(parentnode))	{
		if(parentnode == NULL){
			fListTree->AddItem(0,item->GetName());	
			return;
		}
		fListTree->AddItem(0,parentnode);	
	}

	fListTree->AddItem(fListTree->FindItemByPathname(parentnode),item->GetName());	
	
	if(item->InheritsFrom(TH1::Class()))
		fListTree->FindChildByName(fListTree->FindItemByPathname(parentnode),item->GetName())->SetPictures(gClient->GetPicture("h1_t.xpm"), gClient->GetPicture("h1_t.xpm"));
	if(item->InheritsFrom(TH2::Class()))
		fListTree->FindChildByName(fListTree->FindItemByPathname(parentnode),item->GetName())->SetPictures(gClient->GetPicture("h2_t.xpm"), gClient->GetPicture("h2_t.xpm"));
	fListTree->SortSiblings(fListTree->GetFirstItem());
	if(item->InheritsFrom(TCutG::Class()))
		fListTree->FindChildByName(fListTree->FindItemByPathname(parentnode),item->GetName())->SetPictures(gClient->GetPicture("bld_cut.png"), gClient->GetPicture("bld_cut.png"));

}


void MainWindow::AddToListTree(const char *parentnode,const char *currentnode)	{

	if(!fListTree->FindItemByPathname(parentnode))	{
		if(parentnode == NULL){
			fListTree->AddItem(0,currentnode);
			return;
		}
		fListTree->AddItem(0,parentnode); 
	}
	fListTree->AddItem(fListTree->FindItemByPathname(parentnode),currentnode);	
	fListTree->SortSiblings(fListTree->GetFirstItem());
}




void MainWindow::ClearListTree()	{
	while(fListTree->GetFirstItem())
		fListTree->DeleteItem(fListTree->GetFirstItem());
}

/*========================================================================*/

