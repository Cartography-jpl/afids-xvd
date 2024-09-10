///////////////////////////////////////////////////////////////////
//  DspInstrNimEng.h
///////////////////////////////////////////////////////////////////
#ifndef DSPINSTRNIMSENG_H
#define DSPINSTRNIMSENG_H
#include "DspInstrBasic.h"
#include "DspHistMean.h"
#include "DspHistStdDev.h"
#include "DspDataNimBasicGll.h"
#include "DspDataNimEngRB.h"
#include "ImageDisplayView.h"

enum EngModelType {RB, BW};

class DspInstrNimEng : public DspInstrBasic {

   protected:

	ImageData *             _dataModelBW;
	//ImageDisplayView *	_imageView;
	ImageDisplayView *	_imageViewNimsBW;
	
	virtual void		createHistDisplay( Widget );
	virtual void		createImageDisplay( Widget );
	virtual void		createImageModel();
	virtual void		createPopup();
	virtual void 		updateImageZoom() {}
	virtual void		createTextDisplay();
   	
  public:

	DspInstrNimEng(Widget parent, char * name, DspWindowBasic * window);
	virtual const char *const className() { return "DspInstrNimEng"; }
};
#endif
