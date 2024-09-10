///////////////////////////////////////////////////////////////////
//  DspImageEmpty.cc: Displays an Empty image area, a wedge to the left, 
//		 and a special frame around the Image area.
//		This is obsolete.  these functions could be merged into 
//		DspImageBasic.cc
//
///////////////////////////////////////////////////////////////////
#include "DspImageEmpty.h"

String DspImageEmpty::_defaultsImageEmpty [ ] = {
	"*scrollBarDisplayPolicy:       XvicNEVER",
	"*borderWidth:		  	0",
	"*shadowThickness:		0",
	"*highlightThickness:		0",
        NULL,
};
///////////////////////////////////////////////////////////////////
//	addFirstImage
///////////////////////////////////////////////////////////////////
ImageDisplayView * DspImageEmpty::addFirstImage(char *name, ImageData *dataModel, 
			DspDisplayPolicy * displayPolicy,
			int image_height, int image_width, 
			int zoomX, int zoomY,
			int panX, int panY, int oddEvenFlag,
			int (*fp_getPseudoMode)(),  // pointer to function
			void (*fp_getStretch)(int *, int *, int *),
			void (*fp_getZoom)(int*,int*,int*,int*,int*)) // pntr to getZoom 
{
	setDefaultResources ( XtParent(_w), _defaultsImageEmpty );  

	ImageDisplayView * image = addImage( name, dataModel, 
			displayPolicy,
			image_height, image_width, 
			zoomX, zoomY, panX, panY, oddEvenFlag,
			fp_getPseudoMode, 
			fp_getStretch,
			fp_getZoom);

// 	CREATE FRAMED WEDGE (needs info from first imageView)
	if(_wedgeFrame)createImageWedge();
		
//	RETURN
	return( image );
}
