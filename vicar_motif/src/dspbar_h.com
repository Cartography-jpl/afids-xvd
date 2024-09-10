$!****************************************************************************
$!
$! Build proc for MIPL module dspbar_h
$! VPACK Version 1.8, Monday, November 06, 1995, 21:54:15
$!
$! Execute by entering:		$ @dspbar_h
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
$ write sys$output "*** module dspbar_h ***"
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
$ write sys$output "Invalid argument given to dspbar_h.com file -- ", primary
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
$   if F$SEARCH("dspbar_h.bld") .eqs. "" then gosub Build_File
$   if (primary .eqs. " ")
$   then
$      @dspbar_h.bld "STD"
$   else
$      @dspbar_h.bld "''primary'" "''secondary'"
$   endif
$ Return
$!#############################################################################
$Repack_File:
$ create dspbar_h.repack
$ DECK/DOLLARS="$ VOKAGLEVE"
$ vpack dspbar_h.com -mixed -
	-s DspBarBasicView.h DspBarVertView.h DspBarHorizView.h
$ Exit
$ VOKAGLEVE
$ Return
$!#############################################################################
$Source_File:
$ create DspBarBasicView.h
$ DECK/DOLLARS="$ VOKAGLEVE"
//////////////////////////////////////////////////////////////
// DspBarBasicView.h: An abstract class to draw frame (aroundDspAxis)
/////////////////////////////////////////////////////////////
#ifndef DSPBARBASICVIEW_H
#define DSPBARBASICVIEW_H
#include "DspUIComponent.h"
#include <Xm/Xm.h>
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream.h>	//temporary


class DspBarBasicView : public DspUIComponent{

  private:

	static void	  displayCallback( Widget,
					XtPointer clientData,
					XtPointer );

  protected:
 
	Widget 		_ruler;
	Widget		_widgetCap;
	//Widget	_insideWidget;
    	GC 		_gc;
    	//int		_numTicks;
    	//Dimension	_width, _height;
	//Dimension 	_drawOffset;
	//Dimension 	_longTickLength;
	//Dimension	_leftOrTop;
	//Dimension	_rightOrBottom;
	
	virtual void update(){};
	virtual void	display()=0;

  public:

    	DspBarBasicView ( Widget, char *,Widget);
    	~DspBarBasicView ();
    	//inline Dimension getTickLength() {return _longTickLength; }
    	//inline void setOffset(Dimension sideA, Dimension sideB) 
//    			{ _leftOrTop = 1+ sideA; _rightOrBottom = 1+ sideB; }
	virtual const char *const className() { return "DspBarBasicView"; }
};
#endif

$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspBarVertView.h
$ DECK/DOLLARS="$ VOKAGLEVE"
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

$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspBarHorizView.h
$ DECK/DOLLARS="$ VOKAGLEVE"
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

$ VOKAGLEVE
$ Return
$!#############################################################################
