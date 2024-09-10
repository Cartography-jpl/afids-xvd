/////////////////////////////////////////////////////////////////
// DspDataNimEngRB.h    
//
////////////////////////////////////////////////////////////////
#ifndef DSPDATANIMENGRB_H
#define DSPDATANIMENGRB_H
#include "DspDataNimBasicGll.h"

class DspDataNimEngRB : public DspDataNimBasicGll {

   protected:

   public:

	DspDataNimEngRB::DspDataNimEngRB(void (*fp_getPixels)(unsigned char*,unsigned char*,unsigned char*),
					void (*fp_getRawDN)(unsigned int*, unsigned int*,unsigned int *),
					int nl, int ns, 
					int nc, char * pixelType) 
			: DspDataNimBasicGll((*fp_getPixels), (*fp_getRawDN), nl,ns, 
						nc, pixelType) {}
	virtual StatusType open( char * ); 

	virtual char *className() { return ("DspDataNimEngRB"); }
};
#endif 

