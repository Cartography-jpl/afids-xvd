////////////////////////////////////////////////////////////////
// DspCursorView.h
//
//	Abstract class for views containing cursor information.
//	
///////////////////////////////////////////////////////////////
#ifndef DSPCURSORVIEW_H
#define DSPCURSORVIEW_H
#include "CursorBasicView.h"
#include "DspCurs2ImageGlue.h"
#include "UIComponent.h"
#include "XvicImage.h"
#include "ImageData.h"

class DspCurs2ImageGlue;
class CursorModel;
class ImageData;

class DspCursorView  :  public CursorBasicView {

  friend class DspCurs2ImageGlue;

  protected:
  
  	static XtResource 	_resources[];  	
  	int			_numbRows;
  	int			_numbColumns;  	
  	int			_skipRows; // height of pixels to skip
  	int			_skipColumns; // width of pixels to skip

  	short 			_numColumnsResource;  //
  	short 			_columnsResource;  // width of text widget
  	int			_midRow;
  	int			_midColumn;
	int			_numbLines;
	int			_numbSamples;
	
	DspCurs2ImageGlue *	_glue;
	unsigned char  		_pixelBuffer[132];    // buffer for holding pixel value
	char 			_blankString[132];
	int			_pixelSize;
	
	Widget	**_labelRed, **_textRed, **_formRed;
	Boolean		_redDnEnable;	// on-off 	
	char *		_redDnLabel, _valueRedDn; // label set by resource

	Widget	**_labelGrn, **_textGrn, **_formGrn;
	Boolean		_greenDnEnable;	// on-off 
	char *		_greenDnLabel, _valueGreenDn;// label set by resource
	
	Widget	**_labelBlu, **_textBlu, **_formBlu;
	Boolean		_blueDnEnable;	// on-off
	char *		_blueDnLabel, _valueBlueDn;// label set by resource
	
	Widget	**_labelBw,  **_textBw,  **_formBw;
	Boolean		_bwDnEnable;	// on-off 
	char *		_bwDnLabel, _valueBwDn;	// label set by resource


// 	PURE VIRTUAL FUCTION:  TO FILL IN BLANK VALUES IN ALL IT'S DISPLAYS
	virtual void  	blankSubViews();

//	PURE VIRTUAL FUCTION:  CALLED BY CALLBACK TO UPDATE DISPLAYS ON ALL CURSOR SUBVIEWS
	virtual void  	cursorMoved ( XvicImageCallbackStruct * cb ) ;

//	ADD MINI VIEW
	virtual void 	addNewSubView( Widget w, char * displayName,
        				Widget *form, Widget * label, 
        				Widget * textfield, Boolean enabled );

//	LOCAL FUNCTION FOR GETTING A DN VALUE AND ITS STRING EQUIV.   
	virtual void 	getValueString( ColorType color,  
				int x, int y, char * newValueString , 
				Boolean enabled = True);
 public:
 
//	CONSTRUCTOR/DESTRUCTOR
	DspCursorView( 	Widget parent, 
				char * name,
				CursorModel * cursorModel,
				ImageData * imageData,
				unsigned char bitFlags,
				int pixelSize) ;
			
	~DspCursorView();
	virtual void	removeSubView_(Widget form, Widget label,
                                Widget textfield);
  
//	PURE VIRTUAL FUCTION:  CALLED TO CREATE/REMOVE SUBVIEWS
	virtual void  	createCursorDisplays() ; 
	virtual void  	removeCursorDisplays(); // pure virtual
	
//	CLASSNAME
	virtual const char *const className() { return "DspCursorView" ; } 

};
#endif
