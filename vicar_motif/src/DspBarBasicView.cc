///////////////////////////////////////////////////////
// DspBarBasicView.cc:
////////////////////////////////////////////////////////
#include "DspBarBasicView.h"



DspBarBasicView::DspBarBasicView ( Widget parent, char *name, Widget widgetCap )
					 : DspUIComponent(name)
{
	_widgetCap = widgetCap;
	_w = XtVaCreateWidget ( _name,
                                xmFormWidgetClass, parent,
                                NULL );
	installDestroyHandler();
	
	 _ruler  = XtVaCreateManagedWidget ( "ruler",
                                        xmDrawingAreaWidgetClass, _w,
					XmNleftAttachment,	XmATTACH_FORM,
                     			XmNtopAttachment,     	XmATTACH_FORM,
                     			XmNrightAttachment,   	XmATTACH_FORM,
                     			XmNbottomAttachment,  	XmATTACH_FORM,
					XmNborderWidth, 0,
                                        NULL );


	XtAddCallback ( _ruler, XmNexposeCallback,
                        &DspBarBasicView::displayCallback,
                        ( XtPointer ) this );


	XGCValues values;
	values.line_width = 1; 
	values.foreground = XWhitePixelOfScreen( XtScreen( _w) ) ;
	unsigned long valueMask = GCLineWidth | GCForeground;
	_gc = XtGetGC ( _w, valueMask,&values);                 

}
DspBarBasicView::~DspBarBasicView ()
{
	RemoveFormAttachments( _ruler );
	if ( _w && _gc )
		XtReleaseGC ( _w, _gc );
	XtRemoveCallback ( _ruler, XmNexposeCallback,
                        &DspBarBasicView::displayCallback,
                        ( XtPointer ) this );
}
void  DspBarBasicView::displayCallback ( Widget,
                                 XtPointer clientData,
                                 XtPointer )
{
        DspBarBasicView *obj = ( DspBarBasicView * ) clientData;
        obj->display();
}

 


