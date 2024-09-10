///////////////////////////////////////////////////////////////////
//  DspCursBox.cc:
///////////////////////////////////////////////////////////////////
#include "DspCursBox.h"
#include "BasicImageView.h"
#include "ImageData.h"
#include "DspCursorView.h"
#include "CursorPositionView.h"
#include "CursorModel.h"
#include "BasicImageView.h"
#include <Xm/RowColumn.h>

String DspCursBox::_defaults [ ] = {
        "*isAligned:            True",
        "*orientation:          XmHORIZONTAL",
        "*entryAlignment:       XmALIGNMENT_END",
        "*packing:              XmPACK_COLUMN",
        ".numColumns:		1",
        "*adjustLast:           False",
        NULL,
};
XtResource DspCursBox::_resources [ ] = {
  {
    "enableLoc",
    "enableLoc",
    XmRInt,
    sizeof(int),
    XtOffset( DspCursBox *, _enableLoc),
    XmRString,
    (XtPointer) "1", // on
 }
};
DspCursBox::DspCursBox( Widget parent, char *name,
		ImageData *imageData, BasicImageView *imageView, int pixelSize )
		: UIComponent(name)
{
unsigned char bitFlags;

// 	INIT VARIABLES
 	setDefaultResources ( parent, _defaults );
	_w = XtVaCreateWidget ( _name,  
		xmRowColumnWidgetClass, parent, 
		NULL);
        getResources(_resources, XtNumber( _resources) );        
 	installDestroyHandler();
	
	
//	CREATE CURSOR MODEL
        _cursorModel = new CursorModel(  True,  imageView->getWidget() );


//	DISPLAY LINE/SAMPLE LOCATION  (POSITION) 
	bitFlags = (unsigned char) 255; 		// see Note
	if (_enableLoc == 1) {  // if enabled through resources
		_cursorPositionView = new CursorPositionView( _w, "cursorPositionView", 
					_cursorModel, imageData, bitFlags );
		_cursorPositionView->manage();				
	}


//	DISPLAY PIXEL VALUE UNDER CURSOR
	bitFlags = (unsigned char) 255; 		
	_cursorRangeDnView = new DspCursorView( _w, "dspCursorView", 
					_cursorModel,imageData,bitFlags,pixelSize );
	_cursorRangeDnView->manage();
	
}

DspCursBox::~DspCursBox()
{
        delete _cursorModel;
        delete _cursorRangeDnView;
}
