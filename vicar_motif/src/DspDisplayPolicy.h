///////////////////////////////////////////////////////////////////
//  DspDisplayPolicy.h
//
///////////////////////////////////////////////////////////////////
#ifndef DSPDISPLAYPOLICY_H
#define DSPDISPLAYPOLICY_H

#include "DspLut.h"
#include "XvicImage.h"
#include "DspDefs.h"
//   #include "DspInstrBasic.h"
#include <Xm/Xm.h>
#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>

class DspInstrBasic;

			
class DspDisplayPolicy {

   protected:
   
	int			_pixelSize;
        			
	DspLut *		_lutPseudoColor[3];  // 0=red, 1=green, 2=blue
	DspLut *		_lutStretch[3]; //stretch made on bw image, before pseudoclring
	
   public:
   
   	DspDisplayPolicy(DspInstrBasic * instrDisplay,
   				char *   instrDisplayWidgetName);
			
	virtual ~DspDisplayPolicy(){
	  for(int i = 0; i < 3; ++i) {
	    delete _lutPseudoColor[i];
	    delete _lutStretch[i];
	  }
	}
	inline void	setLinearInLutPseudoColor(RGBTYPE i, 
				int startIndex, int endIndex,
				int startValue, int endValue)
			{	_lutPseudoColor[i]->setLinear(
					startIndex, endIndex, 
					startValue,endValue);
			}
	inline void	setFlatInLutPseudoColor(RGBTYPE i, 
				int firstIndex, int lastIndex, int value)
			{	_lutPseudoColor[i]->setFlat(
					firstIndex, lastIndex, 
					value );
			}
	inline  DspLut* getPseudoColorLut(RGBTYPE i) 
					{  return( _lutPseudoColor[((int)i)]); }
	inline  DspLut* getStretchLut(RGBTYPE i) 
					{ return( _lutStretch[((int)i)]); }
					
	virtual void	stretchImage(Widget w, 
					void (*fp_getStretch)(int *, int *, int *),
					Boolean isDialogStretch);
	virtual void  	updateMode( Widget w, 
					void (*fp_getStretch)(int *, int *, int *),
					Boolean pseudoColored,
					Boolean isDialogStretch );					
        inline int      getPixelSize()         
                                        { return _pixelSize; }
	inline void     setPixelSize(int pixelSize)
					{ _pixelSize = pixelSize; }					
};
#endif
