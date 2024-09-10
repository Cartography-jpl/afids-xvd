///////////////////////////////////////////////////////////////////
// 	 DspImageBasic.cc: Displays an image, a wedge to the left, 
//		 and a special frame around the image 
// 	   WARNING:   (1) See warning at bottom regarding a Motif
// 	   bug that affects this design.
//	   (2) fractional zoom not coded in here.
//	   (3) GUI stretch dialog removed but some old useless code
//	    still remains - cause I was going to get around to fixing
//	    the stretch dialog problems but didn't have a chance.
///////////////////////////////////////////////////////////////////
#include "DspImageBasic.h"
#include <stdlib.h>

///////////////////////////////////////////////////////////////////
//	Resource Defaults  .. widget resources
///////////////////////////////////////////////////////////////////
String DspImageBasic::_defaultsBasic [ ] = {
	"*imageFrame.width:			500",
	"*imageFrame.height:			300",

	"*imageFrame.background:		black",
	"*imageFrame.foreground:		black",

	"*wedgeView.stretchPolicy:              XvicUSE_SW",
	"*wedgeView.colormapPolicy:             XvicALLOC",
	"*wedgeView.scrollBarDisplayPolicy:     XvicNEVER",
	"*wedgeView*orientation:          	VERTICAL",
	"*wedgeView*nsteps:               	16",
	"*wedgeView*minPixelDN:            	0",
	"*wedgeView*maxPixelDN:           	255",
	"*wedgeView.shadowThickness:		0",
	"*wedgeView.highlightThickness:		0",	
	"*wedgeFrame.marginWidth:		0",
	"*wedgeFrame.marginHeight:		0",
	"*wedgeFrame.topShadowColor:		white",
	"*wedgeFrame.bottomShadowColor:		white",				
	"*wedgeFrame.shadowThickness:		1",	
	"*borderWidth:		  		0",

	"*axisNorth*longTickLength:		4",
	"*axisSouth*longTickLength:		4",		
	"*axisEast*longTickLength:		4",
	"*axisWest*longTickLength:		4",

	"*imageAxisFrame.shadowThickness:	1",
	"*imageAxisFrame.marginWidth:		0",
	"*imageAxisFrame.marginHeight:		0",
	"*imageAxisFrame.leftOffset:		6",
	"*imageAxisFrame*topShadowColor:	white",
	"*imageAxisFrame*bottomShadowColor:	white",	

	"*imageFrame.shadowThickness:		1",
	"*imageFrame.marginHeight:		0",	
	"*imageFrame.marginWidth:		0",
        NULL,
};

///////////////////////////////////////////////////////////////////
//	CONSTRUCTOR
///////////////////////////////////////////////////////////////////
DspImageBasic::DspImageBasic(Widget parent, char * name)
				: DspUIComponent(name)
{
	_imageView = 0;
	_numbImages = 0;
	_parent = parent;  
	_isDialogStretch = FALSE;
	
}

