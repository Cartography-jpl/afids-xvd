///////////////////////////////////////////////////////
// DspLut.cc: Is 1 PseudoColorLut or Stretch Lut
////////////////////////////////////////////////////////
#include "DspLut.h"


///////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////
DspLut::DspLut(int pixelSize) : Lut()
{

	_pixelSize=pixelSize; 
	_lutSize=(int) pow((double)2,_pixelSize);

	_defaultLut = new int [_lutSize];
	
	delete [] _lut;
	_lut = new int [_lutSize];
		 
        setDefAsArray(_lut); 
}

///////////////////////////////////////////////////////////////////////////
// Set Default array
///////////////////////////////////////////////////////////////////////////
void DspLut::setDefAsArray ( int *array )
{
        for (int i=0; i<_lutSize; i++)
                _defaultLut[i] = array[i];
}

///////////////////////////////////////////////////////////////////////////
// Restore default values between two points (points inclusive)
///////////////////////////////////////////////////////////////////////////
void DspLut::restoreDefault ( int start, int end )
{
	for (int i = start; i <= end; i++) {
		_lut[i] = _defaultLut[i];
	}
}
///////////////////////////////////////////////////////////////////////////
// Do flat line between two points (points inclusive)
///////////////////////////////////////////////////////////////////////////
void DspLut::setFlat( int firstIndex, int lastIndex, int value)
{

	if (firstIndex > lastIndex) {
		int temp = firstIndex;
		firstIndex = lastIndex;
		lastIndex = temp;
	}
	for (int i=firstIndex; i<=lastIndex; i++)
		_lut[i] =value;

}
///////////////////////////////////////////////////////////////////////////
// Do linear interpolation between two points (points inclusive)
///////////////////////////////////////////////////////////////////////////
void DspLut::setLinear(int startIndex, int endIndex,
				int startValue, int endValue)
{
	assert(endIndex > startIndex);
	for (int i = startIndex; i <= endIndex; i++) 
                _lut[i] = ( (i-startIndex)*(endValue-startValue) +
                          startValue*(endIndex-startIndex) ) /
                          (endIndex-startIndex);
	
}
///////////////////////////////////////////////////////////////////////////
// 	setAsArray
//		Different than setAsArray in parent.  This copies in 
//		array to store locally.  SetAsArray only saves address
//		of array residing somewhere else.
///////////////////////////////////////////////////////////////////////////
void DspLut::setAsArray( int* array ) 
{
	for (int i=0; i<=_lutSize; i++)
		_lut[i] = array[i];
}
///////////////////////////////////////////////////////////////////////////
// 	resizeLut
///////////////////////////////////////////////////////////////////////////
void    DspLut::resizeLut(int pixelSize) // pixelSize = numb bits in pixel
{
//	SET PIXEL SIZE AND NEW SIZE OF COLORMAP
	_pixelSize=pixelSize;
	_lutSize=(int) pow((double)2,_pixelSize);
	
//	CREATE A NEW COLORMAP
	delete [] _lut;
	_lut = new int [_lutSize];	
}

