///////////////////////////////////////////////////////////////////
//  DspHistInput.h
///////////////////////////////////////////////////////////////////
#ifndef DSPHISTINPUT_H
#define DSPHISTINPUT_H
#include "DspHistBasic.h"
#include "BasicImageView.h"
#include "ImageData.h"
#include <Xm/Xm.h>

class DspHistInput : public DspHistBasic {

protected:
		
	inline int *		getHistArray() { return getHistInputArray(); }

public:

	DspHistInput(Widget parent, 
				char * 		name, 
				ImageData * dataModel, 
				char * 		titleLabel,
				DspDisplayPolicy *  displayPolicy )
			: DspHistBasic(parent,name,dataModel,titleLabel, displayPolicy ) 
				{ createAllViews(); }


};
#endif