///////////////////////////////////////////////////////////////////
//	createImageArea
///////////////////////////////////////////////////////////////////
void  DspImageBasic::createImageArea( ImageData *dataModel,
		DspDisplayPolicy * displayPolicy,			
		int view_height, int view_width, int zoomX, int zoomY,
		int panX, int panY, int oddEvenFlag,
		int (*fp_getPseudoMode)(),  // pointer to function
		void (*fp_getStretch)(int *, int *, int *),
		void (*fp_getZoom)(int*,int*,int*,int*,int*), // pntr to getZoom 
		int wedge_flag)
{
	int n = 0;
	Arg args[20];
	
	setDefaultResources ( _parent, _defaultsBasic );  

	_wedge_flag = wedge_flag;

	_viewHeight = view_height;  //  height of area holding images
	_viewWidth = view_width;   // width of area holding images

// 	CREATE A FORM TO HOLD IMAGE WIDGET, AXIS, & WEDGE 
	n=0;	
	XtSetArg(args[n], XmNmarginHeight,  0); n++;
	XtSetArg(args[n], XmNmarginWidth, 0); n++;
	_w = XtCreateWidget( _name, xmFormWidgetClass, _parent, args, n);
       	installDestroyHandler();

//	CREATE FRAME FOR WEDGE (TO BE CREATED LATER)
	if(wedge_flag & 15) {
		_wedgeFrame = XtCreateManagedWidget( "wedgeFrame", xmFrameWidgetClass, _w, NULL, 0);
	} else if(wedge_flag) {
		_wedgeFrame = XtCreateManagedWidget( "wedgeFrame", xmFrameWidgetClass, XtParent(_parent), NULL, 0);
	} else {
		_wedgeFrame = 0;
	}
	       	       
// 	CREATE FRAME (FOR AXIS)
        _imageAxisFrame = XtCreateManagedWidget( "imageAxisFrame", xmFrameWidgetClass, _w, NULL, 0);

// 	CREATE FRAMED IMAGE (IF NOT EMPTY)
	_imageForm  = XtCreateManagedWidget( "imageForm", xmFormWidgetClass, _imageAxisFrame, NULL, 0);	
	n = 0;
	XtSetArg(args[n], XmNwidth,  view_width); n++;
	XtSetArg(args[n], XmNheight, view_height); n++;	
	_imageFrame = XtCreateManagedWidget( "imageFrame", xmFrameWidgetClass, _imageForm, args, n);
	_imageAttachForm  = XtCreateManagedWidget( "imageAttachForm", xmFormWidgetClass, _imageFrame, NULL, 0);
	createPrimaryImage("imageView", dataModel, displayPolicy, 
				view_height, view_width, 
				zoomX, zoomY,
				panX, panY, oddEvenFlag, fp_getPseudoMode,
				fp_getStretch, fp_getZoom);
		
// 	CREATE AXIS (BETWEEN (INSIDE) IMAGEFRAME AND (OUTSIDE) IMAGEFORM) 
   	_axisWest  = new DspAxisVertView( _imageForm, "axisWest",_imageFrame);
	_axisSouth = new DspAxisHorizView( _imageForm, "axisSouth",_imageFrame);
	_axisEast  = new DspAxisVertView( _imageForm, "axisEast",_imageFrame);
	_axisNorth = new DspAxisHorizView( _imageForm, "axisNorth",_imageFrame);
  	_axisWest->setOffset( _axisNorth->getTickLength()+1); 
	_axisEast->setOffset( _axisNorth->getTickLength()+1 ); 
	_axisNorth->setOffset( _axisWest->getTickLength()+1 ); 
	_axisSouth->setOffset( _axisWest->getTickLength()+1 ); 
	_axisWest->manage(); 
	_axisSouth->manage();
	_axisEast->manage();
	_axisNorth->manage();

//	ATTACH EVERYTHING TOGETHER	          	        	  
	int spacing = (int) _axisNorth->getTickLength()+1;
	if(wedge_flag & 1) {	// wedge on left
		XtVaSetValues( _wedgeFrame, // 	set vertical spacing of wedge = image
        		XmNtopAttachment, 	XmATTACH_FORM,
        		XmNtopOffset,     	spacing,
        		XmNleftAttachment,	XmATTACH_FORM,
        		NULL ); 
		XtVaSetValues( _imageAxisFrame,
        		XmNtopAttachment, 	XmATTACH_FORM,
        		XmNleftWidget,		_wedgeFrame,
        		XmNleftAttachment,	XmATTACH_WIDGET,
        		NULL ); 
	} else  if(wedge_flag & 2) {	// wedge on right
		XtVaSetValues( _imageAxisFrame,
        		XmNtopAttachment, 	XmATTACH_FORM,
        		XmNleftAttachment,	XmATTACH_FORM,
        		NULL ); 
		XtVaSetValues( _wedgeFrame, // 	set vertical spacing of wedge = image
        		XmNtopAttachment, 	XmATTACH_FORM,
        		XmNtopOffset,     	spacing,
        		XmNleftWidget,		_imageAxisFrame,
        		XmNleftAttachment,	XmATTACH_WIDGET,
        		NULL ); 
	} else  if(wedge_flag & 4) {	// wedge on top
		XtVaSetValues( _wedgeFrame, // 	set vertical spacing of wedge = image
        		XmNtopAttachment, 	XmATTACH_FORM,
        		XmNleftAttachment,	XmATTACH_FORM,
        		XmNleftOffset,     	spacing,
        		NULL ); 
		XtVaSetValues( _imageAxisFrame,
        		XmNleftAttachment, 	XmATTACH_FORM,
        		XmNtopWidget,		_wedgeFrame,
        		XmNtopAttachment,	XmATTACH_WIDGET,
        		NULL ); 
	} else  if(wedge_flag & 8) {	// wedge on bottom
		XtVaSetValues( _imageAxisFrame,
        		XmNtopAttachment, 	XmATTACH_FORM,
        		XmNleftAttachment,	XmATTACH_FORM,
        		NULL ); 
		XtVaSetValues( _wedgeFrame, // 	set vertical spacing of wedge = image
        		XmNleftAttachment, 	XmATTACH_FORM,
        		XmNleftOffset,     	spacing,
        		XmNtopWidget,		_imageAxisFrame,
        		XmNtopAttachment,	XmATTACH_WIDGET,
        		NULL ); 
	} else  if(wedge_flag & 16) {	// wedge to be placed independently
		XtVaSetValues( _imageAxisFrame,
        		XmNtopAttachment, 	XmATTACH_FORM,
        		XmNleftAttachment,	XmATTACH_FORM,
        		NULL ); 
	} else {		// no wedge
		XtVaSetValues( _imageAxisFrame,
        		XmNtopAttachment, 	XmATTACH_FORM,
        		XmNleftAttachment,	XmATTACH_FORM,
        		NULL ); 
	}
        XtVaSetValues ( _axisNorth->baseWidget(),	// 	TOP AXIS
               		XmNtopAttachment,    	XmATTACH_FORM,
               		NULL );	   
        XtVaSetValues ( _axisWest->baseWidget(),	// 	LEFT AXIS
               		XmNleftAttachment,     	XmATTACH_FORM,            
               		NULL );
        XtVaSetValues ( _imageFrame,
          		XmNtopWidget,		_axisNorth->baseWidget(),
          		XmNtopAttachment,	XmATTACH_WIDGET,
          		XmNleftWidget,		_axisWest->baseWidget(),
          		XmNleftAttachment,	XmATTACH_WIDGET,
          		NULL );
        XtVaSetValues ( _axisSouth->baseWidget(),	// 	Bottom AXIS
               		XmNtopWidget,		_imageFrame,
               		XmNtopAttachment,     	XmATTACH_WIDGET,
               		XmNbottomAttachment,	XmATTACH_FORM,
               		NULL );
        XtVaSetValues ( _axisEast->baseWidget(),	// 	RIGHT AXIS
               		XmNleftWidget,		_imageFrame,
               		XmNleftAttachment,     	XmATTACH_WIDGET,
               		XmNrightAttachment,    	XmATTACH_FORM,
               		NULL );

}

