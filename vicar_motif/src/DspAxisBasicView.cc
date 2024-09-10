///////////////////////////////////////////////////////
// DspAxisBasicView.cc:
////////////////////////////////////////////////////////
#include "DspAxisBasicView.h"


// Resources for this class

XtResource DspAxisBasicView::_resources [ ] = {
 
// {
//    "drawOffset",
//    "DrawOffset",
//    XmRDimension,
//    sizeof ( Dimension ),
//    XtOffset ( DspAxisBasicView *, _drawOffset ),
//    XmRString,
//    ( XtPointer ) "3",
// },
 {
    "longTickLength",
    "LongTickLength",
    XmRDimension,
    sizeof ( Dimension ),
    XtOffset ( DspAxisBasicView *, _longTickLength ),
    XmRString,
    ( XtPointer ) "7",
 },
  {
    "numbTicks",
    "NumbTicks",
    XmRInt,
    sizeof ( int ),
    XtOffset ( DspAxisBasicView *, _numTicks ),
    XmRString,
    ( XtPointer ) "21",
 },
};

DspAxisBasicView::DspAxisBasicView ( Widget parent, char *name, 
					   Widget insideWidget )
					 : DspUIComponent(name)
{
	_insideWidget = insideWidget;
	_w = XtVaCreateWidget ( _name,
                                xmFormWidgetClass, parent,
                                NULL );
	installDestroyHandler();
	
	getResources ( _resources, XtNumber ( _resources ) );

        _ruler  = XtVaCreateManagedWidget ( "ruler",
                                        xmDrawingAreaWidgetClass, _w,
					XmNleftAttachment,	XmATTACH_FORM,
                     			XmNtopAttachment,     	XmATTACH_FORM,
                     			XmNrightAttachment,   	XmATTACH_FORM,
                     			XmNbottomAttachment,  	XmATTACH_FORM,
					XmNborderWidth, 0,
                                        NULL );


	XtAddCallback ( _ruler, XmNexposeCallback,
                        &DspAxisBasicView::displayCallback,
                        ( XtPointer ) this );


	XGCValues values;
	values.line_width = 1; 
	values.foreground = XWhitePixelOfScreen( XtScreen( _w) ) ;
	unsigned long valueMask = GCLineWidth | GCForeground;
	_gc = XtGetGC ( _w, valueMask,&values); 
}
DspAxisBasicView::~DspAxisBasicView ()
{
	RemoveFormAttachments( _ruler );
	if ( _w && _gc )
		XtReleaseGC ( _w, _gc );
	XtRemoveCallback ( _ruler, XmNexposeCallback,
                        &DspAxisBasicView::displayCallback,
                        ( XtPointer ) this );
}
void  DspAxisBasicView::displayCallback ( Widget,
                                 XtPointer clientData,
                                 XtPointer )
{
        DspAxisBasicView *obj = ( DspAxisBasicView * ) clientData;
        obj->display();
}

 


