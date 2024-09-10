/////////////////////////////////////////////////////////////
// DspCursCmd.h: Include file to handle the CursorPos command
//                button from the Browse app.
/////////////////////////////////////////////////////////////
#ifndef DSPCURSCMD_H
#define DSPCURSCMD_H
#include "NoUndoCmd.h"
#include "DspCursWindow.h"
#include <Xm/Xm.h>

class ImageData;
class ImageDisplayView;

class DspCursCmd : public NoUndoCmd {

  private:

     int              	_created;
     Widget		_parent;
     DspCursWindow *    _cursorRangeDnWindow;
     ImageData *       	_imageData;
     ImageDisplayView *	_imageView;
     char *            	_title;
     
  protected:
  
   virtual void doit();

  public:

    DspCursCmd ( char*, char*, int, ImageData*, ImageDisplayView *,Widget, int pixelSize );
    virtual void mapViews() ;
    virtual void unmapViews();
    virtual const char *const className () { return "DspCursCmd"; }
};
#endif

