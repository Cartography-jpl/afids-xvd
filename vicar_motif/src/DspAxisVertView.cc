///////////////////////////////////////////////////////
// DspAxisVertView.cc:
////////////////////////////////////////////////////////
#include "DspAxisVertView.h"

DspAxisVertView::DspAxisVertView ( Widget parent, char *name, 
			  Widget insideWidget ) 
			: DspAxisBasicView (parent, name, insideWidget) 
{
	XtVaSetValues ( _ruler, XmNwidth, (_longTickLength+1), NULL );
}
void DspAxisVertView::display( )
{
   if (XtIsRealized(_w) ) {
   
        Dimension	insideLength;
        XtVaGetValues ( _insideWidget,
                        XmNheight,  &insideLength,
                        NULL );  
        XtVaSetValues ( _ruler, XmNheight, insideLength, NULL );    
                             
//	SET HORIZ OFFSET RELATIVE TO FORM
	XtVaSetValues( _w, 
        		XmNtopWidget,	   	XtParent(_w),
        		XmNtopAttachment, 	XmATTACH_WIDGET,
        		//XmNverticalSpacing,     _leftOrTop,
        		XmNtopOffset,		((int)_leftOrTop),
        		NULL );
                                                 		        		 
//	DRAW LINE CLOSEST TO INSIDE WIDGET  
	Widget 		leftW, rightW;
	XtVaGetValues ( _w,  XmNleftWidget,  &leftW, 
        				XmNrightWidget, &rightW,
        				NULL );        				       				      				
	        		
// 	DRAW TICKS
	int 	stepSize= int(insideLength-1) / (_numTicks - 1);
	int 	excess 	= int(insideLength-1) % (_numTicks - 1);
		
	double 	temp1 	= (double)excess / (double)(_numTicks - 1);
	double 	temp2 = temp1;
	
	int	pointY;
	int	pointX1 = 0;
	int	pointX2 = 0 + _longTickLength;
	
	for ( int i=0; i<_numTicks; ++i) {

	   pointY = int((i*stepSize)+temp1);
	   XDrawLine (XtDisplay(_ruler), XtWindow(_ruler), _gc, 
				pointX1, pointY,pointX2,pointY);
	   temp1 = temp1 + temp2;
        }             
   }
}
