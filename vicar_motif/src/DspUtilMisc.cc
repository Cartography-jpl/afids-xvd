///////////////////////////////////////////////////////////////////
//	DspUtilMisc.cc
//	
//		FUTURE:  setStretchForBw, setStretchForColor
///////////////////////////////////////////////////////////////////
#include "rts_inst_display.h"
#include <Xm/Xm.h>


///////////////////////////////////////////////////////////////////
//	CreateBlankCursor
///////////////////////////////////////////////////////////////////
Cursor createBlankCursor(Widget iw)
{
Cursor		blankCursor;
Pixmap 		blankPixmap;
XGCValues	gc_values;
GC		gc;
unsigned int	height, width;
XColor 		black;
Colormap 	colormap;


    
//	CREATE SMALL PIXMAP (1x1) FOR "BLANK" CURSOR
   	width = 1;
   	height = 1;
   	blankPixmap = XCreatePixmap(XtDisplay(iw),
			RootWindowOfScreen(XtScreen(iw)),
			width, height, 1);

//	CREATE TEMP GC
   	gc_values.foreground = 0;
   	gc_values.background = 0;	
   	gc = XCreateGC(XtDisplay(iw), blankPixmap,
		GCForeground | GCBackground, &gc_values);

//	FILL PIXMAP
   	XFillRectangle(XtDisplay(iw), blankPixmap, gc,
		0, 0, width, height);
		
//	SET BACKGROUND & FORGROUND COLORS
	colormap = DefaultColormapOfScreen(XtScreen((Widget)iw));			
	XParseColor(XtDisplay(iw), colormap, "black", &black);
		
//	CREATE CURSOR
   	blankCursor = XCreatePixmapCursor( XtDisplay(iw), 
   		blankPixmap, blankPixmap, &black,  &black, 0,0);

//	DESTROY PIXMAP
	XFreePixmap(XtDisplay(iw), blankPixmap); 
	
//	DISPLAY CURSOR
    	if (XtIsRealized((Widget) iw)) 
		XDefineCursor( XtDisplay(iw), XtWindow(iw), blankCursor);
		
    
//  RETURN
    return (blankCursor);
}
///////////////////////////////////////////////////////////////////
//      fillImageStretchLutWithRaw()
//              set stretch to ramp (IE off)
///////////////////////////////////////////////////////////////////
void   fillImageStretchLutWithRaw( int * lut1)
{
	for (int i = 0; i<256; i++) {
		lut1[i]=i;
	}
	return; // 
}

///////////////////////////////////////////////////////////////////
//      fillRGBImageStretchLutWithRaw()
//              set stretch to ramp (IE off)
///////////////////////////////////////////////////////////////////
void   fillRGBImageStretchLutWithRaw( int * lut1, int *lut2, int *lut3)
{
	fillImageStretchLutWithRaw(lut1);
	fillImageStretchLutWithRaw(lut2);
	fillImageStretchLutWithRaw(lut3);
	return; // 
}

///////////////////////////////////////////////////////////////////
//      setZoomParams2Off
///////////////////////////////////////////////////////////////////
//void    setZoomParams2Off( int * zoomX, int *zoomY, int *panY, int * panX ,int * oddEvenFlag){
void    setZoomParams2Off( int * , int *, int *panY, int * panX ,int * oddEvenFlag){
        *panY = 0;
        *panX = 0;
        *oddEvenFlag = 0;
}
///////////////////////////////////////////////////////////////////
//      setZoomParamsDouble (IE zoom by 2)
///////////////////////////////////////////////////////////////////
void    setZoomParamsDouble( int * zoomX, int *zoomY, int *panY, int * panX ,int * oddEvenFlag){
        *zoomX = 2;
        *zoomY = 2;
        *panY = 0;
        *panX = 0;
        *oddEvenFlag = 0;
}
///////////////////////////////////////////////////////////////////
//      setPseudoModeOff
//              for preventing pseudo mode from coming up in 
//              specified image
///////////////////////////////////////////////////////////////////
int     setPseudoModeOff()
{
        return(0);
}
///////////////////////////////////////////////////////////////////
//      setPseudoModeOn
//              
///////////////////////////////////////////////////////////////////
int     setPseudoModeOn()
{
        return(1);
}
///////////////////////////////////////////////////////////////////
//      
///////////////////////////////////////////////////////////////////
//void    nofillNimsIntBuffer(unsigned int * buf1, unsigned int * buf2, unsigned int * buf3) 
void    nofillNimsIntBuffer(unsigned int * , unsigned int * , unsigned int * ) 
{
	// nop
}
