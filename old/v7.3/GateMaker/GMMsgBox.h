// @(#)root/gui:$Id: GMMsgBox.h 23115 2008-04-10 13:35:37Z rdm $
// Author: Fons Rademakers   09/01/98

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_GMMsgBox
#define ROOT_GMMsgBox


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMsgBox                                                              //
//                                                                      //
// A message dialog box.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGString
#include "TGString.h"
#endif
#ifndef ROOT_TGPicture
#include "TGPicture.h"
#endif
#ifndef ROOT_TGWidget
#include "TGWidget.h"
#endif


//--- Icon types

enum GMEMsgBoxIcon {
   kGMIconStop,
   kGMIconQuestion,
   kGMIconExclamation,
   kGMIconAsterisk
};

//--- Button types and return codes

enum GMEMsgBoxButton {
   kGMPlot    = BIT(0),
   kGMOverlay = BIT(1),
   kGMDelete  = BIT(2),
   kGMRename  = BIT(3),
   kGMIntegrate = BIT(4),
   kGMIgnore  = BIT(5),
   kGMCancel  = BIT(6),
   kGMClose   = BIT(7),
   kGMDismiss = BIT(8),
   kGMYesAll  = BIT(9),
   kGMNoAll   = BIT(10),
   kGMAppend  = BIT(11),
   kGMNewer   = BIT(12)
};


class TGButton;
class TGIcon;
class TGLabel;


class GMMsgBox : public TGTransientFrame {

protected:
   TGButton            *fPlot, *fOverlay, *fDelete, *fRename;   // buttons in dialog box
   TGButton            *fIntegrate, *fIgnore, *fCancel;  // buttons in dialog box
   TGButton            *fClose, *fYesAll, *fNoAll;   // buttons in dialog box
   TGButton            *fNewer, *fAppend, *fDismiss; // buttons in dialog box
   TGIcon              *fIcon;                       // icon
   TGVerticalFrame   *fButtonFrame;                // frame containing buttons
   TGHorizontalFrame   *fIconFrame;                  // frame containing icon and text
   TGVerticalFrame     *fLabelFrame;                 // frame containing text
   TGLayoutHints       *fL1, *fL2, *fL3, *fL4, *fL5; // layout hints
   TList               *fMsgList;                    // text (list of TGLabels)
   Int_t               *fRetCode;                    // address to store return code

   void PMsgBox(const char *title, const char *msg, const TGPicture *icon,
                Int_t buttons, Int_t *ret_code, Int_t text_align);

private:
   GMMsgBox(const GMMsgBox&);              // not implemented
   GMMsgBox& operator=(const GMMsgBox&);   // not implemented

public:
   GMMsgBox(const TGWindow *p = 0, const TGWindow *main = 0,
            const char *title = 0, const char *msg = 0, const TGPicture *icon = 0,
            Int_t buttons = kGMDismiss, Int_t *ret_code = 0,
            UInt_t options = kVerticalFrame,
            Int_t text_align = kTextCenterX | kTextCenterY);
   GMMsgBox(const TGWindow *p, const TGWindow *main,
            const char *title, const char *msg, GMEMsgBoxIcon icon,
            Int_t buttons = kGMDismiss, Int_t *ret_code = 0,
            UInt_t options = kVerticalFrame,
            Int_t text_align = kTextCenterX | kTextCenterY);
   virtual ~GMMsgBox();

   virtual void CloseWindow();
   virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

};

#endif
