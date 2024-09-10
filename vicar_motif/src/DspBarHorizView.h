//////////////////////////////////////////////////////////////
// DspBarHorizView.h:
/////////////////////////////////////////////////////////////
#ifndef DSPBARHORIZVIEW_H
#define DSPBARHORIZVIEW_H
#include "DspBarBasicView.h"


class DspBarHorizView : public DspBarBasicView {

  protected:

  public:

    	DspBarHorizView ( Widget, char *,Widget ) ;
    	~DspBarHorizView () {}
    	virtual void display();
    	virtual const char *const className() { return "DspBarHorizView"; }
};
#endif

