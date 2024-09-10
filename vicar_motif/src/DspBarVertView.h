//////////////////////////////////////////////////////////////
// DspBarVertView.h:
/////////////////////////////////////////////////////////////
#ifndef DSPBARVERTVIEW_H
#define DSPBARVERTVIEW_H
#include "DspBarBasicView.h"


class DspBarVertView : public DspBarBasicView {

  protected:

  public:

    	DspBarVertView ( Widget, char *, Widget ) ;
    	~DspBarVertView () {}
    	virtual void display();
    	virtual const char *const className() { return "DspBarVertView"; }
};
#endif

