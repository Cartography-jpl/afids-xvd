////////////////////////////////////////////////////////////////
// DspWindowRtGll.h
////////////////////////////////////////////////////////////////
#ifndef DSPWINDOWRTGLL_H
#define DSPWINDOWRTGLL_H
#include "DspWindowBasic.h"

Cursor createBlankCursor ( Widget iw);

class DspWindowRtGll : public DspWindowBasic {

   protected:
   
	virtual int	getNextFrame(){return(RTS_GllFileAvailable(_filename, &_instrument));}
						
   public:

	DspWindowRtGll(char * name) : DspWindowBasic(name){}
	~DspWindowRtGll() {}

	virtual void 	setup();
	virtual void 	initInterface(int , char **) ; 
		
};
#endif
