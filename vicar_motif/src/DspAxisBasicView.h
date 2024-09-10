//////////////////////////////////////////////////////////////
// DspAxisBasicView.h: An abstract class to show axis
/////////////////////////////////////////////////////////////
#ifndef DSPAXISBASICVIEW_H
#define DSPAXISBASICVIEW_H
#include "DspUIComponent.h"
#include <Xm/Xm.h>
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream>	//temporary


class DspAxisBasicView : public DspUIComponent{

  private:

	static XtResource _resources[];
	static void	  displayCallback( Widget,
					XtPointer clientData,
					XtPointer );

  protected:
 
	Widget 		_ruler;
	Widget		_insideWidget;
    	GC 		_gc;
    	int		_numTicks;
    	Dimension	_width, _height;
	Dimension 	_drawOffset;
	Dimension 	_longTickLength;
	Dimension	_leftOrTop;
	Dimension	_rightOrBottom;
	
	virtual void update(){};
	
	virtual void	display()=0;
	

  public:

    	DspAxisBasicView ( Widget, char *, Widget );
    	~DspAxisBasicView ();
    	inline Dimension getTickLength() {return _longTickLength; }
    	inline void setOffset(Dimension sideA) { _leftOrTop = sideA; }
	virtual const char *const className() { return "DspAxisBasicView"; }
};
#endif

