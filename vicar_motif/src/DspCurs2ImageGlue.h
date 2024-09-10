////////////////////////////////////////////////////////////////////////
// DspCurs2ImageGlue.h: See DspImageToCursorDnViewGlue
////////////////////////////////////////////////////////////////////////
#ifndef DSPCURSORIMAGE2GLUE_H
#define DSPCURSORIMAGE2GLUE_H
#include "BasicImageView.h"
#include "ImageData.h"
#include "DspCursorView.h"

class DspCursorView;

class DspCurs2ImageGlue : public BasicImageView {

 protected:
 
   		ModeType 	 	_oldMode;
   		ImageData *	 	_image;
   		DspCursorView *	_cursorView;

 public:

   DspCurs2ImageGlue (ImageData *image, DspCursorView *cursorView);
   ~DspCurs2ImageGlue( ){ /* no way to detachView on image, yet*/ }
   virtual void update();
   virtual const char *const className() { return  "DspImage2CursGlue"; }
   
};
#endif

