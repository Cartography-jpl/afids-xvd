///////////////////////////////////////////////////////////////////
//  DspCursBox.h:
///////////////////////////////////////////////////////////////////
#ifndef DSPCURSBOX_H
#define DSPCURSBOX_H
#include "UIComponent.h"

class CursorPositionView;
class DspCursorView;
class CursorModel;
class ImageData;
class BasicImageView;

class DspCursBox : public UIComponent {

 
   protected:
   
 
	DspCursorView * 	_cursorRangeDnView;
	CursorPositionView *	_cursorPositionView;
        CursorModel *        	_cursorModel;         // model
 	static XtResource 	_resources[];  	
	static String 		_defaults[]; 		   // resource defaults.
        int			_enableLoc;
        

   public:

	DspCursBox( Widget, char *,  ImageData *, BasicImageView *, int  );
	~DspCursBox();

        virtual const char * const className() { return ("DspCursBox"); }

};
#endif
