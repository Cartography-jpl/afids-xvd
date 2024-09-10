////////////////////////////////////////////////////////////////
// 	DspWindowBasic.cc: The base class for realtime and browse
//		      top-level window
//		NOTE:  The design should be to delete a display so
//		we can move to a different device.  But GUI code
//		use to crash - probably still does - when this
//		display code tries to delete. Now this code 
//		may not be able to delete it's own objects.
////////////////////////////////////////////////////////////////
#include "DspWindowBasic.h"

Boolean DspWindowBasic::_firstDisplay = TRUE;


XtResource DspWindowBasic::_resources [ ] = {
  {
    "readFrequency",
    "ReadFrequency",
    XmRInt,
    sizeof(int),
    XtOffset( DspWindowBasic *, _readFrequency),
    XmRString,
    (XtPointer) "1500",
 },
// {
//    "windowBorder",
//    "windowBorder",
//    XmRBoolean,
//    sizeof(Boolean),
//    XtOffset( DspWindowBasic *, _windowBorder),
//    XmRString,
//    (XtPointer) "on",
//  }
};

DspWindowBasic::~DspWindowBasic()
{
//	DELETE WORKAREA & FORM
	delete []_instrumentDisplayer;
	XtDestroyWidget( _form );

//	REMOVE TIMEOUT
	if ( _id ) XtRemoveTimeOut ( _id );
}
/////////////////////////////////////////////////////////
// 	createWorkArea
/////////////////////////////////////////////////////////
Widget DspWindowBasic::createWorkArea( Widget parent)
{
	_prevInstrument = dsp_invalid;	
	
// 	INIT VARIABLES
	_blankCursor = (Cursor) NULL;
        getResources(_resources, XtNumber( _resources) );
        int n = (int) dsp_invalid;
        _instrumentDisplayer = new DspInstrBasic* [n];
        for (int i = 0; i<n; i++ )
        	_instrumentDisplayer[i] = 0;

//	CREATE FRAME TO HOLD ALL WIDGETS
	_form = XtVaCreateManagedWidget( _name, 
			xmFrameWidgetClass, parent, 
			XmNwidth, 640,
			XmNheight, 480,
			XmNshadowThickness, 0,
			NULL);

// 	DISABLE AUTO RESIZE
	Widget topLevel = (Widget) _form;
	while ((!XtIsShell(topLevel)) && (XtParent(topLevel) != 0))
                topLevel = XtParent(topLevel);
	XtVaSetValues(topLevel, XtNallowShellResize, FALSE, NULL);
	
//      REMOVE BORDER ON SHELL, IF RESOURCE SET
        setShellBorder();
 
// 	SET UP TO CALL CREATEINSTRUMENTDISPLAYINWORKAREA (IN SUBCLASS)
	setup();

//	RETURN
   	return (_form);	
}

void DspWindowBasic::createInstrumentDisplayInWorkArea()
{
}

