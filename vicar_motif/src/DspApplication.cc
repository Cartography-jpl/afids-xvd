////////////////////////////////////////////////////////////
// DspApplication.cc: 
////////////////////////////////////////////////////////////
#include "DspApplication.h"
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <zvproto.h>
#if XmVERSION != 1 || XmREVISION > 1
#include <Xm/RepType.h>
#endif

extern	char *rts_log_prgm;


#if (XlibSpecificationRelease>=5)
void DspApplication::initialize_X ( int *argcp, char **argv )
#else
void DspApplication::initialize_X ( unsigned int *argcp, char **argv )
#endif
{
//	GET ARG
	int * argc = (int *) argcp;
	
//	CAST TO DSPBASICWINDOW
	DspWindowBasic ** dspWindows = (DspWindowBasic **) _windows;
	
//	CHECK TO MAKE SURE THE MAINWINDOW IS OF TYPE DSPBASICWINDOW
	assert (strcmp( dspWindows[0]->className(), "DspWindowBasic") == 0 ) ;

//	INIT TOP LEVEL ( _w = toplevel ) (See Note #1, at bottom)
	dspWindows[0]->initInterface(*argc, argv); // initIF before we can get name
	char * displayName = dspWindows[0]->getDisplayName(); // (See Note #2)
	XtToolkitInitialize();
	_appContext = XtCreateApplicationContext();
	
//	OPEN DISPLAY FOR displayName
	_display     = 0;
	_display     = XtOpenDisplay(_appContext, displayName,
				_name,_applicationClass, NULL,0, argc, argv);
//	IF UNABLE TO OPEN displayName, DEFAULT TO LOCAL DISPLAY	(NULL)			
	if (_display == 0 ) {
		if ( strcmp(displayName,"") != 0) // Not an error for browse
		  std::cout << "Unable to display to screen named: "<< displayName << "."<< std::endl;
		std::cout << "Will display to local screen. " << std::endl;
		_display = XtOpenDisplay(_appContext,NULL,
				_name,_applicationClass, NULL,0, argc, argv);
	}

	_w = XtAppCreateShell( _name,_applicationClass, 
				applicationShellWidgetClass,_display, NULL, 0);
}
//	NOTE #1:
//		This piece of code replaces:
//    		_w = XtAppInitialize ( &_appContext, 
//			_applicationClass, NULL, 0, 
//		        argc, argv, NULL, NULL, 0 );
//	NOTE #2:
//		The above code constrains This application to have only
//		one screen.  

