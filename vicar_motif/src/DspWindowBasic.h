////////////////////////////////////////////////////////////////
// DspWindowBasic.h
////////////////////////////////////////////////////////////////
#ifndef DSPBASICWINDOW_H
#define DSPBASICWINDOW_H
#include "MainWindow.h"
#include "DisplayInstrumentDefs.h"
#include "DspDefs.h"
#include "DspInstrBasic.h"
//#include "DspInstrSsi.h"
//#include "DspInstrPws.h"
//#include "DspInstrNimSci.h"
//#include "DspInstrNimEng.h"
//#include "DspInstrMpfRvRt.h"
//#include "DspInstrMpfRvRr.h"
//#include "DspInstrMpfApxs.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <Xm/Frame.h>
#include <Xm/Xm.h>


class DspWindowBasic : public MainWindow {

protected:

  	Widget 			_form;

	static Boolean		_firstDisplay; // first time thru only
	static XtResource 	_resources[];
	DspInstrBasic 	** 	_instrumentDisplayer; // view within this windows work area
	char 			_filename[MAX_NAMESTRINGLENGTH];//edr or udr name
	InstrumentType		_instrument;  // s/c instrument or display type
	InstrumentType		_prevInstrument;  // prev display
	int			_readFrequency; 
	XtIntervalId		_id;
	char			_displayName[MAX_NAMESTRINGLENGTH]; //node name
	int			_newDataArrived;  // new 
	Boolean			_windowBorder;
	Cursor			_blankCursor;
	
	static  void 		timeoutCallback( XtPointer, XtIntervalId * );
	Widget 			createWorkArea( Widget parent );
	void 			createInstrumentDisplayInWorkArea();
	virtual void		setShellBorder();
	virtual void		updateAllDisplays();
	
	virtual void            setup()=0;
	virtual int		getNextFrame()=0;
	
public:

	DspWindowBasic( char * name) : MainWindow (name) {}
	~DspWindowBasic();
	
	virtual void 		initInterface(int , char **) = 0; 	  // called by dspapplication
	inline char *		getDisplayName() { return _displayName; } // called by dspapplication
	inline char *		getFilename(){return _filename;} 	  // called by dspinstrument
	
	virtual const char *const className() { return "DspWindowBasic";} // used by all windows
	
};
#endif