///////////////////////////////////////////////////////////////////
//	addImage - called by subclasses, 1 or more times
///////////////////////////////////////////////////////////////////
ImageDisplayView * DspImageBasic::addImage(char *name, 
			ImageData *dataModel,  		// data Model 
			DspDisplayPolicy * displayPolicy,
			int image_height, int image_width,  // size of image on screen
			int zoomX, int zoomY,  		// initial zoom
			int panX, int panY, int oddEvenFlag,
			int (*fp_getPseudoMode)(),  	// pointer to function
			void	 (*fp_getStretch)(int *, int *, int * ),
			void (*fp_getZoom)(int*,int*,int*,int*,int*)) // pntr to getZoom 
{


// 	MAKE LIST OF VIEWS LARGER By 1
	ImageView **	newImageViewList;	
	newImageViewList = new ImageView*[ _numbImages + 1 ];
	for ( int i = 0; i < _numbImages; i++ ) 
		newImageViewList[i] = _imageView[i];
	if (_numbImages > 0) 
		delete []_imageView;
	_imageView = newImageViewList;
			
// 	CREATE NEW IMAGE (AT END OF LIST)
	_imageView[_numbImages] = new ImageView;
	_imageView[_numbImages]->image = new ImageDisplayView( _imageAttachForm,  name, 
			          dataModel, image_height, image_width ); 
	_imageView[_numbImages]->imageModel = dataModel;
	_imageView[_numbImages]->fp_getPseudoMode = fp_getPseudoMode;	
	_imageView[_numbImages]->fp_getZoom = fp_getZoom;			
	_imageView[_numbImages]->displayPolicy=displayPolicy;
	_imageView[_numbImages]->fp_getStretch=fp_getStretch;
	_imageView[_numbImages]->zoomX = zoomX;
	_imageView[_numbImages]->zoomY = zoomY;
	_imageView[_numbImages]->panX = panX;
	_imageView[_numbImages]->panY = panY;
	_imageView[_numbImages]->image->manage();
	
	
//	ZOOM 
	zoomImage(  zoomX, zoomY, panX, panY, oddEvenFlag, 
			_imageView[_numbImages]->image );
	
		
//	SET PSEUDOCOLORS AND STRETCH IN IMAGE WIDGET
	_imageView[_numbImages]->displayPolicy->updateMode(
				  _imageView[_numbImages]->image->getWidget(),
				  _imageView[_numbImages]->fp_getStretch,
				 (*_imageView[_numbImages]->fp_getPseudoMode)(),
				  False);
		

//	INC COUNTER	
	_numbImages++;
	
//	RETURN IMAGE
	return (_imageView[_numbImages-1]->image);
}
///////////////////////////////////////////////////////////////////
//	Update    (all images in image area)
//			.. called automatically by dspinstrument class
///////////////////////////////////////////////////////////////////
void DspImageBasic::update() 
{

//	MAKE SURE THERE ARE SOME IMAGES  
	if (_numbImages > 0 ) {	

//		UPDATE PSEUDO COLOR & STRETCH TABLE IN ALL IMAGE WIDGETS
		for (int i=0; i<_numbImages; i++) 
			_imageView[i]->displayPolicy->updateMode( 
					    _imageView[i]->image->getWidget(),
					    _imageView[i]->fp_getStretch,
					   (*_imageView[i]->fp_getPseudoMode)(),
					    _isDialogStretch );		
	
//		UPDATE PSEUDO COLOR (ONLY) IN WEDGE, but not stretch table
		if(_wedgeFrame) {
			_wedgeView->displayPolicy->updateMode( 
					_wedgeView->wedge->getWidget(), 
					_wedgeView->fp_getStretch, 
					(*_imageView[0]->fp_getPseudoMode)(),
					    _isDialogStretch );
		}
					
			
//		UPDATE ZOOM IN ALL IMAGE VIEWS
		updateImageZoom();
	}
}

