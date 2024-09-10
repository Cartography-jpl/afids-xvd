///////////////////////////////////////////////////////////////////
//  DspUtilMisc.h
///////////////////////////////////////////////////////////////////
#ifndef DSPUTILMISC_H
#define DSPUTILMISC_H
#include <Xm/Xm.h>

	Cursor	createBlankCursor(Widget iw);  // external utility routine
	void    setZoomParams2Off( int * zoomX, int *zoomY, int *panY, int * panX ,int*oddEvenFlag);	 
	int     setPseudoModeOff();
	int     setPseudoModeOn();
	void	fillImageStretchLutWithRaw(int * lut1);
	void	fillRGBImageStretchLutWithRaw(int * lut1, int *, int *);
	void    nofillNimsIntBuffer(unsigned int *  buf1, unsigned int * buf2, unsigned int * buf3);
	
#endif
