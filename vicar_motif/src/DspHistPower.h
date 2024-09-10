///////////////////////////////////////////////////////////////////
//  DspHistPower.h
///////////////////////////////////////////////////////////////////
#ifndef DSPHISTPOWER_H
#define DSPHISTPOWER_H
#include "DspHistBasic.h"
#include "BasicImageView.h"
#include "ImageData.h"
#include <Xm/Xm.h>

class DspHistPower : public DspHistBasic {

protected:
		
	inline int *		getHistArray() { return getHistPwrSpecArray(); }
	
public:

	DspHistPower(Widget parent, 
				char * 		name, 
				ImageData * dataModel, 
				char * 		titleLabel,
				DspDisplayPolicy *  displayPolicy  )
			: DspHistBasic(parent,name,dataModel,titleLabel, displayPolicy ) 
			
					{ createAllViews(); }
};
#endif

