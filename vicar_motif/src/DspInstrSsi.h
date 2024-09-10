///////////////////////////////////////////////////////////////////
//  DspInstrSsi.h
///////////////////////////////////////////////////////////////////
#ifndef DSPINSTRSSI_H
#define DSPINSTRSSI_H
#include "DspInstrBasic.h"
#include <iostream>

class DspInstrSsi : public DspInstrBasic {

   protected:

	virtual void	createImageDisplay( Widget );
	virtual void	createHistDisplay( Widget );
	virtual void	createImageModel();
	virtual void	createPopup();
	virtual void	createTextDisplay();
   	
   

  public:

	DspInstrSsi(Widget parent, char * name, 
			DspWindowBasic * window);
	virtual const char *const className() { return "DspInstrSsi"; }
};

#endif
