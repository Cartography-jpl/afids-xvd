///////////////////////////////////////////////////////////
// DspCursDspCmd.cc: Command class that displays a window 
//		    with (x,y) cursor current position 
//		    within the image area
//////////////////////////////////////////////////////////
#include "DspCursCmd.h"
#include "DspCursWindow.h"
#include "ImageData.h"
#include "ImageDisplayView.h"

///////////////////////////////////////////////////////////
//   CONSTRUCTOR
//  	Save the cursor model pointer and set value to create
//  	the display component once.
///////////////////////////////////////////////////////////
DspCursCmd::DspCursCmd ( char *name, char* titleName, int active,
		ImageData *imageData, ImageDisplayView *imageView,
		Widget parent, int pixelSize ) 
		: NoUndoCmd ( name, active )
{
    _created = FALSE;
    _imageData = imageData;
    _imageView = imageView;
    _title = titleName;
    _parent = parent;
        
//  CREATE WINDOW (See Note #1)
    if (active) {
    	_cursorRangeDnWindow = new DspCursWindow ( _title, _imageData, _imageView, _parent, pixelSize);
    	_cursorRangeDnWindow->initialize();
///    	XtVaSetValues(_cursorRangeDnWindow->baseWidget(),
///                		XmNdeleteResponse, XmUNMAP,
///               		NULL);
        _created = TRUE;
     }
         
}

///////////////////////////////////////////////////////////
//   doit()
// 	Execute the following upon button activation.
// 	Create display Window only once and then display it.
// 	Set the Close button to the UNMAP state so the Window
// 	is only unmanaged when it is closed and can therefore
// 	be managed again when the user hits the command button.
///////////////////////////////////////////////////////////
void DspCursCmd::doit()
{
  	
//	MANAGE WINDOW
	if (_created != FALSE ) 
  		_cursorRangeDnWindow->manage();

}

void DspCursCmd::mapViews()   { if (_created) _cursorRangeDnWindow->restoreState();}
void DspCursCmd::unmapViews() { if (_created) _cursorRangeDnWindow->saveState();}

// NOTE #1
//	Had to move the instantiation of DspCursWindow out of doit() 
//	to constructor. however, this means that model->readPixel
//	is being read even if it is desensitized.  In the case of 
//	PWSdata, pixels were not meant to be read.  SO..
//	I only instatiate if sensitized.  
