//////////////////////////////////////////////////////////////
// ZoomCmdInterface.h
///////////////////////////////////////////////////////////////
#ifndef ZOOMCMDINTERFACE
#define ZOOMCMDINTERFACE
#include "CmdInterface.h"
#include <Xm/Xm.h>

class KeyinView;
class BasicImageView;

class ZoomCmdInterface : public CmdInterface {
 private:
   static void newZoomCallback(Widget, XtPointer, XtPointer);

 protected:

   KeyinView *_zoomXIn;
   KeyinView *_zoomXOut;
   KeyinView *_zoomYIn;
   KeyinView *_zoomYOut;

   virtual KeyinView *addOneSubView(Widget parent, char *name);
   virtual void createAllSubViews(Widget parent, BasicImageView *imageView);
   virtual void executeCmd(XtPointer=0);
   virtual void setValue(CmdValue);
   virtual void newZoom(Widget);

 public:

   ZoomCmdInterface(Widget, Cmd *, BasicImageView * = 0);
   ZoomCmdInterface(Widget, char *, BasicImageView * = 0);
   ~ZoomCmdInterface();
};
#endif

