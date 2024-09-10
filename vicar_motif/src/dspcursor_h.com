$!****************************************************************************
$!
$! Build proc for MIPL module dspcursor_h
$! VPACK Version 1.8, Monday, April 01, 1996, 21:52:48
$!
$! Execute by entering:		$ @dspcursor_h
$!
$! The primary option controls how much is to be built.  It must be in
$! the first parameter.  Only the capitalized letters below are necessary.
$!
$! Primary options are:
$!   ALL         Build a private version, and unpack the PDF and DOC files.
$!   STD         Build a private version, and unpack the PDF file(s).
$!   SYStem      Build the system version with the CLEAN option, and
$!               unpack the PDF and DOC files.
$!   CLEAN       Clean (delete/purge) parts of the code, see secondary options
$!   UNPACK      All files are created.
$!   REPACK      Only the repack file is created.
$!   SOURCE      Only the source files are created.
$!   SORC        Only the source files are created.
$!               (This parameter is left in for backward compatibility).
$!
$!   The default is to use the STD parameter if none is provided.
$!
$!****************************************************************************
$!
$! The secondary options modify how the primary option is performed.
$! Note that secondary options apply to particular primary options,
$! listed below.  If more than one secondary is desired, separate them by
$! commas so the entire list is in a single parameter.
$!
$! Secondary options are:
$! CLEAN:
$!   OBJ        Delete object and list files, and purge executable (default)
$!   SRC        Delete source and make files
$!
$!****************************************************************************
$!
$ write sys$output "*** module dspcursor_h ***"
$!
$ Create_Source = ""
$ Create_Repack =""
$!
$! Parse the primary option, which must be in p1.
$ primary = f$edit(p1,"UPCASE,TRIM")
$ if (primary.eqs."") then primary = " "
$ secondary = f$edit(p2,"UPCASE,TRIM")
$!
$ if primary .eqs. "UNPACK" then gosub Set_Unpack_Options
$ if (f$locate("ALL", primary) .eqs. 0) then gosub Set_All_Options
$ if (f$locate("STD", primary) .eqs. 0) then gosub Set_Default_Options
$ if (f$locate("SYS", primary) .eqs. 0) then gosub Set_Sys_Options
$ if primary .eqs. " " then gosub Set_Default_Options
$ if primary .eqs. "REPACK" then Create_Repack = "Y"
$ if primary .eqs. "SORC" .or. primary .eqs. "SOURCE" then Create_Source = "Y"
$!
$ if (Create_Source .or. Create_Repack) -
        then goto Parameter_Okay
$ write sys$output "Invalid argument given to dspcursor_h.com file -- ", primary
$ write sys$output "For a list of valid arguments, please see the header of"
$ write sys$output "of this .com file."
$ exit
$!
$Parameter_Okay:
$ if Create_Repack then gosub Repack_File
$ if Create_Source then gosub Source_File
$ exit
$!
$ Set_Unpack_Options:
$   Create_Repack = "Y"
$   Create_Source = "Y"
$ Return
$!
$ Set_Default_Options:
$   Create_Source = "Y"
$   Create_Build = "Y"
$   Do_Make = "Y"
$ Return
$!
$ Set_All_Options:
$   Create_Source = "Y"
$   Create_Build = "Y"
$   Do_Make = "Y"
$ Return
$!
$ Set_Sys_Options:
$   Create_Source = "Y"
$   Create_Build = "Y"
$   Do_Make = "Y"
$ Return
$!
$Run_Make_File:
$   if F$SEARCH("dspcursor_h.bld") .eqs. "" then gosub Build_File
$   if (primary .eqs. " ")
$   then
$      @dspcursor_h.bld "STD"
$   else
$      @dspcursor_h.bld "''primary'" "''secondary'"
$   endif
$ Return
$!#############################################################################
$Repack_File:
$ create dspcursor_h.repack
$ DECK/DOLLARS="$ VOKAGLEVE"
$ vpack dspcursor_h.com -mixed -
	-s DspCurs2ImageGlue.h DspCursorView.h DspCursBox.h DspCursWindow.h -
	   DspCursCmd.h
$ Exit
$ VOKAGLEVE
$ Return
$!#############################################################################
$Source_File:
$ create DspCurs2ImageGlue.h
$ DECK/DOLLARS="$ VOKAGLEVE"
////////////////////////////////////////////////////////////////////////
// DspCurs2ImageGlue.h: See DspImageToCursorDnViewGlue
////////////////////////////////////////////////////////////////////////
#ifndef DSPCURSORIMAGE2GLUE_H
#define DSPCURSORIMAGE2GLUE_H
#include "BasicImageView.h"
#include "ImageData.h"
#include "DspCursorView.h"

class DspCursorView;

class DspCurs2ImageGlue : public BasicImageView {

 protected:
 
   		ModeType 	 	_oldMode;
   		ImageData *	 	_image;
   		DspCursorView *	_cursorView;

 public:

   DspCurs2ImageGlue (ImageData *image, DspCursorView *cursorView);
   ~DspCurs2ImageGlue( ){ /* no way to detachView on image, yet*/ }
   virtual void update();
   virtual const char *const className() { return  "DspImage2CursGlue"; }
   
};
#endif

$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspCursorView.h
$ DECK/DOLLARS="$ VOKAGLEVE"
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
$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspCursBox.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspCursBox.h:
///////////////////////////////////////////////////////////////////
#ifndef DSPCURSBOX_H
#define DSPCURSBOX_H
#include "UIComponent.h"

class CursorPositionView;
class DspCursorView;
class CursorModel;
class ImageData;
class BasicImageView;

class DspCursBox : public UIComponent {

 
   protected:
   
 
	DspCursorView * 	_cursorRangeDnView;
	CursorPositionView *	_cursorPositionView;
        CursorModel *        	_cursorModel;         // model
 	static XtResource 	_resources[];  	
	static String 		_defaults[]; 		   // resource defaults.
        int			_enableLoc;
        

   public:

	DspCursBox( Widget, char *,  ImageData *, BasicImageView *, int  );
	~DspCursBox();

        virtual const char * const className() { return ("DspCursBox"); }

};
#endif
$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspCursWindow.h
$ DECK/DOLLARS="$ VOKAGLEVE"
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
$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspCursCmd.h
$ DECK/DOLLARS="$ VOKAGLEVE"
/////////////////////////////////////////////////////////////
// DspCursCmd.h: Include file to handle the CursorPos command
//                button from the Browse app.
/////////////////////////////////////////////////////////////
#ifndef DSPCURSCMD_H
#define DSPCURSCMD_H
#include "NoUndoCmd.h"
#include "DspCursWindow.h"
#include <Xm/Xm.h>

class ImageData;
class ImageDisplayView;

class DspCursCmd : public NoUndoCmd {

  private:

     int              	_created;
     Widget		_parent;
     DspCursWindow *    _cursorRangeDnWindow;
     ImageData *       	_imageData;
     ImageDisplayView *	_imageView;
     char *            	_title;
     
  protected:
  
   virtual void doit();

  public:

    DspCursCmd ( char*, char*, int, ImageData*, ImageDisplayView *,Widget, int pixelSize );
    virtual void mapViews() ;
    virtual void unmapViews();
    virtual const char *const className () { return "DspCursCmd"; }
};
#endif

$ VOKAGLEVE
$ Return
$!#############################################################################
