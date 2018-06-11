#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdio.h>
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
#include "TGString.h"
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
  IMPORT,
  CLGT,
  QUIT,
  DRAWGATE,
  ADDGATETOLIST,
  SAVEFILE
};

class MainWindow:public TGMainFrame	{

	public:
		MainWindow(int,int);
		~MainWindow();

		static MainWindow* GetMainWindow();

		void ListTreeDoubleClickedAction(TGListTreeItem*);
		virtual Bool_t ProcessMessage (Long_t msg, Long_t parm1, Long_t);	

		void AddToListTree(const char *parentnode,TNamed *item);
		void AddToListTree(const char *parentnode,const char *currentnode);
		void ClearListTree();
		void ClearHistogramTree();
		void ClearGateTree();
		void QuitButtonAction();
		void OpenAction();
		void ImportAction();
		void ClearGatesAction();
		void DrawGateAction();
		void GetGateAction();
		void SaveFileAction();
	
	private:
		static MainWindow *fMainWindow;
		TGVerticalFrame   *fMainFrame;
		TGHorizontalFrame *fMasterFrame;
		TGHorizontalFrame *fCanvasFrame;
		TGHorizontalFrame *fControlFrame;
		TGVerticalFrame		*fListHolderFrame;
		TGVerticalFrame *fListFrame;
		TGVerticalFrame 	*fInfoFrame;
		TGVerticalFrame 	*fButtonFrame;
		TGVerticalFrame 	*fLabelFrame;

		TGHorizontalFrame 	*fLabelFrame1;
		TGHorizontalFrame 	*fLabelFrame2;
		TGHorizontalFrame 	*fLabelFrame3;

		TGLayoutHints *fCanvasLayout;
		TGLayoutHints *fListLayout;
		TGLayoutHints *fButtonLayout;
		TGLayoutHints *fLabelLayout;

		TGTextButton  *Load;
		TGTextButton  *Gate;
		TGTextButton  *Quit;
		TGTextButton  *fGetGateButton;
		TGTextButton  *fSaveFile;
		TGTextButton  *Import;
		TGTextButton  *ClearGates;
		
		TRootEmbeddedCanvas* ECanvas;

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

		/* char    str[132]; */
};


#endif







