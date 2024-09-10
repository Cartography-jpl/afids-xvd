//////////////////////////////////////////////////////////////
// DspAxisVertView.h:
/////////////////////////////////////////////////////////////
#ifndef DSPAXISVERTVIEW_H
#define DSPAXISVERTVIEW_H
#include "DspAxisBasicView.h"

class DspAxisVertView : public DspAxisBasicView {

  protected:	

  public:

    	DspAxisVertView ( Widget , char *, Widget) ;
    	~DspAxisVertView () {}
    	virtual void display();
    	virtual const char *const className() { return "DspAxisVertView"; }
};
#endif

