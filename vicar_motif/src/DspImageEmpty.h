///////////////////////////////////////////////////////////////////
//  DspImageEmpty.h
///////////////////////////////////////////////////////////////////
#ifndef DSPIMAGEEMPTY_H
#define DSPIMAGEEMPTY_H
#include "DspImageBasic.h"

class DspImageEmpty : public DspImageBasic {

   protected:

	static String _defaultsImageEmpty[]; // resource defaults for ImageEmpty only
	virtual void  createPrimaryImage(char *, ImageData *, DspDisplayPolicy * ,
						int, int, int, 
						int, int, int, int,
						int (*)(),
						void (*)(int *, int *, int *),			
						void (*)(int*,int*,int*,int*,int*))
						{ }

   public:

	DspImageEmpty(Widget parent, char * name, 
			ImageData * dataModel=0, DspDisplayPolicy * displayPolicy=0,
			int view_height = 400, int view_width = 400, int zoomX=1, int zoomY=1,
			int panX=0, int panY=0, int oddEvenFlag=0, int wedge_flag = 1) 
	      : DspImageBasic( parent, name ) 
		{ 
			int (*fp_getPseudoMode)() = getPseudoMode;
			void (*fp_getStretch)(int *, int *, int * ) = fillRGBImageStretchLut;
			void (*fp_getZoom)(int*,int*,int*,int*,int*) = fillZoomParams;
			createImageArea(dataModel, displayPolicy, view_height,view_width, zoomX, zoomY,
					panX, panY, oddEvenFlag,fp_getPseudoMode,fp_getStretch,
					fp_getZoom, wedge_flag); 
		}

        virtual ImageDisplayView * addFirstImage( 
        		char *, ImageData *,
        		DspDisplayPolicy * displayPolicy,
			int image_height, int image_width, int zoomX, int zoomY,
			int panX, int panY, int oddEvenFlag,
			int (*fp_getPseudoMode)(),  // pointer to function
			void (*fp_getStretch)(int *, int *, int *),			
			void (*fp_getZoom)(int*,int*,int*,int*,int*)); // pntr to getZoom );

};
#endif
