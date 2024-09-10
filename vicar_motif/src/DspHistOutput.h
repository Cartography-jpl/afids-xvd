///////////////////////////////////////////////////////////////////
//  DspHistOutput.h
///////////////////////////////////////////////////////////////////
#ifndef DSPHISTOUTPUT_H
#define DSPHISTOUTPUT_H
#include "DspHistBasic.h"
#include "BasicImageView.h"
#include "ImageData.h"
#include <Xm/Xm.h>

class DspHistOutput : public DspHistBasic {

protected:
		
	inline int *		getHistArray() { return getHistOutputArray(); }
public:

	DspHistOutput(Widget parent, 
				char * 		name, 
				ImageData * dataModel, 
				char * 		titleLabel,
				DspDisplayPolicy *  displayPolicy  )
			: DspHistBasic(parent,name,dataModel,titleLabel, displayPolicy ) 
					{ createAllViews(); }
};
#endif
