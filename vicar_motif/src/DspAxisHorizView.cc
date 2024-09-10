///////////////////////////////////////////////////////
// DspAxisHorizView.cc:
////////////////////////////////////////////////////////
#include "DspAxisHorizView.h"

DspAxisHorizView::DspAxisHorizView ( Widget parent, char *name, 
			  Widget insideWidget ) 
			: DspAxisBasicView (parent, name, insideWidget) 
{
   XtVaSetValues ( _ruler, XmNheight, (_longTickLength+1), NULL );
}
void DspAxisHorizView::display( )
{
   if (XtIsRealized(_w) ) {      
   
        Dimension	insideLength;
        XtVaGetValues ( _insideWidget,
                        XmNwidth,  &insideLength,
                        NULL );  
        XtVaSetValues ( _ruler, XmNwidth, insideLength, NULL );   
        
        
//	SET HORIZ OFFSET  TO FORM
	XtVaSetValues( _w, 
        		XmNleftWidget,	   	XtParent(_w),
        		XmNleftAttachment, 	XmATTACH_WIDGET,
        		//XmNhorizontalSpacing,   _leftOrTop,
        		XmNleftOffset,		((int)_leftOrTop),
        		NULL );
        
                              
                          		
        		 
//	DETERMINE TOP OR BOTTOM ATTACHMENT TO INSIDE LENGTH   
	Widget 		topW, bottomW;
	XtVaGetValues ( _w,  XmNtopWidget,  &topW, 
        				XmNbottomWidget, &bottomW,
        				NULL );
	        		
// 	DRAW TICKS
	int 	stepSize= int(insideLength-1) / (_numTicks - 1);
	int 	excess 	= int(insideLength-1) % (_numTicks - 1);
		
	double 	temp1 	= (double)excess / (double)(_numTicks - 1);
	double 	temp2 = temp1;
	
	int	pointX;
	int	pointY1 = 0;
	int	pointY2 = 0 + _longTickLength;
	
	for ( int i=0; i<_numTicks; ++i) {
	   pointX = int((i*stepSize)+temp1 );
	   XDrawLine (XtDisplay(_ruler), XtWindow(_ruler), _gc, 
				pointX, pointY1,pointX,pointY2);
	   temp1 = temp1 + temp2;
        }
   }
}

