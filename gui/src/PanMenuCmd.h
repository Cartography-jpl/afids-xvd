/////////////////////////////////////////////////////////////
// PanMenuCmd.h: Include file to handle the Pan command
//                button from the Image widget.
/////////////////////////////////////////////////////////////
#ifndef PANMENUCMD_H
#define PANMENUCMD_H
#include "NoUndoCmd.h"
#include "PanToolWindow.h"

class MainWindow;
class ImageData;
class Lut;

class PanMenuCmd : public NoUndoCmd {

  private:

     int        _created;
     ImageData  * _imageData;
     Widget     _imageViewWidget;
     MainWindow *_panToolWindow;
     Lut	*_rlut, *_glut, *_blut, *_rplut, *_gplut, *_bplut;

  protected:
    
    virtual void doit();

  public:

    PanMenuCmd ( char*, int, ImageData*, Widget,
		Lut *rlut=0,  Lut *glut=0,  Lut *blut=0,
		Lut *rplut=0, Lut *gplut=0, Lut *bplut=0);
    virtual const char *const className () { return "PanMenuCmd"; }
};
#endif
