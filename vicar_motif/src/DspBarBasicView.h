//////////////////////////////////////////////////////////////
// DspBarBasicView.h: An abstract class to draw frame (aroundDspAxis)
/////////////////////////////////////////////////////////////
#ifndef DSPBARBASICVIEW_H
#define DSPBARBASICVIEW_H
#include "DspUIComponent.h"
#include <Xm/Xm.h>
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream>	//temporary


class DspBarBasicView : public DspUIComponent{

  private:

	static void	  displayCallback( Widget,
					XtPointer clientData,
					XtPointer );

  protected:
 
	Widget 		_ruler;
	Widget		_widgetCap;
	//Widget	_insideWidget;
    	GC 		_gc;
    	//int		_numTicks;
    	//Dimension	_width, _height;
	//Dimension 	_drawOffset;
	//Dimension 	_longTickLength;
	//Dimension	_leftOrTop;
	//Dimension	_rightOrBottom;
	
	virtual void update(){};
	virtual void	display()=0;

  public:

    	DspBarBasicView ( Widget, char *,Widget);
    	~DspBarBasicView ();
    	//inline Dimension getTickLength() {return _longTickLength; }
    	//inline void setOffset(Dimension sideA, Dimension sideB) 
//    			{ _leftOrTop = 1+ sideA; _rightOrBottom = 1+ sideB; }
	virtual const char *const className() { return "DspBarBasicView"; }
};
#endif

