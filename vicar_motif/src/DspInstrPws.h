///////////////////////////////////////////////////////////////////
//  DspInstrPws.h
///////////////////////////////////////////////////////////////////
#ifndef DSPINSTRUMENTPWS_H
#define DSPINSTRUMENTPWS_H
#include "DspInstrBasic.h"
#include "DspHistPower.h"
#include "DspDataPwsGll.h"
#include <Xm/Xm.h>
#include <iostream>

class DspDataPwsGll;

class DspInstrPws : public DspInstrBasic {
   protected:

	virtual void	createImageDisplay( Widget );
	virtual void	createHistDisplay( Widget );
	virtual void	createImageModel();
	virtual void	createPopup();
	virtual void	createTextDisplay();
   	
   	
	
  public:

	DspInstrPws(Widget parent, char * name, DspWindowBasic * window) 
		: DspInstrBasic(parent, name, window, 1) 
		{ createDisplay(); }
	~DspInstrPws(){}
	virtual const char *const className() { return "DspInstrPws"; }
	
};
#endif
