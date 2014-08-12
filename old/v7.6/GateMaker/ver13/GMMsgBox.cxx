// @(#)root/gui:$Id: GMMsgBox.cxx 35518 2010-09-21 09:54:10Z bellenot $
// Author: Fons Rademakers   09/01/98

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/
/**************************************************************************

    This source is based on Xclass95, a Win95-looking GUI toolkit.
    Copyright (C) 1996, 1997 David Barth, Ricky Ralston, Hector Peraza.

    Xclass95 is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

**************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMsgBox                                                              //
//                                                                      //
// A message dialog box.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "GMMsgBox.h"
#include "TGButton.h"
#include "TGIcon.h"
#include "TGLabel.h"
#include "TList.h"



//______________________________________________________________________________
GMMsgBox::GMMsgBox(const TGWindow *p, const TGWindow *main,
                   const char *title, const char *msg, const TGPicture *icon,
                   Int_t buttons, Int_t *ret_code, UInt_t options,
                   Int_t text_align) :
   TGTransientFrame(p, main, 10, 10, options)
{
   // Create a message dialog box.

   if (p)
      PMsgBox(title, msg, icon, buttons, ret_code, text_align);
   else
      MakeZombie();
}

//______________________________________________________________________________
GMMsgBox::GMMsgBox(const TGWindow *p, const TGWindow *main,
                   const char *title, const char *msg, GMEMsgBoxIcon icon,
                   Int_t buttons, Int_t *ret_code, UInt_t options,
                   Int_t text_align) :
   TGTransientFrame(p, main, 10, 10, options)
{
   // Create a message dialog box with the following parameters:.
   //       title: Window title
   //         msg: Message to be shown ('\n' may be used to split it in lines)
   //        icon: Picture to be shown at the left on the dialog window.
   //              It might take any of the following values:
   //              kGMIconStop, kGMIconQuestion,
   //              kGMIconExclamation, kGMIconAsterisk
   //     buttons: Buttons to be shown at the botton of the dialgo window.
   //              Look at GMEMsgBoxButton for the different possible values.
   //    ret_code: It will hold the value of the button pressed when the
   //              dialog is closed
   //     options: Frame options of this dialog window.
   //  text_align: Align options for 'msg'. See ETextJustification for the values.

   const TGPicture *icon_pic;

   switch (icon) {
      case kGMIconStop:
         icon_pic = fClient->GetPicture("mb_stop_s.xpm");
         if (!icon_pic) Error("GMMsgBox", "mb_stop_s.xpm not found");
         break;

      case kGMIconQuestion:
         icon_pic = fClient->GetPicture("mb_question_s.xpm");
         if (!icon_pic) Error("GMMsgBox", "mb_question_s.xpm not found");
         break;

      case kGMIconExclamation:
         icon_pic = fClient->GetPicture("mb_exclamation_s.xpm");
         if (!icon_pic) Error("GMMsgBox", "mb_exclamation_s.xpm not found");
         break;

      case kGMIconAsterisk:
         icon_pic = fClient->GetPicture("mb_asterisk_s.xpm");
         if (!icon_pic) Error("GMMsgBox", "mb_asterisk_s.xpm not found");
         break;

      default:
         icon_pic = 0;
         break;
   }

   if (p)
      PMsgBox(title, msg, icon_pic, buttons, ret_code, text_align);
   else
      MakeZombie();
}

//______________________________________________________________________________
void GMMsgBox::PMsgBox(const char *title, const char *msg,
                       const TGPicture *icon, Int_t buttons, Int_t *ret_code,
                       Int_t text_align)
{
   // Protected, common message dialog box initialization.

   UInt_t nb, width, height;

   fPlot = fOverlay = fDelete = fRename = fIntegrate = fIgnore = fCancel = fClose =
   fYesAll = fNoAll = fNewer = fAppend = fDismiss   = 0;
   fIcon      = 0;
   fMsgList   = new TList;
   fRetCode   = ret_code;
   nb = width = 0;

   // create the buttons

   fButtonFrame = new TGVerticalFrame(this, 60, 20);
   fL1 = new TGLayoutHints(kLHintsCenterY | kLHintsExpandX, 3, 3, 0, 0);

   buttons &= (kGMPlot | kGMOverlay | kGMDelete | kGMRename |
               kGMIntegrate | kGMIgnore | kGMCancel | kGMClose | kGMDismiss |
               kGMYesAll | kGMNoAll | kGMAppend | kGMNewer);
   if (buttons == 0) buttons = kGMDismiss;

   if (buttons & kGMPlot) {
      fPlot = new TGTextButton(fButtonFrame, new TGHotString("&Plot"), kGMPlot);
      fPlot->Associate(this);
      fButtonFrame->AddFrame(fPlot, fL1);
      width = TMath::Max(width, fPlot->GetDefaultWidth()); ++nb;
   }

   if (buttons & kGMOverlay) {
      fOverlay = new TGTextButton(fButtonFrame, new TGHotString("&Overlay"), kGMOverlay);
      fOverlay->Associate(this);
      fButtonFrame->AddFrame(fOverlay, fL1);
      width = TMath::Max(width, fOverlay->GetDefaultWidth()); ++nb;
   }

 
   if (buttons & kGMRename) {
      fRename = new TGTextButton(fButtonFrame, new TGHotString("&Rename"), kGMRename);
      fRename->Associate(this);
      fButtonFrame->AddFrame(fRename, fL1);
      width = TMath::Max(width, fRename->GetDefaultWidth()); ++nb;
   }

  if (buttons & kGMDelete) {
      fDelete = new TGTextButton(fButtonFrame, new TGHotString("&Delete"), kGMDelete);
      fDelete->Associate(this);
      fButtonFrame->AddFrame(fDelete, fL1);
      width = TMath::Max(width, fDelete->GetDefaultWidth()); ++nb;
   }

   if (buttons & kGMIntegrate) {
      fIntegrate = new TGTextButton(fButtonFrame, new TGHotString("&Integrate"), kGMIntegrate);
      fIntegrate->Associate(this);
      fButtonFrame->AddFrame(fIntegrate, fL1);
      width = TMath::Max(width, fIntegrate->GetDefaultWidth()); ++nb;
   }

   if (buttons & kGMIgnore) {
      fIgnore = new TGTextButton(fButtonFrame, new TGHotString("&Ignore"), kGMIgnore);
      fIgnore->Associate(this);
      fButtonFrame->AddFrame(fIgnore, fL1);
      width = TMath::Max(width, fIgnore->GetDefaultWidth()); ++nb;
   }

   if (buttons & kGMCancel) {
      fCancel = new TGTextButton(fButtonFrame, new TGHotString("&Cancel"), kGMCancel);
      fCancel->Associate(this);
      fButtonFrame->AddFrame(fCancel, fL1);
      width = TMath::Max(width, fCancel->GetDefaultWidth()); ++nb;
   }

   if (buttons & kGMClose) {
      fClose = new TGTextButton(fButtonFrame, new TGHotString("C&lose"), kGMClose);
      fClose->Associate(this);
      fButtonFrame->AddFrame(fClose, fL1);
      width = TMath::Max(width, fClose->GetDefaultWidth()); ++nb;
   }

   if (buttons & kGMYesAll) {
      fYesAll = new TGTextButton(fButtonFrame, new TGHotString("Y&es to All"), kGMYesAll);
      fYesAll->Associate(this);
      fButtonFrame->AddFrame(fYesAll, fL1);
      width = TMath::Max(width, fYesAll->GetDefaultWidth()); ++nb;
   }

   if (buttons & kGMNoAll) {
      fNoAll = new TGTextButton(fButtonFrame, new TGHotString("No &to All"), kGMNoAll);
      fNoAll->Associate(this);
      fButtonFrame->AddFrame(fNoAll, fL1);
      width = TMath::Max(width, fNoAll->GetDefaultWidth()); ++nb;
   }

   if (buttons & kGMNewer) {
      fNewer = new TGTextButton(fButtonFrame, new TGHotString("Ne&wer Only"), kGMNewer);
      fNewer->Associate(this);
      fButtonFrame->AddFrame(fNewer, fL1);
      width = TMath::Max(width, fNewer->GetDefaultWidth()); ++nb;
   }

   if (buttons & kGMAppend) {
      fAppend = new TGTextButton(fButtonFrame, new TGHotString("A&ppend"), kGMAppend);
      fAppend->Associate(this);
      fButtonFrame->AddFrame(fAppend, fL1);
      width = TMath::Max(width, fAppend->GetDefaultWidth()); ++nb;
   }

   if (buttons & kGMDismiss) {
      fDismiss = new TGTextButton(fButtonFrame, new TGHotString("&Dismiss"), kGMDismiss);
      fDismiss->Associate(this);
      fButtonFrame->AddFrame(fDismiss, fL1);
      width = TMath::Max(width, fDismiss->GetDefaultWidth()); ++nb;
   }

   // place buttons at the bottom

   fL2 = new TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);
   AddFrame(fButtonFrame, fL2);

   // keep the buttons centered and with the same width

   fButtonFrame->Resize((width + 20) * nb, GetDefaultHeight());

   fIconFrame = new TGHorizontalFrame(this, 60, 20);

   fL3 = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 2, 2, 2, 2);

   if (icon) {
      fIcon = new TGIcon(fIconFrame, icon, icon->GetWidth(), icon->GetHeight());
      fIconFrame->AddFrame(fIcon, fL3);
   }

   fLabelFrame = new TGVerticalFrame(fIconFrame, 60, 20);

   fL4 = new TGLayoutHints(kLHintsCenterY | kLHintsLeft | kLHintsExpandX,
                           4, 2, 2, 2);
   fL5 = new TGLayoutHints(kLHintsTop | kLHintsExpandX, 10, 10, 7, 2);

   // make one label per line of the message
   TGLabel *label;

   char *line;
   char *tmpMsg, *nextLine;

   tmpMsg = new char[strlen(msg) + 1];
   nextLine = tmpMsg;

   line = tmpMsg;
   strlcpy(nextLine, msg, strlen(msg) + 1);
   while ((nextLine = strchr(line, '\n'))) {
      *nextLine = 0;
      label = new TGLabel(fLabelFrame, line);
      label->SetTextJustify(text_align);
      fMsgList->Add(label);
      fLabelFrame->AddFrame(label, fL4);
      line = nextLine + 1;
   }

   label = new TGLabel(fLabelFrame, line);
   label->SetTextJustify(text_align);
   fMsgList->Add(label);
   fLabelFrame->AddFrame(label, fL4);
   delete [] tmpMsg;

   fIconFrame->AddFrame(fLabelFrame, fL4);
   AddFrame(fIconFrame, fL5);

   MapSubwindows();

   width  = GetDefaultWidth();
   height = GetDefaultHeight();

   Resize(width, height);

   // position relative to the parent's window

   CenterOnParent();

   // make the message box non-resizable

   SetWMSize(width, height);
   SetWMSizeHints(width, height, width, height, 0, 0);

   // set names

   SetWindowName(title);
   SetIconName(title);
   SetClassHints("MsgBox", "MsgBox");

   SetMWMHints(kMWMDecorAll | kMWMDecorResizeH  | kMWMDecorMaximize |
                              kMWMDecorMinimize | kMWMDecorMenu,
               kMWMFuncAll  | kMWMFuncResize    | kMWMFuncMaximize |
                              kMWMFuncMinimize,
               kMWMInputModeless);

   MapRaised();
   fClient->WaitFor(this);
}

//______________________________________________________________________________
GMMsgBox::~GMMsgBox()
{
   // Destroy message dialog box.

   if (IsZombie()) return;
   if (fPlot)     delete fPlot;
   if (fOverlay)      delete fOverlay;
   if (fDelete)      delete fDelete;
   if (fRename)   delete fRename;
   if (fIntegrate)   delete fIntegrate;
   if (fIgnore)  delete fIgnore;
   if (fCancel)  delete fCancel;
   if (fClose)   delete fClose;
   if (fDismiss) delete fDismiss;
   if (fYesAll)  delete fYesAll;
   if (fNoAll)   delete fNoAll;
   if (fNewer)   delete fNewer;
   if (fAppend)  delete fAppend;

   if (fIcon) delete fIcon;
   delete fButtonFrame;
   delete fIconFrame;
   delete fLabelFrame;
   fMsgList->Delete();
   delete fMsgList;
   delete fL1; delete fL2; delete fL3; delete fL4; delete fL5;
}

//______________________________________________________________________________
void GMMsgBox::CloseWindow()
{
   // Close dialog box. Before deleting itself it sets the return code
   // to kGMClose.

   if (fRetCode) *fRetCode = kGMClose;
   DeleteWindow();
}

//______________________________________________________________________________
Bool_t GMMsgBox::ProcessMessage(Long_t msg, Long_t parm1, Long_t)
{
   // Process message dialog box event.

   switch (GET_MSG(msg)) {
      case kC_COMMAND:
         switch (GET_SUBMSG(msg)) {
            case kCM_BUTTON:
               if (fRetCode) *fRetCode = (Int_t) parm1;
               DeleteWindow();
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
