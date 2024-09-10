///////////////////////////////////////////////////////
// DspBarHorizView.cc:
////////////////////////////////////////////////////////
#include "DspBarHorizView.h"

DspBarHorizView::DspBarHorizView ( Widget parent, char *name, Widget widgetCap ) 
			: DspBarBasicView (parent, name, widgetCap) 
{

}
void DspBarHorizView::display( )
{
   if (XtIsRealized(_w) ) {  
   
//	SET SIZE OF RULER = SIZE OF WIDGETCAP 
	Dimension length;
   	XtVaGetValues ( _widgetCap, XmNwidth, &length, NULL );
//	length = length - 5; 
	XtVaSetValues ( _ruler, XmNwidth,  length, NULL );    		                	               	
	                		
//	DRAW LINE				
        XDrawLine ( XtDisplay(_ruler), XtWindow(_ruler), _gc, 
			0,  0, 
			(int) length, 0 );
   }
}
