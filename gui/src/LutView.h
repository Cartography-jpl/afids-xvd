///////////////////////////////////////////////////////////////////
// LutView.h: Abstact base class for LUT views
///////////////////////////////////////////////////////////////////
#ifndef LUTVIEW_H
#define LUTVIEW_H
#include "UIComponent.h"
#include "Lut.h"
#include <iostream>

class LutView : public UIComponent {

  protected:

	Lut *_lut;	// red
	Lut *_lut1;	// green
	Lut *_lut2;	// blue

  public:

        LutView ( char *name, Lut *r, Lut *g, Lut *b ) 
	    	: UIComponent (name) 
	    { _lut = r; _lut1 = g; _lut2 = b; }

	LutView ( char *name, Lut *r )
                : UIComponent (name)
            { _lut = r; _lut1 = 0; _lut2 = 0; }

	virtual ~LutView() 
	    { if (_lut) _lut->detachView(this);
	      if (_lut) _lut1->detachView(this);
	      if (_lut) _lut2->detachView(this);
	    }

        virtual void update()=0;

        virtual const char *const className() { return "LutView"; }
};
#endif
