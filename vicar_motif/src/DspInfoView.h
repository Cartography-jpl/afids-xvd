///////////////////////////////////////////////////////////////////
//  DspInfoView.h:
//
//      This Class opens the ImageModel & ImageDisplayView
//      to read a vicar image from file and display it in the view.  
///////////////////////////////////////////////////////////////////
#ifndef DSPINFOVIEW_H
#define DSPINFOVIEW_H
#include "DspUIComponent.h"
#include "string.h"
#include <Xm/Text.h>

class DspInfoView : public DspUIComponent {
 
protected:
 
        static String 	_defaults[];
        char *		_blankString;
        char *		_stringBuffer;
        int		_numChars;
        char 		_text[512];
        void 		(*_fp)(char*);    

public:
 
      	DspInfoView( Widget, char *, void (*fp)(char*) );
        ~DspInfoView();
        virtual void 	update( );
        virtual char *	getStringBuffer(); 
        inline int	getMaxStringLength() {return _numChars;}
        virtual const char * const className() { return ("DspInfoView"); } 

};
#endif

