///////////////////////////////////////////////////////////////////
//  DspImageBasic.h
///////////////////////////////////////////////////////////////////
#ifndef DSPIMAGEBASIC_H
#define DSPIMAGEBASIC_H
#include "DspUIComponent.h"
#include "BasicImageView.h"
#include "WedgeView.h"
#include "ImageDisplayView.h"
#include "ImageData.h"
#include "DspAxisHorizView.h"
#include "DspAxisVertView.h"
#include "DspBarHorizView.h"
#include "DspBarVertView.h"
#include "DspDisplayPolicy.h"
#include "DspUtilMisc.h"
#include "ZoomFactor.h"
#include "rts_inst_display.h"
#include "Histogram.h"
#include <assert.h>
#include <Xm/Xm.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>
#include <Xm/RowColumn.h>
#include <iostream>


class DspImageBasic : public DspUIComponent {

   protected:

	static String 		_defaultsBasic[]; // resource defaults for
						 //everything except images.

	DspAxisVertView *	_axisWest;
	DspAxisHorizView *	_axisSouth;
	DspAxisVertView *	_axisEast;
	DspAxisHorizView *	_axisNorth;
	
	Widget			_imageAxisFrame;
	Widget			_imageForm;
	Widget			_imageFrame;
	Widget			_imageAttachForm;
	struct ImageView{	  
		ImageDisplayView *	image;
		ImageData *		imageModel;
		int			(*fp_getPseudoMode)();
		void 			(*fp_getStretch)(int *, int *, int * );
		//Boolean		stretchability;
		void			(*fp_getZoom)(int*,int*,int*,int*,int*);
		DspDisplayPolicy *	displayPolicy;
		int			zoomX;
		int			zoomY;
		int			panX;
		int			panY;
		int			oddEvenFlag;
			
	};
	struct WedgeViewPlus{	  
		WedgeView *		wedge;
		int			(*fp_getPseudoMode)();
		void	 		(*fp_getStretch)(int *, int *, int *);
		//Boolean		stretchability;
		void			(*fp_getZoom)(int*,int*,int*,int*,int*);
		DspDisplayPolicy *	displayPolicy;
	};
	ImageView **		_imageView;	
	WedgeViewPlus *		_wedgeView;
	Widget			_wedgeForm;
	Widget			_wedgeFrame;
	Widget			_parent;
	int			_numbImages;
	int			_viewHeight; // ht of all images 
	int			_viewWidth; // wdth of all images in image area
	int			_wedge_flag; // where is wedge (lrtb?)
	Boolean			_isDialogStretch; 
	
	virtual void createPrimaryImage(char * name, ImageData * dataModel, 
	 		DspDisplayPolicy * displayPolicy,
			int view_height, int view_width, int zoomX, int zoomY,
	 		int panX, int panY, int oddEvenFlag,
			int (*fp_getPseudoMode)(),  // pointer to function
			void (*fp_getStretch)(int *, int *, int *),
			void (*fp_getZoom)(int*,int*,int*,int*,int*) // pntr to getZoom 
	 		) = 0;
	virtual void zoomImage(int zoomX, int zoomY, int panX, int panY, 
				int oddEvenFlag, ImageDisplayView * imageView );
	
   public:

	DspImageBasic(Widget parent, char * name );
	~DspImageBasic();
	inline void setDialogStretch(Boolean b) { _isDialogStretch = b; };
	virtual void createImageWedge();
	Widget get_wedge_widget(void) { return(	_wedgeFrame); }
	
	virtual void createImageArea( ImageData *dataModel, 
			DspDisplayPolicy * displayPolicy,
			int view_height, int view_width, int zoomX, int zoomY,
			int panX, int panY, int oddEvenFlag,
			int (*fp_getPseudoMode)(),  // pointer to function
			void (*fp_getStretch)(int *, int *, int *),
			void (*fp_getZoom)(int*,int*,int*,int*,int*), // pntr to getZoom 
			int wedge_flag = 1				//add wedge to left??
			);

	virtual ImageDisplayView *addFirstImage( char * name, ImageData * dataModel, 
	 		DspDisplayPolicy * displayPolicy,
			int view_height=400, int view_width=400, 
	 		int zoomX=1, int zoomY=1,
	 		int panX=0, int panY=0,
	 		int oddEvenFlag=0,
	 		int (*fp_getPseudoMode)()=setPseudoModeOff,  // pointer to function
			void (*fp_getStretch)(int *, int *, int *)=fillRGBImageStretchLut,
			void (*fp_getZoom)(int*,int*,int*,int*,int*)=setZoomParams2Off // pntr to getZoom 
	 		) = 0;

	virtual ImageDisplayView *addImage(char * name, ImageData *dataModel, 
	 		DspDisplayPolicy * displayPolicy,
			int view_height=400, int view_width=400, 
	 		int zoomX=1, int zoomY=1,
	 		int panX=0, int panY=0,
	 		int oddEvenFlag=0,
	 		int (*fp_getPseudoMode)()=setPseudoModeOff,  // pointer to function
			void (*fp_getStretch)(int *, int *, int *)=fillRGBImageStretchLutWithRaw,
			void (*fp_getZoom)(int*,int*,int*,int*,int*)=setZoomParams2Off // pntr to getZoom 
	 		);

	virtual Widget  		getImageWidget(int i = 0);  
	virtual DspDisplayPolicy * 	getDisplayPolicy(int i = 0);
	inline	int			getNumbImages() { return _numbImages; }
	inline  ImageData *		getImageModel(int i = 0) { return (_imageView[i]->imageModel); }
	virtual void			update();
	virtual void			updateImageZoom ();
};
#endif