///////////////////////////////////////////////////////////////////
//	Update Image Zoom (in all image views)
///////////////////////////////////////////////////////////////////
void DspImageBasic::updateImageZoom()
{
//	MAKE SURE THERE ARE SOME IMAGES FIRST
	if (_numbImages > 0 ){	
	
//		ZOOM
		int zoomX, zoomY;
		int panY;  // startLine
		int panX;  // startSample
		int oddEvenFlag; 
		
//		FOR EACH VIEW
		for (int i=0; i<_numbImages; i++) 	{
		
//			PASS OLD ZOOM VALUE TO FUNCTION
		        zoomX = _imageView[i]->zoomX;
		        zoomY = _imageView[i]->zoomY;
		        panX  = _imageView[i]->panX;
		        panY  = _imageView[i]->panY;
		        oddEvenFlag = _imageView[i]->oddEvenFlag;
		        
//			CALL FUNCTION (TO GET ZOOM PARAMS FROM gll_dsp_cntrl)		        
			(*_imageView[i]->fp_getZoom)(&zoomX, &zoomY, &panY, &panX, &oddEvenFlag);
			
//			ZOOM IMAGE
			zoomImage(zoomX, zoomY, panX, panY, oddEvenFlag, _imageView[i]->image);
		}
	}
}

///////////////////////////////////////////////////////////////////
//	Zoom Image
//		(Precludes fractional zoom.)
///////////////////////////////////////////////////////////////////
void DspImageBasic::zoomImage (int zoomX, int zoomY, 
				int panX, int panY, 
				int oddEvenFlag, 
				ImageDisplayView * imageView )
{
	ZoomFactor z;
	int zoomXIn, zoomXOut, zoomYIn, zoomYOut;
	
//	BREAK OUT ZOOM X 
	if (zoomX > 0) {
		zoomXIn = zoomX;
		zoomXOut = 1;
	}
	if (zoomX < 0) {
		zoomXIn = 1;
		zoomXOut = abs(zoomX);
	}
	if (zoomX == 0) {
		zoomXIn = 1;
		zoomXOut = 1;
	}

//	BREAK OUT ZOOM Y 
	if (zoomY > 0) {
                zoomYIn = zoomY;
                zoomYOut = 1;
        }
        if (zoomY < 0) {
                zoomYIn = 1;
                zoomYOut = abs(zoomY);
        }
        if (zoomY == 0) {
        	zoomYIn = 1;
        	zoomYOut = 1;
        }

// 	CREATE A ZOOMFACTOR FOR VIEW
	z.setX( zoomXIn, zoomXOut );
	z.setY( zoomYIn, zoomYOut );
	z.setSubPixelPanX( oddEvenFlag );
	z.setSubPixelPanY( oddEvenFlag );	
	
//	SET ZOOM IN VIEW	
	imageView->setUserZoom( z );
	
//	PAN	
	XtVaSetValues(imageView->getWidget(), 
			XvicNxPan, abs(panX), 
			XvicNyPan, abs(panY),
			NULL);

}

