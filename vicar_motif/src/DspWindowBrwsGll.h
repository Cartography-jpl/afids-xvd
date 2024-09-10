////////////////////////////////////////////////////////////////
// DspWindowBrwsGll.h
////////////////////////////////////////////////////////////////
#ifndef DSPWINDOWBRWSGLL_H
#define DSPWINDOWBRWSGLL_H
#include "DspWindowBasic.h"

class DspWindowBrwsGll : public DspWindowBasic {

   protected:
   
   	virtual int	getNextFrame(){return(BRWS_GllFileAvailable(_filename, &_instrument));}
							
   public:

	DspWindowBrwsGll(char * name) : DspWindowBasic(name){}
	~DspWindowBrwsGll() {}

	virtual void 	setup();
	virtual void 	initInterface(int , char **); 
		
};
#endif
