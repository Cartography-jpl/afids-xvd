////////////////////////////////////////////////////////////////////
// DspCursWindow.h: Creates a top level window for cursor position display
////////////////////////////////////////////////////////////////////
#ifndef DSPCURSWINDOW_H
#define DSPCURSWINDOW_H
#include "MainWindow.h"
#include <Xm/Xm.h>
#include <assert.h>


class Cmd;

class DspCursBox;
class ImageData;
class ImageDisplayView;

class DspCursWindow : public MainWindow {

    
  protected:

    	Boolean			_pop;
      	DspCursBox *		_cursorRangeDnBox;
    	ImageData      *	_imageData;
    	ImageDisplayView *	_imageView;
    	Widget			_parent; 
    	Widget 			_form;
    	int			_pixelSize;

  public:
  
     	DspCursWindow ( char *, ImageData *, ImageDisplayView *, Widget, int );
    	~DspCursWindow()	{}
    	virtual void		restoreState();
    	virtual void		saveState();
    	virtual Widget 		createWorkArea ( Widget );
    	virtual void 		initialize();
    	
 
};
#endif
