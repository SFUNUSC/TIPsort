// @(#)root/gui:$Id: GMTextEntry.h 23115 2008-04-10 13:35:37Z rdm $
// Author: Fons Rademakers   09/01/98

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_GMTextEntry
#define ROOT_GMTextEntry


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMsgBox                                                              //
//                                                                      //
// A message dialog box.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TGTextEntry.h"
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



//--- Button types and return codes


class TGLabel;


class GMTextEntry : public TGTransientFrame {

protected:
  TGTextEntry            *fPlot;
   TGVerticalFrame   *fButtonFrame;                // frame containing buttons
   TGHorizontalFrame   *fIconFrame;                  // frame containing icon and text
   TGVerticalFrame     *fLabelFrame;                 // frame containing text
   TGLayoutHints       *fL1, *fL2, *fL3, *fL4, *fL5; // layout hints
   TList               *fMsgList;                    // text (list of TGLabels)
   char                *fRetCode;                    // address to store return code

   void PMsgBox(const char *title, const char *msg,
                 char *name, Int_t text_align);

private:
   GMTextEntry(const GMTextEntry&);              // not implemented
   GMTextEntry& operator=(const GMTextEntry&);   // not implemented

public:
   GMTextEntry(const TGWindow *p = 0, const TGWindow *main = 0,
            const char *title = 0, const char *msg = 0,
            char* name=0,
            UInt_t options = kVerticalFrame,
            Int_t text_align = kTextCenterX | kTextCenterY);
 
   virtual ~GMTextEntry();

   virtual void CloseWindow();
   virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

};

#endif
