#ifndef ROOTMANAGER_H
#define ROOTMANAGER_H

#include<stdio.h>
#include<vector>

#include<TROOT.h>
#include<TGClient.h>
#include<TFile.h>
#include<TCanvas.h>
#include<TH1.h>
#include<TH2.h>
#include<TCutG.h>
#include<TGTextEntry.h>
#include<TGMsgBox.h>
#include <TStyle.h>

#include "MainWindow.h"
using namespace std;

class RootManager	{

	public:
		static RootManager *instance();

	private:
		static RootManager *fRootManager;
		RootManager();
		~RootManager();

	private:
		TFile *fRootFile;
		TCanvas *fCanvas;

		std::vector<TCutG> fListOfCuts;		
		TCutG* cut;
		TCutG current;

  
		int numberofgatesmade;
		
		void InitRoot();

	public:
		void	 SetRootFile(TFile *fF) { fRootFile = fF;};
		TFile	*GetRootFile()	{	return fRootFile;};
		void	 SetCanvas(TCanvas *fC)	{	fCanvas = fC;};
		TCanvas *GetCanvas()	{return fCanvas;};
		TList *OpenFile(char*);
		void    MakeCut(void);
		void    GateToList(void);
		void    RefreshGateList(void);
		void 	HandleObject(const char*);
		void   SaveGates(const char*);
		void   ImportGates(const char*);
		void   ClearGates(void);
};

#endif
