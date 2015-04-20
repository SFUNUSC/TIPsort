#include<stdio.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include "TROOT.h"
#include <TGFrame.h>
#include <TGClient.h>
#include <TApplication.h>
#include <TGFrame.h>
#include <TGButton.h>
#include <TGListTree.h>
#include <TGMenu.h>
#include <TGApplication.h>

#include "TCanvas.h"
#include "TRootEmbeddedCanvas.h"
#include <TGFileDialog.h>
#include "TGGC.h"
#include "TFile.h"
#include "TGClient.h"
#include "TGResourcePool.h"
#include "TGLabel.h"

#include <iostream>

enum Commands //commands for the menu bar popups
{
  LOAD,
  GATE,
	QUIT,
	SAVEGATE,
	SAVEFILE
};

class MainWindow:public TGMainFrame	{

	public:
		MainWindow(int,int);
		~MainWindow();

		void ListTreeDoubleClickedAction(TGListTreeItem*);
		virtual Bool_t ProcessMessage (Long_t msg, Long_t parm1, Long_t);	

		void AddToListTree(const char *parentnode,TNamed *item);
		void AddToListTree(const char *parentnode,const char *currentnode);
		void ClearListTree();
	
		//void OpenFile(char*);
		//int MakeCut(void);
		void QuitButtonAction();
		void OpenAction();
		void GateAction();
		void SaveGateAction();
		void SaveFileAction();

	private:
		//TGMainFrame *fMain;

		TGHorizontalFrame *fMasterFrame;
		TGHorizontalFrame *fCanvasFrame;
		TGHorizontalFrame *fControlFrame;
		TGVerticalFrame		*fListHolderFrame;
		TGHorizontalFrame *fListFrame;
		TGVerticalFrame 	*fInfoFrame;
		TGVerticalFrame 	*fButtonFrame;
		TGVerticalFrame 	*fLabelFrame;

		TGLayoutHints *fCanvasLayout;
		TGLayoutHints *fListLayout;
		TGLayoutHints *fButtonLayout;
		TGLayoutHints *fLabelLayout;

		TGTextButton  *Load;
		TGTextButton  *Gate;
		TGTextButton  *Quit;
		TGTextButton  *fSaveGateButton;
		TGTextButton  *fSaveFile;

		TRootEmbeddedCanvas* ECanvas;
		//TGLayoutHints *lCanvas,*lHFrame,*lLoad;

		//TCanvas *fCanvas;
		TGListTree    *fListTree;
		TGCanvas      *fLTCanvas;
		TGViewPort    *fLTViewPort;
		TList         *fLTList;

		TGLabel *fLabel1;
		TGLabel *fLabel2;
		TGLabel *fLabel3;
		TGLabel *fLabel4;
		TGLabel *fLabel5;
		TGLabel *fLabel6;
	 //TFile *fRootFile;


};










