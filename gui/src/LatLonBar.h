//////////////////////////////////////////////////////////////
// LatLonBar.h:
/////////////////////////////////////////////////////////////
#ifndef LATLONBAR_H
#define LATLONBAR_H
#include "UIComponent.h"

//#include "CursorLatLonView.h"
class CursorLatLonView;

class Cmd;
class ImageData;
class BasicImageView;

class LatLonBar : public UIComponent {

  protected:

    CursorLatLonView *_cursorLatLonView;

  public:

    LatLonBar ( Widget, char *, BasicImageView *, ImageData *);
    virtual CursorLatLonView *getCursorLatLonView(){return _cursorLatLonView; }

    virtual const char *const className() { return "LatLonBar"; }
};
#endif

