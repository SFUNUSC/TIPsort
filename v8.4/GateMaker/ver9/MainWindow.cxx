#include "MainWindow.h"
#include "RootManager.h"

MainWindow *MainWindow::fMainWindow = NULL;

MainWindow *MainWindow::GetMainWindow()	{
	if(!fMainWindow)	{
		printf("MainWindow not created yet.\n");
		return 0;
	}
	return fMainWindow;
}


MainWindow::MainWindow(int width, int height)	
	:TGMainFrame(gClient->GetRoot(),width,height)	{

	SetWindowName("GateMaker");

	fMainWindow = this;

	fMasterFrame = new TGHorizontalFrame(this,width,height);

	fCanvasFrame	=	new TGHorizontalFrame(fMasterFrame,width/2,height);;
	fControlFrame	=	new TGHorizontalFrame(fMasterFrame,width/2,height);;

	fListHolderFrame = new TGVerticalFrame(fControlFrame,width/4, height);
	fListFrame		=	new TGHorizontalFrame(fListHolderFrame,width/4,height);;
	fInfoFrame		=	new TGVerticalFrame(fControlFrame,width/4,height);;

	fButtonFrame	=	new TGVerticalFrame(fInfoFrame,width/4,height/2);;
	fLabelFrame		=	new TGVerticalFrame(fInfoFrame,width/4,height/2);;

	fButtonLayout	=	new TGLayoutHints ( kLHintsExpandX | kLHintsExpandY, 0,0, 0, 0);


	Load = new TGTextButton(fButtonFrame, "Open Histograms",LOAD);
	Load->Associate(this);
	fButtonFrame->AddFrame(Load,fButtonLayout);

	Import = new TGTextButton(fButtonFrame, "Import Gates",IMPORT);
	Import->Associate(this);
	fButtonFrame->AddFrame(Import,fButtonLayout);

	ClearGates = new TGTextButton(fButtonFrame, "Clear Gates",CLGT);
	ClearGates->Associate(this);
	fButtonFrame->AddFrame(ClearGates,fButtonLayout);

	fSaveFile = new TGTextButton(fButtonFrame,"Save Gates",SAVEFILE);
	fSaveFile->Associate(this);
	fButtonFrame->AddFrame(fSaveFile,fButtonLayout);

	Gate = new TGTextButton(fButtonFrame, "Draw Gate",DRAWGATE);
	Gate->Associate(this);
	fButtonFrame->AddFrame(Gate,fButtonLayout);

	fGetGateButton = new TGTextButton(fButtonFrame,"Gate to List",ADDGATETOLIST);
	fGetGateButton->Associate(this);
	fButtonFrame->AddFrame(fGetGateButton,fButtonLayout);

	Quit = new TGTextButton(fButtonFrame,"Quit",QUIT);
	Quit->Associate(this);
	fButtonFrame->AddFrame(Quit,fButtonLayout);


	fLabel1 = new TGLabel(fLabelFrame,"Histogram File:");
	fLabel1-> SetTextJustify(kTextLeft);
	fLabelFrame->AddFrame(fLabel1,fButtonLayout);	

	fLabel2 = new TGLabel(fLabelFrame,"");
	fLabelFrame->AddFrame(fLabel2,fButtonLayout);	

	fLabel5 = new TGLabel(fLabelFrame,"Gates in File");
	fLabel5-> SetTextJustify(kTextLeft);
	fLabelFrame->AddFrame(fLabel5,fButtonLayout);	

	fLabel6 = new TGLabel(fLabelFrame,"");
	fLabelFrame->AddFrame(fLabel6,fButtonLayout);	


	fLabel3 = new TGLabel(fLabelFrame,"Gates from File:");
	fLabel3-> SetTextJustify(kTextLeft);
	fLabelFrame->AddFrame(fLabel3,fButtonLayout);	

	fLabel4 = new TGLabel(fLabelFrame,"");
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

  if(RootManager::instance()->HandleObject(item->GetText())==-1)
    fListTree->DeleteSelected();
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
	      case IMPORT:
		ImportAction();
		break;
	      case CLGT:
		ClearGatesAction();
		break;
	      case DRAWGATE:
		DrawGateAction();
		break;
	      case QUIT:
		QuitButtonAction();
		break;
	      case ADDGATETOLIST:
		GetGateAction();
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

void MainWindow::ClearGatesAction()	{
  RootManager::instance()->ClearGates();
  ClearGateTree();
  
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
		}
	}
}
/*========================================================================*/

void MainWindow::ImportAction()	{
	printf("open...\n");
	TGFileInfo file_info;
	const char *filetypes[] = {"ROOT Files","*.root", 0, 0};
	file_info.fFileTypes = filetypes;
	file_info.fIniDir = StrDup(".");
	new TGFileDialog(gClient->GetRoot(),this,kFDOpen,&file_info);
	printf("filename = %s\n",file_info.fFilename);
	fLabel4->SetText(file_info.fFilename);
	RootManager::instance()->ImportGates(file_info.fFilename);		
	

}

/*========================================================================*/

void MainWindow::DrawGateAction()	{
  RootManager::instance()->MakeCut();

}

/*========================================================================*/

void MainWindow::GetGateAction()	{
  RootManager::instance()->GateToList();
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

	RootManager::instance()->SaveGates(file_info.fFilename);
	fLabel6->SetText(file_info.fFilename);
}



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

/*========================================================================*/

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



/*========================================================================*/

void MainWindow::ClearListTree()	{
	while(fListTree->GetFirstItem())
		fListTree->DeleteItem(fListTree->GetFirstItem());
}

/*========================================================================*/

void MainWindow::ClearHistogramTree()	{
  TGListTreeItem *item;
  while((item=fListTree->FindItemByPathname("histograms"))!=0)
		fListTree->DeleteItem(item);
}

/*========================================================================*/


void MainWindow::ClearGateTree()	{
  TGListTreeItem *item;
  while((item=fListTree->FindItemByPathname("gates"))!=0)
		fListTree->DeleteItem(item);
}

/*========================================================================*/

