///////////////////////////////////////////////////////////////////
//  DspHistInputPlain.h
///////////////////////////////////////////////////////////////////
#ifndef DSPHISTINPUTPlain_H
#define DSPHISTINPUTPlain_H
#include "DspHistPlain.h"
#include "BasicImageView.h"
#include "ImageData.h"
#include <Xm/Xm.h>

class DspHistInputPlain : public DspHistPlain {

protected:
		
	inline int *		getHistArray() { 
					if(getHistArrayfunc) {
						return getHistArrayfunc();
					} else {
						return getHistInputArray(); 
					}
				}

public:

	DspHistInputPlain(Widget parent, 
				char * 		name, 
				ImageData * dataModel, 
				char * 		titleLabel,
				DspDisplayPolicy *  displayPolicy,
				int		displayLabel=0 )
			: DspHistPlain(parent,name,dataModel,titleLabel, displayPolicy, displayLabel ) 
				{ createAllViews(); }


};
#endif
