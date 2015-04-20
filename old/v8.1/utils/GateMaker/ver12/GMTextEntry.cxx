

#include "GMTextEntry.h"
#include "TGLabel.h"
#include "TList.h"



//______________________________________________________________________________
GMTextEntry::GMTextEntry(const TGWindow *p, const TGWindow *main,
                   const char *title, const char *msg, 
                   char *name, UInt_t options,
                   Int_t text_align) :
   TGTransientFrame(p, main, 10, 10, options)
{
   // Create a message dialog box.

   if (p)
      PMsgBox(title, msg, name, text_align);
   else
      MakeZombie();
}


//______________________________________________________________________________
void GMTextEntry::PMsgBox(const char *title, const char *msg,
                       char* name,
                       Int_t text_align)
{
   // Protected, common message dialog box initialization.

   UInt_t nb, width, height;

   fPlot =  0;
   fMsgList   = new TList;
   fRetCode   = name;
   nb = width = 0;

   // create the buttons

   fButtonFrame = new TGVerticalFrame(this, 60, 20);
   fL1 = new TGLayoutHints(kLHintsCenterY | kLHintsExpandX, 3, 3, 0, 0);

  
   fPlot = new TGTextEntry(fButtonFrame, name);
   fPlot->Associate(this);
   fButtonFrame->AddFrame(fPlot, fL1);
   width = TMath::Max(width, fPlot->GetDefaultWidth()); ++nb;
   

   
   // place buttons at the bottom

   fL2 = new TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);
   AddFrame(fButtonFrame, fL2);

   // keep the buttons centered and with the same width

   fButtonFrame->Resize((width + 20) * nb, GetDefaultHeight());

   fIconFrame = new TGHorizontalFrame(this, 60, 20);

   fL3 = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 2, 2, 2, 2);


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
GMTextEntry::~GMTextEntry()
{
   // Destroy message dialog box.

   if (IsZombie()) return;
   if (fPlot)     delete fPlot;
  

   delete fButtonFrame;
   delete fIconFrame;
   delete fLabelFrame;
   fMsgList->Delete();
   delete fMsgList;
   delete fL1; delete fL2; delete fL3; delete fL4; delete fL5;
}

//______________________________________________________________________________
void GMTextEntry::CloseWindow()
{
   // Close dialog box. Before deleting itself it sets the return code
   // to 0.

   if (fRetCode) *fRetCode = 0;
   DeleteWindow();
}

//______________________________________________________________________________
Bool_t GMTextEntry::ProcessMessage(Long_t msg, Long_t parm1, Long_t)
{
   // Process message dialog box event.

   switch (GET_MSG(msg)) {
      case kC_TEXTENTRY:
         switch (GET_SUBMSG(msg)) {
            case kTE_ENTER:
	      memcpy(fRetCode,fPlot->GetText(),strlen(fPlot->GetText())*sizeof(char));
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
