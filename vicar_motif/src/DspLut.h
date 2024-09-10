///////////////////////////////////////////////////////////////////
//  DspLut.h
///////////////////////////////////////////////////////////////////
#ifndef DSPLUT_H
#define DSPLUT_H

#include "Lut.h"
#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>


class DspLut : public Lut {


   protected:

	int * 	_defaultLut;		// default values
	int	_pixelSize;		// numb bits	
	int	_lutSize;		

   public:

	DspLut( int pixelSize=8);
	~DspLut() { delete [] _lut; }

	inline  int	getDN(int index) { return(_lut[index]); /*needs test*/}
	virtual void 	setDefAsArray ( int *array );
	virtual void	restoreDefault( int start, int end );
	virtual void	setFlat( int firstIndex, int lastIndex, int value);
	virtual void	setLinear(int firstIndex, int lastIndex, 
					int startValue, int endValue);
	virtual void	setAsArray(int * array); // copy in new array - not just int*array;
	virtual void    resizeLut(int pixelSize);
	inline  int	getNumberOfBitsInPixel()
					{return(_pixelSize);}
	inline  int     getLutSize() { return _lutSize; }
	inline  int     getPixelSize() { return _pixelSize; }

};
#endif
