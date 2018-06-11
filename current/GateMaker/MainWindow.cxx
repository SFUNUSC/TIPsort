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

	fMainFrame = new TGVerticalFrame(this,width,height);

	fMasterFrame = new TGHorizontalFrame(fMainFrame,width,0.85*height);
	fLabelFrame	=  new TGVerticalFrame(fMainFrame,width,0.15*height);

	fLabelFrame1	=  new TGHorizontalFrame(fLabelFrame,width,0.05*height);
	fLabelFrame2	=  new TGHorizontalFrame(fLabelFrame,width,0.05*height);
	fLabelFrame3	=  new TGHorizontalFrame(fLabelFrame,width,0.05*height);

	fCanvasFrame	=	new TGHorizontalFrame(fMasterFrame,0.7*width,0.85*height);
	fControlFrame	=	new TGHorizontalFrame(fMasterFrame,0.3*width,0.85*height);

	fListHolderFrame = new TGVerticalFrame(fControlFrame,0.2*width, 0.85*height);
	fListFrame		=	new TGVerticalFrame(fListHolderFrame,0.2*width,0.85*height);
	fInfoFrame		=	new TGVerticalFrame(fControlFrame,0.1*width,0.85*height);
	fButtonFrame	=	new TGVerticalFrame(fInfoFrame,0.1*width,0.85*height);

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
	
	TGLayoutHints *h=new TGLayoutHints ( kLHintsTop,50,0,0,10);
	fLabel1 = new TGLabel(fLabelFrame1 ,"  Histogram File:  ");
	fLabel1-> SetTextJustify(kTextLeft);
	fLabelFrame1 ->AddFrame(fLabel1,h);	
	
	char str[256];
	for(int i=0;i<256;i++) sprintf(&str[i],"%s"," ");
	fLabel2 = new TGLabel(fLabelFrame1,str);
	fLabel2-> SetTextJustify(kTextLeft);
	fLabelFrame1 ->AddFrame(fLabel2,h);	

	fLabel5 = new TGLabel(fLabelFrame2,"    Gates to File:  ");
	fLabel5-> SetTextJustify(kTextLeft);
	fLabelFrame2->AddFrame(fLabel5,h);	

	fLabel6 = new TGLabel(fLabelFrame2,str);
	fLabel6-> SetTextJustify(kTextLeft);
	fLabelFrame2->AddFrame(fLabel6,h);	

	fLabel3 = new TGLabel(fLabelFrame3," Gates from File:  ");
	fLabel3-> SetTextJustify(kTextLeft);
	fLabelFrame3->AddFrame(fLabel3,h);	
	
	fLabel4 = new TGLabel(fLabelFrame3,str);
	fLabel4-> SetTextJustify(kTextLeft);
	fLabelFrame3->AddFrame(fLabel4,h);	

	ECanvas = new TRootEmbeddedCanvas("ECanvas", fCanvasFrame,0.7*width,0.85*height);
	fCanvasFrame->AddFrame(ECanvas,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));


	fLTCanvas = new TGCanvas(fListFrame,0.2*width, 0.85*height);
	fLTViewPort = fLTCanvas->GetViewPort();
	fListTree = new TGListTree(fLTCanvas, kHorizontalFrame);
	fListTree->SetCheckMode(TGListTree::kRecursive);
	fLTList = new TList();
	fListFrame->AddFrame(fLTCanvas,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY ,2,2,2,2)); 
       
	fListTree->Associate(this);
	fListHolderFrame->AddFrame(fListFrame,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
	
	fInfoFrame->AddFrame(fButtonFrame,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kLHintsTop,0,0,0,0));

	fControlFrame->AddFrame(fListHolderFrame,new TGLayoutHints( kLHintsExpandY | kLHintsRight));
	fControlFrame->AddFrame(fInfoFrame,new TGLayoutHints( kLHintsExpandY | kLHintsLeft));

	fMasterFrame->AddFrame(fControlFrame,new TGLayoutHints( kLHintsLeft|kLHintsTop));
	fMasterFrame->AddFrame(fCanvasFrame,new TGLayoutHints( kLHintsRight|kLHintsTop));

	fLabelFrame->AddFrame(fLabelFrame1,new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));
	fLabelFrame->AddFrame(fLabelFrame2,new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));
	fLabelFrame->AddFrame(fLabelFrame3,new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));

	fMainFrame->AddFrame(fMasterFrame,new TGLayoutHints(kLHintsLeft | kLHintsTop));
	fMainFrame->AddFrame(fLabelFrame,new TGLayoutHints(kLHintsLeft | kLHintsBottom));

	this->AddFrame(fMainFrame,new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));


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

  fListTree->SortChildren(fListTree->FindItemByPathname("gates"));
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

	TGFileInfo file_info;
	const char *filetypes[] = {"ROOT Files","*.root", 0, 0};
	file_info.fFileTypes = filetypes;
	file_info.fIniDir = StrDup(".");
	new TGFileDialog(gClient->GetRoot(),this,kFDOpen,&file_info);
	fLabel2->ChangeText(file_info.fFilename);
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

	TGFileInfo file_info;
	const char *filetypes[] = {"ROOT Files","*.root", 0, 0};
	file_info.fFileTypes = filetypes;
	file_info.fIniDir = StrDup(".");
	new TGFileDialog(gClient->GetRoot(),this,kFDOpen,&file_info);
	fLabel4->SetText(file_info.fFilename);
	fLabel4->Layout();
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

	TGFileInfo file_info;
	const char *filetypes[] = {"ROOT Files","*.root", 0, 0};
	file_info.fFileTypes = filetypes;
	file_info.fIniDir = StrDup(".");
	new TGFileDialog(gClient->GetRoot(),this,kFDSave,&file_info);
	RootManager::instance()->SaveGates(file_info.fFilename);
	fLabel6->SetText(file_info.fFilename);
	fLabel6->Layout();
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

