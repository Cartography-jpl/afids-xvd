///////////////////////////////////////////////////////////////////
//  DspHistMean.h
///////////////////////////////////////////////////////////////////
#ifndef DSPHISTMEAN_H
#define DSPHISTMEAN_H
#include "DspHistBasic.h"
#include "BasicImageView.h"
#include "ImageData.h"
#include <Xm/Xm.h>

class DspHistMean : public DspHistBasic {

protected:
		
	inline int *		getHistArray() { return getHistMeanArray(); }
	
public:

	DspHistMean(Widget parent, 
				char * 		name, 
				ImageData * dataModel, 
				char * 		titleLabel,
				DspDisplayPolicy *  displayPolicy  )				
			: DspHistBasic(parent,name,dataModel,titleLabel, displayPolicy ) 
				{ createAllViews(); }

};
#endif

