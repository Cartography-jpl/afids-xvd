///////////////////////////////////////////////////////
// DspBarVertView.cc:
////////////////////////////////////////////////////////
#include "DspBarVertView.h"

DspBarVertView::DspBarVertView ( Widget parent, char *name, Widget widgetCap ) 
			: DspBarBasicView (parent, name, widgetCap) 
{

}
void DspBarVertView::display( )
{

   if (XtIsRealized(_w) ) {  
   
//	SET SIZE OF RULER = SIZE OF WidgetCap   
	Dimension length;
   	XtVaGetValues ( _widgetCap, XmNheight, &length, NULL );
//	length = length -2;
	XtVaSetValues ( _ruler, XmNheight,  length, NULL );    
                          		
//	DRAW LINE				
        XDrawLine ( XtDisplay(_ruler), XtWindow(_ruler), _gc, 
			0,  0, 
			0, (int) length );
   }
}