///////////////////////////////////////////////////////////////////
//	
///////////////////////////////////////////////////////////////////
Widget DspImageBasic::getImageWidget (int i)
{
	if ((i < _numbImages ) && ( i >= 0) && ( _numbImages > 0))
		 return (_imageView[i]->image->getWidget());
	else return 0;
}
///////////////////////////////////////////////////////////////////
//	
///////////////////////////////////////////////////////////////////
DspDisplayPolicy * DspImageBasic::getDisplayPolicy (int i)
{
	if ((i < _numbImages ) && ( i >= 0) && ( _numbImages > 0))
		 return (_imageView[i]->displayPolicy);
	else return 0;
}

///////////////////////////////////////////////////////////////////
//	Destructor
///////////////////////////////////////////////////////////////////
DspImageBasic::~DspImageBasic()
{

	RemoveFormAttachments( _axisEast->baseWidget());
	RemoveFormAttachments( _axisSouth->baseWidget());
	RemoveFormAttachments( _imageFrame);
	RemoveFormAttachments( _imageForm);		 	 
	RemoveFormAttachments( _axisWest->baseWidget());
	RemoveFormAttachments( _axisNorth->baseWidget());
	RemoveFormAttachments( _imageAxisFrame );         
	if( _wedgeFrame ) {
		RemoveFormAttachments( _wedgeFrame );
		RemoveFormAttachments( _wedgeView->wedge->baseWidget());
		delete _wedgeView;
		_wedgeView = 0;
		_wedgeFrame = 0;
	}
	
	delete []_imageView;                 	 	 	 	 	 	         
	delete _axisEast;
	delete _axisSouth;
	delete _axisWest;
	delete _axisNorth;		
}

///////////////////////////////////////////////////////////////////
//	createImageWedge (called after first image view is created
///////////////////////////////////////////////////////////////////
void  DspImageBasic::createImageWedge()
{

// 	CREATE FRAMED WEDGE (needs info from first imageView)
	
	_wedgeView = new WedgeViewPlus;
	_wedgeView->displayPolicy = _imageView[0]->displayPolicy; // wedge shares 1st view policy
	_wedgeView->fp_getStretch = fillRGBImageStretchLutWithRaw;

	if(_wedge_flag & 28) {	// if wedge on top or bottom of image or horizontal independent
		_wedgeView->wedge = new WedgeView( _wedgeFrame,  "wedgeView", 30, _viewWidth );
		_wedgeView->wedge->setOrientation(HORIZONTAL);
	} else {
		_wedgeView->wedge = new WedgeView( _wedgeFrame,  "wedgeView", _viewHeight, 30 );
		_wedgeView->wedge->setOrientation(VERTICAL);
	}

        _wedgeView->wedge->manage();        
	_wedgeView->displayPolicy->updateMode(
					_wedgeView->wedge->getWidget(), 
					_wedgeView->fp_getStretch,
					(*_imageView[0]->fp_getPseudoMode)(),
					False); 

}

/*
WARNING:    This component should use a FORM widget to contain its child.  However, there
seems to be a bug in (at least) Motif 1.2.1's Form widgets.  If you have a form, with a child that's
a form, with a child that's a ScrolledWindow, some bizarre behavior results.  If the SW is adjusted
such that one scrollbar is managed and the other not,  and then adjusted again such that neither
are managed (via methods other than resizing the window), the XtUnmanagedChild() call for the
second scrollbar causes the window to expand to be big enough to contain the other,
*unmanaged*, scrollbar at its old location!  This is completely bogus as unmanaged widgets should
never take part in geometry negotiations.

This bug seems to occur only when two adjacent Form's are in the hierarchy.  Changing
one Form to a Frame causes the problem to disappear.  Inserting a Frame between the
two Forms also causes the problem to disappear.  In this case, since there is only one child of 
this component, we replace the Form with a Frame.  In general, if more than one child of this is
needed (so we have to use a Form), we may need to insert a Frame widget around the Form.  A
Frame with XmNshadowThickness set to '0' is just as good as a Form with a single child with all
its attachments set to ATTACH_FORM.
*/
       
//
