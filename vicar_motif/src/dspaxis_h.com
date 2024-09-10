$!****************************************************************************
$!
$! Build proc for MIPL module dspaxis_h
$! VPACK Version 1.8, Monday, April 01, 1996, 21:52:47
$!
$! Execute by entering:		$ @dspaxis_h
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
$ write sys$output "*** module dspaxis_h ***"
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
$ write sys$output "Invalid argument given to dspaxis_h.com file -- ", primary
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
$   if F$SEARCH("dspaxis_h.bld") .eqs. "" then gosub Build_File
$   if (primary .eqs. " ")
$   then
$      @dspaxis_h.bld "STD"
$   else
$      @dspaxis_h.bld "''primary'" "''secondary'"
$   endif
$ Return
$!#############################################################################
$Repack_File:
$ create dspaxis_h.repack
$ DECK/DOLLARS="$ VOKAGLEVE"
$ vpack dspaxis_h.com -mixed -
	-s DspAxisBasicView.h DspAxisVertView.h DspAxisHorizView.h
$ Exit
$ VOKAGLEVE
$ Return
$!#############################################################################
$Source_File:
$ create DspAxisBasicView.h
$ DECK/DOLLARS="$ VOKAGLEVE"
//////////////////////////////////////////////////////////////
// DspAxisBasicView.h: An abstract class to show axis
/////////////////////////////////////////////////////////////
#ifndef DSPAXISBASICVIEW_H
#define DSPAXISBASICVIEW_H
#include "DspUIComponent.h"
#include <Xm/Xm.h>
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream.h>	//temporary


class DspAxisBasicView : public DspUIComponent{

  private:

	static XtResource _resources[];
	static void	  displayCallback( Widget,
					XtPointer clientData,
					XtPointer );

  protected:
 
	Widget 		_ruler;
	Widget		_insideWidget;
    	GC 		_gc;
    	int		_numTicks;
    	Dimension	_width, _height;
	Dimension 	_drawOffset;
	Dimension 	_longTickLength;
	Dimension	_leftOrTop;
	Dimension	_rightOrBottom;
	
	virtual void update(){};
	
	virtual void	display()=0;
	

  public:

    	DspAxisBasicView ( Widget, char *, Widget );
    	~DspAxisBasicView ();
    	inline Dimension getTickLength() {return _longTickLength; }
    	inline void setOffset(Dimension sideA) { _leftOrTop = sideA; }
	virtual const char *const className() { return "DspAxisBasicView"; }
};
#endif

$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspAxisVertView.h
$ DECK/DOLLARS="$ VOKAGLEVE"
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

$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspAxisHorizView.h
$ DECK/DOLLARS="$ VOKAGLEVE"
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

$ VOKAGLEVE
$ Return
$!#############################################################################
