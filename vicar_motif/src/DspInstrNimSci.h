///////////////////////////////////////////////////////////////////
//  DspInstrNimSci.h
///////////////////////////////////////////////////////////////////
#ifndef DSPINSTRNIMSCI_H
#define DSPINSTRNIMSCI_H
#include "DspInstrBasic.h"
#include "DspHistMean.h"
#include "DspHistStdDev.h"
#include "DspDataNimBasicGll.h"
#include "ImageDisplayView.h"

enum SciModelType {primary=0,D2=1, Wh=2, Av=3} ;

//	EXTERNAL	
	void    setZoomParams2Off( int * zoomX, int *zoomY, int *panY, int * panX ,int*oddEvenFlag);	 
	int     setPseudoModeOff();
	int     setPseudoModeOn();
	void	fillImageStretchLutWithRaw(int * lut1);


class DspInstrNimSci : public DspInstrBasic {
   protected:

	ImageDisplayView *	_imageViewNimsAv;
	ImageDisplayView *	_imageViewNimsWh;	
	ImageDisplayView *	_imageViewNims2D;	
	//ImageDisplayView *	_imageView;	
		
	virtual void		createHistDisplay( Widget );
	virtual void		createImageDisplay( Widget );
	virtual void		createImageModel();
	virtual void		createPopup();
	virtual void		createTextDisplay();
   	   	
		
  public:

	DspInstrNimSci(Widget parent, char * name, DspWindowBasic * window) ;
	~DspInstrNimSci(){}
	virtual const char *const className() { return "DspInstrNimSci"; }
	
};
#endif
