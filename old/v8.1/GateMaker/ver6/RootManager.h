#ifndef ROOTMANAGER_H
#define ROOTMANAGER_H

#include<stdio.h>

#include<TROOT.h>
#include<TFile.h>
#include<TCanvas.h>
#include<TH1.h>
#include<TH2.h>
#include<TCutG.h>

#include <TStyle.h>

class RootManager	{

	public:
		static RootManager *instance();

	private:
		static RootManager *fRootManager;
		RootManager();
		~RootManager();

	private:
		TFile *fRootFile;
		TFile *fRootFileOut;
		TCanvas *fCanvas;

		TCutG *fCurrentCut;		
		int numberofgatesmade;

		void InitRoot();

	public:
		void	 SetRootFile(TFile *fF) { fRootFile = fF;};
		TFile	*GetRootFile()	{	return fRootFile;};

		void	 SetCanvas(TCanvas *fC)	{	fCanvas = fC;};
		TCanvas *GetCanvas()	{return fCanvas;};

	public:
		TList *OpenFile(char*);
		int		MakeCut(void);
		void 	HandleObject(const char*);
		TCutG *SaveGate();
		TFile *OpenSaveFile(const char*);
};

#endif
