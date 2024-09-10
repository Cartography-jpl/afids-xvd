////////////////////////////////////////////////////////////////////
// DspCursWindow.cc:
//	Would have been better to use dialog instead of window here
//	but its too late to change it now.  MainWindow uses TopLevelShell
//	which doesn't manage/unmanage automatically when parent is 
//	managed and unmanaged. 
////////////////////////////////////////////////////////////////////
#include "DspCursWindow.h"
#include "ImageData.h"
#include "ImageDisplayView.h"
#include "DspCursBox.h"
#include <Xm/Form.h>
#include <Xm/MainW.h>
#include <zvproto.h>


DspCursWindow::DspCursWindow ( char *name, ImageData * imageData, 
		ImageDisplayView * imageView,
		Widget parent, int pixelSize) : MainWindow ( name )
{
    _imageData = imageData;
    _imageView = imageView;
    _parent = parent;
    _pixelSize = pixelSize;

}

Widget DspCursWindow::createWorkArea ( Widget parent )
{
   _form = XtVaCreateWidget("workArea",
                     xmFormWidgetClass, parent,
                     NULL);

   _cursorRangeDnBox = new DspCursBox(_form, "dspCursBox", 
   							_imageData, 
   							_imageView,
   							_pixelSize);

   XtVaSetValues   ( _cursorRangeDnBox->baseWidget(),
                     XmNtopAttachment,     XmATTACH_FORM,
                     XmNleftAttachment,    XmATTACH_FORM,
                     XmNrightAttachment,   XmATTACH_FORM,
                     XmNbottomAttachment,  XmATTACH_FORM,
                     NULL );
   _cursorRangeDnBox->manage();

   return (_form);
}
void DspCursWindow::initialize()
{

// 	All toplevel windows in the MotifApp framework are 
// 	implemented as a popup shell off the Application's
// 	base widget.
    	_w = XtVaCreatePopupShell ( _name, 
                             	topLevelShellWidgetClass,
                             	//theApplication->baseWidget(),
                             	_parent,
                             	XmNallowShellResize, True,
                             	XmNdeleteResponse, XmUNMAP,
                             	NULL );
                             	
        _pop = False;
                    	
                             	
    	installDestroyHandler();
    
// 	Use a Motif XmMainWindow widget to handle window layout
    	_main = XtCreateManagedWidget ( "mainWindow", 
                                   xmMainWindowWidgetClass,
                                   _w, 
                                   NULL, 0 );
    
// 	Called derived class to create the work area
    	_workArea = createWorkArea ( _main );  
    	assert ( _workArea );
    
// 	Designate the _workArea widget as the XmMainWindow
// 	widget's XmNworkWindow widget
    
    	XtVaSetValues ( _main, 
                   XmNworkWindow, _workArea, 
                   NULL );
    
// 	Manage the work area if the derived class hasn't already.   
    	if ( !XtIsManaged ( _workArea ) )
        	XtManageChild ( _workArea ); 
                    
}

void DspCursWindow::restoreState()
{
//char msg[132];
	if (_pop) MainWindow::manage();
}
void DspCursWindow::saveState()  // this doesn't work.    
{
//char msg[132];
	if ( XtIsManaged(_w)) _pop = True;
	else _pop = False;
	MainWindow::unmanage();
}
	
