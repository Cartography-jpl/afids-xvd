//////////////////////////////////////////////////////////////
// DspAxisHorizView.h:
/////////////////////////////////////////////////////////////
#ifndef DSPAXISHORIZVIEW_H
#define DSPAXISHORIZVIEW_H
#include "DspAxisBasicView.h"


class DspAxisHorizView : public DspAxisBasicView {

  protected:

  public:

    	DspAxisHorizView ( Widget, char *, Widget ) ;
    	~DspAxisHorizView () {}
    	virtual void display();
    	virtual const char *const className() { return "DspAxisHorizView"; }
};
#endif