/****
/////////////////////////////////////////////////////////
// 	createInstrumentDisplayInWorkArea
// 		CALL BY SUBCLASS WHEN DATA ARRIVES
/////////////////////////////////////////////////////////
void DspWindowBasic::createInstrumentDisplayInWorkArea()
{

// 	CREATE IMAGE WINDOW (IF NOT ALREADY CREATED)
	int i         = (int) _instrument;
	
	if (_instrumentDisplayer[i] == NULL) {
	    if (((int)_instrument) == gll_dsp_ssi)	
		_instrumentDisplayer[i] = new DspInstrSsi( _form, "ssiDisplayer", 
								(DspWindowBasic *) this );
	    if (((int)_instrument) == gll_dsp_pws)
		_instrumentDisplayer[i] = new DspInstrPws( _form, "pwsDisplayer", 
								(DspWindowBasic *) this );
	    //if (((int)_instrument) == gll_dsp_nims_eng)	
	    //	_instrumentDisplayer[i] = new DspInstrNimEng( _form, "nimsDisplayerEng", 
	    //							(DspWindowBasic *) this ); //uses blank filename	
	    if (((int)_instrument) == gll_dsp_nims_sci)	
		_instrumentDisplayer[i] = new DspInstrNimSci( _form, "nimsDisplayerSci", 
								(DspWindowBasic *) this ); // uses blank filename
	    if (((int)_instrument) ==  mpf_dsp_rover_front) {
	    	_instrumentDisplayer[i] = new DspInstrMpfRvRt( _form, "mpfRvRtDisplayerSci", 
	    						(DspWindowBasic *) this );
	    }
	    if (((int)_instrument) ==  mpf_dsp_rover_rear) {
	    	_instrumentDisplayer[i] = new DspInstrMpfRvRr( _form, "mpfRvRrDisplayerSci",
	    							(DspWindowBasic *) this ); 
	    }
	    if (((int)_instrument) ==  mpf_dsp_apxs) {
	    	_instrumentDisplayer[i] = new DspInstrMpfApxs( _form, "mpfApxsDisplayerSci", 
	    							(DspWindowBasic *) this );
	    }
	}
		
// 	UNMANAGE PREVIOUS DISPLAY
	int max_instr = (int) dsp_invalid;
	for ( int n = 0; n < max_instr; n++ )
	       if ( (_instrumentDisplayer[n] != NULL) && (_instrumentDisplayer[n]->isManaged()) 
	             && (_prevInstrument != _instrument )) 
			_instrumentDisplayer[n]->unmanage();

//	MANAGE THE NEW DISPLAY
	if ((_instrumentDisplayer[i] != NULL) && (_instrument != dsp_invalid )) {
	     _instrumentDisplayer[i]->manage(); 
	     _prevInstrument = _instrument; 
	     _instrumentDisplayer[i]->update();
	}
}
****/
/////////////////////////////////////////////////////////
// 	setShellBorder (non-public) .. called from createWorkArea
//	    old way:
// 		removes motif border if resource set to "off".
//	    new way:
//		gets the info fromm gll_dsp_cntrl
// 	    Most likely to be used in r/t on small screens.
//	    
/////////////////////////////////////////////////////////
void DspWindowBasic::setShellBorder()
{
        Widget shell = _w;

//	FIND SHELL WIDGET        
          if (shell && !XtIsShell(shell)) {
                do {
                        shell = XtParent(shell);
                } while (shell && !XtIsShell(shell));
          } 
 
        
//     	GET SCREEN HEIGHT AND WIDTH 
	  int screenHt = XDisplayHeight(XtDisplay(shell),
                                       XDefaultScreen(XtDisplay(shell)));                               
          int screenWth = XDisplayWidth(XtDisplay(shell),
                                            XDefaultScreen(XtDisplay(shell)));
        
                
//	IF RESOURCE SET BORDER TO OFF      
//        if (_windowBorder == FALSE ) {
//??	  if (getBorderMode()) {
	  if ((screenHt < 550) || (screenWth < 650)) { 
        
          
//	   REMOVE BORDER          
           if (shell != 0)
                XtVaSetValues(shell, XmNmwmDecorations, 0, NULL);
        }
}
///////////////////////////////////////////////////////////////
//	updateAllDisplays:
//		updates display and sets a timeout to come back 
//		
///////////////////////////////////////////////////////////////
void DspWindowBasic::updateAllDisplays()
{
	
// 	BLANK CURSOR IN REST OF DISPLAY WINDOW (in case it moves)
	if (_blankCursor != (Cursor) NULL )
	    if (XtIsRealized(_form)) 	 
	   	XDefineCursor( XtDisplay(_form), XtWindow(_form), _blankCursor);

//	IF THIS IS FIRST TIME THROUGH, 
//	WE MAY HAVE INFO FROM CALL TO INITDISPLAY FROM DSPAPPLICATION
	if (_firstDisplay) {
		_firstDisplay = FALSE;
		if (_newDataArrived == 0) 
			_newDataArrived = getNextFrame();
		if (_newDataArrived != 0) {
			createInstrumentDisplayInWorkArea();
		}
	}

// 	IF THIS IS NOT THE FIRST TIME CALLED && 
// 	DATA HAS ARRIVED.... CREATE DISPLAY 
	else {
	 	_newDataArrived = getNextFrame();
		if ( _newDataArrived != 0 ) {
			createInstrumentDisplayInWorkArea();
		}
	}	

// 	SET APP TIMEOUT TO READ LATER
	_id  = XtAppAddTimeOut ( XtWidgetToApplicationContext ( _form ),
                        _readFrequency,
                         &DspWindowBasic::timeoutCallback,
                         (XtPointer) this );
}	
///////////////////////////////////////////////////////////////
//	TIMEOUT CALLBACK
///////////////////////////////////////////////////////////////
void DspWindowBasic::timeoutCallback(  XtPointer clientData, XtIntervalId * )
{
    	DspWindowBasic *obj = (DspWindowBasic *) clientData;
    	obj->updateAllDisplays();
}

 
