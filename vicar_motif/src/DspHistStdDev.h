///////////////////////////////////////////////////////////////////
//  DspHistStdDev.h
///////////////////////////////////////////////////////////////////
#ifndef DSPHISTSTDDEV_H
#define DSPHISTSTDDEV_H
#include "DspHistBasic.h"
#include "BasicImageView.h"
#include "ImageData.h"
#include <Xm/Xm.h>

class DspHistStdDev : public DspHistBasic {

protected:
		
	inline int *		getHistArray() { return getHistStdDevArray(); }
	
public:

	DspHistStdDev(Widget parent, 
				char * 		name, 
				ImageData * dataModel, 
				char * 		titleLabel,
				DspDisplayPolicy *  displayPolicy  )
			: DspHistBasic(parent,name,dataModel,titleLabel, displayPolicy ) 
					{ createAllViews(); }

};
#endif

