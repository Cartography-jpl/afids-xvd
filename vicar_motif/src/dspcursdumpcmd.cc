///////////////////////////////////////////////////////////
// DspCursDumpCmd.cc:  Dump cursor values in log
//////////////////////////////////////////////////////////
#include "DspCursDumpCmd.h"


///////////////////////////////////////////////////////////
//   CONSTRUCTOR
///////////////////////////////////////////////////////////
DspCursDumpCmd::DspCursDumpCmd ( Widget parent, 
		char * name, 
		char *		title,
		int 		active,
		ImageData *	imageData,  
		ImageDisplayView *imageView, 
		unsigned char  bitFlags,
		int pixelSize ) 
	: NoUndoCmd ( name, active )
{
    _created = FALSE;
    if (active) {
    	_cursDump = new DspCursDump ( parent, title , imageData, imageView, bitFlags,pixelSize );
	_created = TRUE;
	
    }
    _fileName = 0;
	
}

///////////////////////////////////////////////////////////
//   doit()
// 	Execute the following upon button activation:
//	     _cursDump->dump() to dump.
///////////////////////////////////////////////////////////
void DspCursDumpCmd::doit()
{
     if (_created == TRUE) {   
    	_cursDump->dump( _fileName );
     }
} 
///////////////////////////////////////////////////////////
//   setNameLink acts like a callback.  
//		currently called by DspFileNameCmd
///////////////////////////////////////////////////////////
void DspCursDumpCmd::setNameLink(XtPointer obj, char * fileName)
{   	
	DspCursDumpCmd * me = (DspCursDumpCmd *) obj;
	me->setName( fileName );
}
///////////////////////////////////////////////////////////
//   setName called by SetNameLink, above
///////////////////////////////////////////////////////////
void DspCursDumpCmd::setName(char * fileName)
{
	if (_fileName != 0)
		delete _fileName;
		
	if (fileName != 0) {
		_fileName = new char[(strlen(fileName)+1)];
		strcpy(_fileName, fileName);
	}
}
  	
